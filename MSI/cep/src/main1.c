/*

STM32F407 Motion Sensor + Buzzer + I2C LCD (16x2) + RFID (RC522)
Connections:
BUZZER:
PB0 -> TIM3_CH3 (PWM Output)
-> 5V (if active buzzer) or driver
-> GND

ACCELEROMETER LIS302DL (SPI1):
PA5 -> SCK
PA6 -> MISO
PA7 -> MOSI
PE3 -> CS

LCD 16x2 (I2C PCF8574):
PB6 -> I2C1 SCL
PB7 -> I2C1 SDA
Address = 0x27 or 0x3F

SERVO:
-> 5V
-> GND
PD14  -> TIM4_CH3

RFID (RC522) - SPI2:
PB13 -> SCK
PB14 -> MISO
PB15 -> MOSI
PD8  -> CS
PD9  -> RST
PC6  -> IRQ (EXTI9_5, Falling Edge)
*/

#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------- HANDLES -------------------- */
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim3;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim4;

/* -------------------- ACC REGISTERS -------------------- */
#define LIS302DL_CTRL_REG1 0x20
#define LIS302DL_OUT_X     0x29
#define LIS302DL_OUT_Y     0x2B
#define LIS302DL_OUT_Z     0x2D

/* -------------------- LCD SETTINGS -------------------- */
#define LCD_ADDR 0x27 << 1

/* -------------------- BUZZER NOTES -------------------- */
#define NOTE_E4 330

/* -------------------- RFID RC522 REGISTERS -------------------- */
#define RC522_COMMAND_REG       0x01
#define RC522_FIFO_DATA_REG     0x04
#define RC522_FIFO_LEVEL_REG    0x0A
#define RC522_DIV_IRQ_REG       0x05
#define RC522_STATUS1_REG       0x06
#define RC522_STATUS2_REG       0x08
#define RC522_RX_MODE_REG       0x13
#define RC522_TX_MODE_REG       0x14
#define RC522_TX_CONTROL_REG    0x15
#define RC522_MOD_WIDTH_REG     0x27
#define RC522_MODE_REG          0x11
#define RC522_RX_GAIN_REG       0x26
#define RC522_T_MODE_REG        0x2A
#define RC522_T_RELOAD_L_REG    0x2D
#define RC522_T_RELOAD_H_REG    0x2C
#define RC522_CONTROL_REG       0x0C
#define RC522_COMMAND_IDLE      0x00
#define RC522_COMMAND_MEM       0x01
#define RC522_COMMAND_GEN_ID    0x02
#define RC522_COMMAND_CRC       0x03
#define RC522_COMMAND_TRANSMIT  0x04
#define RC522_COMMAND_RECEIVE   0x08
#define RC522_COMMAND_TRANSREC  0x0C
#define RC522_COMMAND_RESERVED  0x0D
#define RC522_COMMAND_RESET     0x0F

/* RFID Commands */
#define PICC_REQUEST_A          0x26
#define PICC_WUPA               0x52
#define PICC_SELECT_CL1         0x93
#define PICC_SELECT_CL2         0x95
#define PICC_SELECT_CL3         0x97
#define PICC_AUTH_KEY_A         0x60
#define PICC_AUTH_KEY_B         0x61
#define PICC_READ               0x30
#define PICC_WRITE              0xA0
#define PICC_HALT               0x50

/* -------------------- CS MACROS -------------------- */
#define ACC_CS_LOW()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define ACC_CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

#define RFID_CS_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)
#define RFID_CS_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)

#define RFID_RST_HIGH()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define RFID_RST_LOW()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)

/* -------------------- MOTION DATA -------------------- */
typedef struct {
    int8_t x, y, z;
    uint16_t motion_count;
    uint32_t last_motion_time;
} MotionData;

/* -------------------- RFID DATA -------------------- */
typedef struct {
    uint8_t uid[10];
    uint8_t uid_len;
    uint8_t card_detected;
    uint32_t last_detect_time;
    uint8_t card_type;
} RFIDData;

MotionData motion = {0, 0, 0, 0, 0};
RFIDData rfid_data = {0};
volatile uint8_t card_interrupt_flag = 0;

/* -------------------- FUNCTION PROTOTYPES -------------------- */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
static void MX_EXTI_Init(void);

/* Accelerometer */
void ACC_Init(void);
void ACC_WriteReg(uint8_t reg, uint8_t data);
int8_t ACC_ReadAxis(uint8_t reg);

/* Buzzer */
void buzzer_play(uint32_t freq);
void buzzer_stop(void);

/* LCD */
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_clear(void);
void lcd_print(char *str);
void lcd_set_cursor(uint8_t row, uint8_t col);

/* Servo */
void servo_set_angle(uint8_t angle);

/* RFID */
void RFID_Init(void);
void RFID_Reset(void);
void RFID_WriteReg(uint8_t addr, uint8_t val);
uint8_t RFID_ReadReg(uint8_t addr);
void RFID_SetBitMask(uint8_t reg, uint8_t mask);
void RFID_ClearBitMask(uint8_t reg, uint8_t mask);
uint8_t RFID_Request(uint8_t req_code, uint8_t *tag_type);
uint8_t RFID_Anticoll(uint8_t *serial_num);
uint8_t RFID_Select(uint8_t *serial_num);
uint8_t RFID_Auth(uint8_t auth_mode, uint8_t block_addr, uint8_t *key, uint8_t *serial_num);
uint8_t RFID_Read(uint8_t block_addr, uint8_t *recv_data);
void RFID_Halt(void);

/* Interrupt */
void EXTI9_5_IRQHandler(void);

/* Motion Detection */
void update_lcd_display(void);
uint8_t is_motion_detected(void);


/* ======================== MAIN ======================== */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Initialize all peripherals */
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_I2C1_Init();
    MX_EXTI_Init();

    /* Initialize devices */
    ACC_Init();
    RFID_Init();
    lcd_init();

    /* Start PWM for servo */
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

    /* Initial LCD welcome screen */
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Motion + RFID");
    lcd_set_cursor(1, 0);
    lcd_print("Initializing...");
    HAL_Delay(2000);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Motion: 0");
    lcd_set_cursor(1, 0);
    lcd_print("RFID Ready");
    HAL_Delay(1000);

    motion.last_motion_time = 0;
    motion.motion_count = 0;

    /* ===================== MAIN LOOP ===================== */
    while(1)
    {
        /* ---- MOTION DETECTION ---- */
        motion.x = ACC_ReadAxis(LIS302DL_OUT_X);
        motion.y = ACC_ReadAxis(LIS302DL_OUT_Y);
        motion.z = ACC_ReadAxis(LIS302DL_OUT_Z);

        int16_t accel_mag = (motion.x * motion.x) +
                            (motion.y * motion.y) +
                            (motion.z * motion.z);

        if(accel_mag > 150)
        {
            uint32_t current_time = HAL_GetTick();

            if((current_time - motion.last_motion_time) > 500)
            {
                motion.motion_count++;
                motion.last_motion_time = current_time;

                buzzer_play(NOTE_E4);
                HAL_Delay(200);
                buzzer_stop();

                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("Motion: ");
                char count_str[5];
                sprintf(count_str, "%d", motion.motion_count);
                lcd_print(count_str);

                lcd_set_cursor(1, 0);
                char accel_str[16];
                sprintf(accel_str, "X:%d Y:%d", motion.x, motion.y);
                lcd_print(accel_str);

                servo_set_angle(90);
                HAL_Delay(300);
                servo_set_angle(0);
                HAL_Delay(300);
            }
        }

        /* ---- RFID DETECTION ---- */
        if(card_interrupt_flag)
        {
            card_interrupt_flag = 0;
            uint32_t current_time = HAL_GetTick();

            /* Debounce: 1000ms between card reads */
            if((current_time - rfid_data.last_detect_time) > 1000)
            {
                uint8_t tag_type[2];
                uint8_t uid[10] = {0};

                if(RFID_Request(PICC_REQUEST_A, tag_type) == 0)
                {
                    if(RFID_Anticoll(uid) == 0)
                    {
                        rfid_data.card_detected = 1;
                        rfid_data.uid_len = 4;
                        memcpy(rfid_data.uid, uid, rfid_data.uid_len);
                        rfid_data.last_detect_time = current_time;

                        /* BUZZER ALERT */
                        buzzer_play(NOTE_E4);
                        HAL_Delay(100);
                        buzzer_stop();
                        HAL_Delay(50);
                        buzzer_play(NOTE_E4);
                        HAL_Delay(100);
                        buzzer_stop();

                        /* UPDATE LCD */
                        lcd_clear();
                        lcd_set_cursor(0, 0);
                        lcd_print("RFID: ");
                        
                        char rfid_str[16];
                        sprintf(rfid_str, "%02X%02X%02X%02X", 
                                rfid_data.uid[0], rfid_data.uid[1],
                                rfid_data.uid[2], rfid_data.uid[3]);
                        lcd_print(rfid_str);

                        lcd_set_cursor(1, 0);
                        lcd_print("Card Detected!");

                        RFID_Halt();
                        HAL_Delay(500);
                    }
                }
            }
        }

        HAL_Delay(100);
    }
}


/* ===================== ACCELEROMETER ===================== */
void ACC_Init(void)
{
    ACC_WriteReg(LIS302DL_CTRL_REG1, 0x47);
}

void ACC_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    ACC_CS_LOW();
    HAL_SPI_Transmit(&hspi1, buf, 2, HAL_MAX_DELAY);
    ACC_CS_HIGH();
}

int8_t ACC_ReadAxis(uint8_t reg)
{
    uint8_t value;
    uint8_t address = reg | 0x80;

    ACC_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &value, 1, HAL_MAX_DELAY);
    ACC_CS_HIGH();

    return (int8_t)value;
}


/* ===================== BUZZER ===================== */
void buzzer_play(uint32_t freq)
{
    uint32_t timer_clock = 1000000;
    uint32_t period = (timer_clock / freq) - 1;

    __HAL_TIM_SET_AUTORELOAD(&htim3, period);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, period / 2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void buzzer_stop(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}


/* ===================== LCD ===================== */
void lcd_send_cmd(uint8_t cmd)
{
    uint8_t data_u = (cmd & 0xF0);
    uint8_t data_l = ((cmd << 4) & 0xF0);

    uint8_t data_arr[4] = {
        data_u | 0x0C,
        data_u | 0x08,
        data_l | 0x0C,
        data_l | 0x08
    };

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, 4, 100);
}

void lcd_send_data(uint8_t data)
{
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);

    uint8_t data_arr[4] = {
        data_u | 0x0D,
        data_u | 0x09,
        data_l | 0x0D,
        data_l | 0x09
    };

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, 4, 100);
}

void lcd_init(void)
{
    HAL_Delay(50);
    lcd_send_cmd(0x30);
    lcd_send_cmd(0x02);
    lcd_send_cmd(0x28);
    lcd_send_cmd(0x0C);
    lcd_send_cmd(0x06);
    lcd_send_cmd(0x01);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

void lcd_print(char *str)
{
    while (*str) lcd_send_data(*str++);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_send_cmd(address);
}


/* ===================== SERVO ===================== */
void servo_set_angle(uint8_t angle)
{
    if(angle > 180) angle = 180;

    uint32_t pulse = 1000 + ((uint32_t)angle * 1000) / 180;

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);
}


/* ===================== RFID RC522 ===================== */
void RFID_Init(void)
{
    RFID_Reset();
    HAL_Delay(50);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_RESET);
    HAL_Delay(50);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    /* Set default gain */
    RFID_WriteReg(RC522_RX_GAIN_REG, 0x70);

    /* Enable TX1 and TX2 */
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x03);

    /* Configure antenna driver and receiver */
    RFID_WriteReg(RC522_T_MODE_REG, 0x8D);
    RFID_WriteReg(RC522_T_RELOAD_H_REG, 0x3E);
    RFID_WriteReg(RC522_T_RELOAD_L_REG, 0x1E);

    /* Set modulation */
    RFID_WriteReg(RC522_MOD_WIDTH_REG, 0x13);

    /* Configure CRC */
    RFID_WriteReg(RC522_MODE_REG, 0x3D);
}

void RFID_Reset(void)
{
    RFID_RST_HIGH();
    HAL_Delay(1);
    RFID_RST_LOW();
    HAL_Delay(1);
    RFID_RST_HIGH();
}

void RFID_WriteReg(uint8_t addr, uint8_t val)
{
    uint8_t tx[2];
    tx[0] = (addr << 1) & 0x7E;
    tx[1] = val;

    RFID_CS_LOW();
    HAL_SPI_Transmit(&hspi2, tx, 2, 100);
    RFID_CS_HIGH();
}

uint8_t RFID_ReadReg(uint8_t addr)
{
    uint8_t tx[2], rx[2];
    tx[0] = ((addr << 1) & 0x7E) | 0x80;
    tx[1] = 0xFF;

    RFID_CS_LOW();
    HAL_SPI_TransmitReceive(&hspi2, tx, rx, 2, 100);
    RFID_CS_HIGH();

    return rx[1];
}

void RFID_SetBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp = RFID_ReadReg(reg);
    RFID_WriteReg(reg, tmp | mask);
}

void RFID_ClearBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp = RFID_ReadReg(reg);
    RFID_WriteReg(reg, tmp & (~mask));
}

uint8_t RFID_Request(uint8_t req_code, uint8_t *tag_type)
{
    uint8_t status = 2;
    uint8_t irq_en = 0x80;
    uint8_t wait_irq = 0x30;
    uint8_t last_bits;
    uint16_t n;

    RFID_WriteReg(RC522_DIV_IRQ_REG, irq_en << 7);
    RFID_ClearBitMask(RC522_STATUS1_REG, 0x80);
    RFID_SetBitMask(RC522_FIFO_LEVEL_REG, 0x80);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    /* Write command to FIFO */
    RFID_WriteReg(RC522_FIFO_DATA_REG, req_code);

    /* Send command */
    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_TRANSREC);
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x80);

    n = 0;
    while(n < 1000)
    {
        if(RFID_ReadReg(RC522_DIV_IRQ_REG) & wait_irq)
            break;
        n++;
        HAL_Delay(1);
    }

    RFID_ClearBitMask(RC522_TX_CONTROL_REG, 0x80);

    if(n == 1000)
        return 1;

    last_bits = RFID_ReadReg(RC522_RX_MODE_REG) & 0x07;

    if(RFID_ReadReg(RC522_STATUS2_REG) & 0x08)
    {
        status = 0;
        n = RFID_ReadReg(RC522_FIFO_LEVEL_REG);
        if(n != 2)
            status = 1;

        tag_type[0] = RFID_ReadReg(RC522_FIFO_DATA_REG);
        tag_type[1] = RFID_ReadReg(RC522_FIFO_DATA_REG);
    }

    return status;
}

uint8_t RFID_Anticoll(uint8_t *serial_num)
{
    uint8_t status = 2;
    uint8_t i, n;
    uint16_t recvlen = 0;

    RFID_WriteReg(RC522_DIV_IRQ_REG, 0x80);
    RFID_ClearBitMask(RC522_STATUS1_REG, 0x80);
    RFID_SetBitMask(RC522_FIFO_LEVEL_REG, 0x80);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    /* Anticoll command */
    RFID_WriteReg(RC522_FIFO_DATA_REG, PICC_SELECT_CL1);
    RFID_WriteReg(RC522_FIFO_DATA_REG, 0x20);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_TRANSREC);
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x80);

    n = 0;
    while(n < 1000)
    {
        if(RFID_ReadReg(RC522_DIV_IRQ_REG) & 0x10)
            break;
        n++;
        HAL_Delay(1);
    }

    RFID_ClearBitMask(RC522_TX_CONTROL_REG, 0x80);

    if(n == 1000)
        return 1;

    if(!(RFID_ReadReg(RC522_STATUS2_REG) & 0x08))
        return 1;

    recvlen = RFID_ReadReg(RC522_FIFO_LEVEL_REG);
    if(recvlen != 5)
        return 1;

    for(i = 0; i < 4; i++)
        serial_num[i] = RFID_ReadReg(RC522_FIFO_DATA_REG);

    status = 0;
    return status;
}

uint8_t RFID_Select(uint8_t *serial_num)
{
    uint8_t i;
    uint8_t status = 2;

    RFID_WriteReg(RC522_DIV_IRQ_REG, 0x80);
    RFID_ClearBitMask(RC522_STATUS1_REG, 0x80);
    RFID_SetBitMask(RC522_FIFO_LEVEL_REG, 0x80);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    RFID_WriteReg(RC522_FIFO_DATA_REG, PICC_SELECT_CL1);
    RFID_WriteReg(RC522_FIFO_DATA_REG, 0x70);

    for(i = 0; i < 4; i++)
        RFID_WriteReg(RC522_FIFO_DATA_REG, serial_num[i]);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_TRANSREC);
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x80);

    i = 0;
    while(i < 1000)
    {
        if(RFID_ReadReg(RC522_DIV_IRQ_REG) & 0x10)
            break;
        i++;
        HAL_Delay(1);
    }

    RFID_ClearBitMask(RC522_TX_CONTROL_REG, 0x80);

    if(i == 1000)
        return 1;

    if(RFID_ReadReg(RC522_STATUS2_REG) & 0x08)
    {
        status = 0;
    }

    return status;
}

uint8_t RFID_Auth(uint8_t auth_mode, uint8_t block_addr, uint8_t *key, uint8_t *serial_num)
{
    uint8_t i;

    RFID_WriteReg(RC522_DIV_IRQ_REG, 0x80);
    RFID_ClearBitMask(RC522_STATUS1_REG, 0x80);
    RFID_SetBitMask(RC522_FIFO_LEVEL_REG, 0x80);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    RFID_WriteReg(RC522_FIFO_DATA_REG, auth_mode);
    RFID_WriteReg(RC522_FIFO_DATA_REG, block_addr);

    for(i = 0; i < 6; i++)
        RFID_WriteReg(RC522_FIFO_DATA_REG, key[i]);

    for(i = 0; i < 4; i++)
        RFID_WriteReg(RC522_FIFO_DATA_REG, serial_num[i]);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_TRANSREC);
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x80);

    i = 0;
    while(i < 1000)
    {
        if(RFID_ReadReg(RC522_DIV_IRQ_REG) & 0x10)
            break;
        i++;
        HAL_Delay(1);
    }

    RFID_ClearBitMask(RC522_TX_CONTROL_REG, 0x80);

    if((RFID_ReadReg(RC522_STATUS2_REG) & 0x08) == 0)
        return 1;

    return 0;
}

uint8_t RFID_Read(uint8_t block_addr, uint8_t *recv_data)
{
    uint8_t i;
    uint16_t recvlen = 0;

    RFID_WriteReg(RC522_DIV_IRQ_REG, 0x80);
    RFID_ClearBitMask(RC522_STATUS1_REG, 0x80);
    RFID_SetBitMask(RC522_FIFO_LEVEL_REG, 0x80);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    RFID_WriteReg(RC522_FIFO_DATA_REG, PICC_READ);
    RFID_WriteReg(RC522_FIFO_DATA_REG, block_addr);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_TRANSREC);
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x80);

    i = 0;
    while(i < 1000)
    {
        if(RFID_ReadReg(RC522_DIV_IRQ_REG) & 0x10)
            break;
        i++;
        HAL_Delay(1);
    }

    RFID_ClearBitMask(RC522_TX_CONTROL_REG, 0x80);

    if(!(RFID_ReadReg(RC522_STATUS2_REG) & 0x08))
        return 1;

    recvlen = RFID_ReadReg(RC522_FIFO_LEVEL_REG);

    if(recvlen != 16)
        return 1;

    for(i = 0; i < 16; i++)
        recv_data[i] = RFID_ReadReg(RC522_FIFO_DATA_REG);

    return 0;
}

void RFID_Halt(void)
{
    uint8_t status_buf[2];

    RFID_WriteReg(RC522_DIV_IRQ_REG, 0x80);
    RFID_ClearBitMask(RC522_STATUS1_REG, 0x80);
    RFID_SetBitMask(RC522_FIFO_LEVEL_REG, 0x80);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_IDLE);

    RFID_WriteReg(RC522_FIFO_DATA_REG, PICC_HALT);
    RFID_WriteReg(RC522_FIFO_DATA_REG, 0);

    RFID_WriteReg(RC522_COMMAND_REG, RC522_COMMAND_TRANSREC);
    RFID_SetBitMask(RC522_TX_CONTROL_REG, 0x80);

    HAL_Delay(10);
    RFID_ClearBitMask(RC522_TX_CONTROL_REG, 0x80);
}


/* ===================== INTERRUPT HANDLER ===================== */
void EXTI9_5_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
        card_interrupt_flag = 1;
    }
}


/* ===================== PERIPHERAL INIT ===================== */
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* BUZZER PB0 - TIM3_CH3 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ACCEL CS PE3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    ACC_CS_HIGH();

    /* I2C LCD PB6 SCL, PB7 SDA */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SERVO PD14 - TIM4_CH3 */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* SPI1 PA5 (SCK), PA6 (MISO), PA7 (MOSI) */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* RFID CS PD8, RST PD9 */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    RFID_CS_HIGH();
    RFID_RST_HIGH();

    /* SPI2 PB13 (SCK), PB14 (MISO), PB15 (MOSI) */
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* RFID IRQ PC6 - EXTI */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void MX_SPI1_Init(void)
{
    __HAL_RCC_SPI1_CLK_ENABLE();

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    HAL_SPI_Init(&hspi1);
}

static void MX_SPI2_Init(void)
{
    __HAL_RCC_SPI2_CLK_ENABLE();

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    HAL_SPI_Init(&hspi2);
}

static void MX_TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 84 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
}

static void MX_TIM4_Init(void)
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 84 - 1;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 20000 - 1;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim4);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);
}

static void MX_I2C1_Init(void)
{
    __HAL_RCC_I2C1_CLK_ENABLE();

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&hi2c1);
}

static void MX_EXTI_Init(void)
{
    /* PC6 - EXTI9_5 for RFID IRQ (Falling Edge) */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
