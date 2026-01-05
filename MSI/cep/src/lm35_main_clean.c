/*
PIN CONFIGURATION MAP:
PC7→IR Sensor 
PC8→Exit Button
PC9→LED Light
PE6→LED OFF Button
PA15→TIM2_CH1(Buzzer)
PB0→TIM3_CH3(Servo)
PD15→TIM4_CH4(Fan)
PA5/6/7,PE3→SPI1(Accelerometer) 
PB6/7→I2C1(LCD)
PB13/14/15,PD8/9,PC6→SPI2(RFID)
PA0→ADC1_IN0(LM35)
*/

#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Peripheral Handles */
SPI_HandleTypeDef hspi1, hspi2;
TIM_HandleTypeDef htim2, htim3, htim4;
I2C_HandleTypeDef hi2c1;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC Data */
volatile uint32_t adc_value = 0;
static uint32_t adc_avg = 0;

/* LCD Config */
#define LCD_ADDR (0x27 << 1)

/* Buzzer */
#define NOTE_E4 330

/* MFRC522 Registers */
#define CommandReg     0x01
#define ComIrqReg      0x04
#define FIFODataReg    0x09
#define FIFOLevelReg   0x0A
#define ControlReg     0x0C
#define BitFramingReg  0x0D
#define ErrorReg       0x06
#define ModeReg        0x11
#define TxASKReg       0x15
#define TxControlReg   0x14
#define VersionReg     0x37

#define PCD_IDLE       0x00
#define PCD_TRANSCEIVE 0x0C
#define PCD_RESETPHASE 0x0F

#define PICC_REQIDL    0x26
#define PICC_ANTICOLL  0x93

/* Accelerometer Registers */
#define LIS302DL_CTRL_REG1 0x20
#define LIS302DL_OUT_X     0x29
#define LIS302DL_OUT_Y     0x2B
#define LIS302DL_OUT_Z     0x2D

/* Chip Select Macros */
#define ACC_CS_LOW()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define ACC_CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define RFID_CS_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)
#define RFID_CS_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)
#define RFID_RST_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define RFID_RST_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)

/* GPIO Macros */
#define IR_SENSOR_PIN       GPIO_PIN_7
#define EXIT_BUTTON_PIN     GPIO_PIN_8
#define LED_LIGHT_PIN       GPIO_PIN_9
#define LED_OFF_BUTTON_PIN  GPIO_PIN_6

#define ir_sensor_read()       HAL_GPIO_ReadPin(GPIOC, IR_SENSOR_PIN)
#define exit_button_read()     (HAL_GPIO_ReadPin(GPIOC, EXIT_BUTTON_PIN) == GPIO_PIN_RESET)
#define led_off_button_read()  (HAL_GPIO_ReadPin(GPIOE, LED_OFF_BUTTON_PIN) == GPIO_PIN_RESET)
#define light_on()             HAL_GPIO_WritePin(GPIOC, LED_LIGHT_PIN, GPIO_PIN_SET)
#define light_off()            HAL_GPIO_WritePin(GPIOC, LED_LIGHT_PIN, GPIO_PIN_RESET)

/* Data Structures */
typedef struct {
    int8_t x, y, z;
    uint16_t motion_count;
    uint32_t last_motion_time;
    int8_t baseline_x, baseline_y, baseline_z;
    uint8_t calibrated;
} MotionData;

typedef struct {
    uint8_t uid[10];
    uint8_t uid_len;
    uint8_t card_detected;
    uint32_t last_detect_time;
} RFIDData;

typedef struct {
    uint16_t duration_ms;
    uint32_t start_time;
    uint8_t running;
} FanTimer;

MotionData motion = {0, 0, 0, 0, 0};
RFIDData rfid_data = {0};
FanTimer fan_timer = {0, 0, 0};
volatile uint8_t card_interrupt_flag = 0;

/* Function Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_DMA_Init(void);
static void MX_EXTI_Init(void);

void ACC_Init(void);
void ACC_WriteReg(uint8_t reg, uint8_t data);
int8_t ACC_ReadAxis(uint8_t reg);

void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_clear(void);
void lcd_print(char *str);
void lcd_set_cursor(uint8_t row, uint8_t col);

void buzzer_play(uint32_t freq);
void buzzer_stop(void);

void servo_set_angle(uint8_t angle);

void fan_start(void);
void fan_set_speed(uint16_t duty_cycle);
void fan_stop(void);
void fan_set_duration(uint16_t duration_ms, uint16_t speed);
void fan_update_timer(void);

float LM35_GetTemperature(void);
void LM35_StartConversion(void);
uint32_t LM35_ReadADC(void);

void MFRC522_Init(void);
uint8_t MFRC522_ReadReg(uint8_t addr);
void MFRC522_WriteReg(uint8_t addr, uint8_t val);
uint8_t MFRC522_Request(uint8_t *tagType);
uint8_t MFRC522_Anticoll(uint8_t *uid);
void RFID_Init(void);

/* Smart Room Automation Main */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_I2C1_Init();
    MX_EXTI_Init();

    RFID_Init();
    ACC_Init();
    lcd_init();
    LM35_StartConversion();

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Smart Room");
    lcd_set_cursor(1, 0);
    lcd_print("System Ready");
    HAL_Delay(2000);
    
    /* Calibrate accelerometer baseline */
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Calibrating...");
    HAL_Delay(500);
    motion.baseline_x = ACC_ReadAxis(LIS302DL_OUT_X);
    motion.baseline_y = ACC_ReadAxis(LIS302DL_OUT_Y);
    motion.baseline_z = ACC_ReadAxis(LIS302DL_OUT_Z);
    motion.calibrated = 1;
    lcd_set_cursor(1, 0);
    lcd_print("Ready!");
    HAL_Delay(1000);

    servo_set_angle(0); /* Lock door */
    
    uint8_t room_occupied = 0;
    uint8_t tagType[2];
    uint8_t uid[10];
    uint8_t light_manual_control = 0;
    char temp_str[16];
    char display_buf[32];
    uint8_t lcd_state = 0;
    uint32_t last_lcd_update = 0;
    
    while(1)
    {
        /* STATE 1: WAITING FOR ENTRY */
        if (!room_occupied) {
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("Scan RFID Card");
            lcd_set_cursor(1, 0);
            lcd_print("To Enter...");
            
            if (MFRC522_Request(tagType) == 1) {
                if (MFRC522_Anticoll(uid) == 1) {
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_print("Access Granted!");
                    
                    buzzer_play(NOTE_E4);
                    HAL_Delay(200);
                    buzzer_stop();
                    
                    servo_set_angle(90); /* Unlock */
                    HAL_Delay(1000);
                    
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_print("Door Open");
                    lcd_set_cursor(1, 0);
                    lcd_print("Enter Now...");
                    
                    uint32_t wait_start = HAL_GetTick();
                    uint8_t person_entered = 0;
                    
                    while ((HAL_GetTick() - wait_start) < 10000) {
                        if (ir_sensor_read() == GPIO_PIN_SET) {
                            HAL_Delay(2000);
                            while (ir_sensor_read() == GPIO_PIN_SET) {
                                HAL_Delay(50);
                            }
                            person_entered = 1;
                            break;
                        }
                        HAL_Delay(50);
                    }
                    
                    HAL_Delay(3000);
                    servo_set_angle(0); /* Lock */
                    
                    if (person_entered) {
                        room_occupied = 1;
                        light_on();
                        light_manual_control = 0;
                        
                        lcd_clear();
                        lcd_set_cursor(0, 0);
                        lcd_print("Welcome!");
                        HAL_Delay(1500);
                    } else {
                        lcd_clear();
                        lcd_set_cursor(0, 0);
                        lcd_print("Entry Timeout");
                        HAL_Delay(2000);
                    }
                }
            }
            
            HAL_Delay(100);
        }
        
        /* STATE 2: ROOM OCCUPIED - MONITORING */
        if (room_occupied) {
            float temp = LM35_GetTemperature();
            
            /* Fan control */
            if (temp > 28.0) {
                if (!fan_timer.running) {
                    fan_start();
                    fan_set_speed(15000); /* 75% speed */
                    fan_timer.running = 1;
                }
            } else {
                if (fan_timer.running) {
                    fan_set_speed(0);
                    HAL_Delay(50);
                    fan_stop();
                }
            }
            
            /* Manual light control */
            if (led_off_button_read()) {
                light_off();
                light_manual_control = 1;
                HAL_Delay(300);
            }
            
            /* Motion detection */
            motion.x = ACC_ReadAxis(LIS302DL_OUT_X);
            motion.y = ACC_ReadAxis(LIS302DL_OUT_Y);
            motion.z = ACC_ReadAxis(LIS302DL_OUT_Z);
            
            int16_t delta_x = abs(motion.x - motion.baseline_x);
            int16_t delta_y = abs(motion.y - motion.baseline_y);
            int16_t delta_z = abs(motion.z - motion.baseline_z);
            int16_t total_delta = delta_x + delta_y + delta_z;
            
            /* Minor fluctuation (20-60) */
            if (total_delta > 20 && total_delta < 60) {
                uint32_t current_time = HAL_GetTick();
                if (current_time - motion.last_motion_time > 3000) {
                    for (int i = 0; i < 3; i++) {
                        buzzer_play(NOTE_E4);
                        HAL_Delay(80);
                        buzzer_stop();
                        HAL_Delay(80);
                    }
                    motion.last_motion_time = current_time;
                }
            }
            
            /* Earthquake (>60) */
            if (total_delta > 60) {
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("EARTHQUAKE!");
                lcd_set_cursor(1, 0);
                lcd_print("ALERT!");
                
                for (int i = 0; i < 5; i++) {
                    buzzer_play(NOTE_E4);
                    HAL_Delay(200);
                    buzzer_stop();
                    HAL_Delay(100);
                }
                
                HAL_Delay(1000);
            }
            
            /* Sliding LCD Display (3 states, 2 sec each) */
            uint32_t current_time = HAL_GetTick();
            if (current_time - last_lcd_update > 2000) {
                lcd_state = (lcd_state + 1) % 3;
                last_lcd_update = current_time;
                
                lcd_clear();
                
                if (lcd_state == 0) {
                    /* Temperature & Fan */
                    lcd_set_cursor(0, 0);
                    lcd_print("Temp: ");
                    // Convert float to integer-based formatting (no float support needed)
                    int temp_int = (int)temp;
                    int temp_dec = (int)((temp - temp_int) * 10);
                    sprintf(display_buf, "%d.%dC", temp_int, temp_dec);
                    lcd_print(display_buf);
                    
                    lcd_set_cursor(1, 0);
                    if (fan_timer.running) {
                        lcd_print("Fan: ON (75%)");
                    } else {
                        lcd_print("Fan: OFF");
                    }
                    
                } else if (lcd_state == 1) {
                    /* Motion Status */
                    lcd_set_cursor(0, 0);
                    lcd_print("Motion Delta:");
                    
                    lcd_set_cursor(1, 0);
                    sprintf(display_buf, "%d ", total_delta);
                    lcd_print(display_buf);
                    if (total_delta > 60) {
                        lcd_print("QUAKE!");
                    } else if (total_delta > 20) {
                        lcd_print("Alert");
                    } else {
                        lcd_print("Normal");
                    }
                    
                } else if (lcd_state == 2) {
                    /* Light & System Status */
                    lcd_set_cursor(0, 0);
                    lcd_print("Light: ");
                    if (HAL_GPIO_ReadPin(GPIOC, LED_LIGHT_PIN) == GPIO_PIN_SET) {
                        lcd_print("ON");
                    } else {
                        lcd_print("OFF");
                    }
                    
                    lcd_set_cursor(1, 0);
                    lcd_print("Room: Active");
                }
            }
            
            /* Exit button */
            if (exit_button_read()) {
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("Exiting...");
                lcd_set_cursor(1, 0);
                lcd_print("Please Wait");
                
                servo_set_angle(90);
                HAL_Delay(3000);
                
                light_off();
                if (fan_timer.running) {
                    fan_set_speed(0);
                    HAL_Delay(100);
                    fan_stop();
                }
                
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("Door Open");
                lcd_set_cursor(1, 0);
                lcd_print("Exit Now...");
                
                uint32_t exit_wait = HAL_GetTick();
                while ((HAL_GetTick() - exit_wait) < 10000) {
                    if (ir_sensor_read() == GPIO_PIN_SET) {
                        HAL_Delay(2000);
                        while (ir_sensor_read() == GPIO_PIN_SET) {
                            HAL_Delay(50);
                        }
                        break;
                    }
                    HAL_Delay(50);
                }
                
                HAL_Delay(3000);
                servo_set_angle(0);
                
                room_occupied = 0;
                
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("Goodbye!");
                lcd_set_cursor(1, 0);
                lcd_print("Room Secured");
                HAL_Delay(2000);
            }
        }
        
        HAL_Delay(500);
    }
}

/* LM35 Temperature Functions */
void LM35_StartConversion(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_value, 1);
}

uint32_t LM35_ReadADC(void)
{
    return adc_value;
}

float LM35_GetTemperature(void)
{
    uint32_t raw_adc = LM35_ReadADC();
    adc_avg = (adc_avg * 31 + raw_adc) / 32; /* Exponential filter */
    float voltage = (adc_avg * 3.3f) / 4095.0f;
    float temperature = voltage * 100.0f; /* LM35: 10mV/°C */
    return temperature;
}

/* Accelerometer Functions */
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

/* Buzzer Functions */
void buzzer_play(uint32_t freq)
{
    uint32_t timer_clock = 1000000;
    uint32_t period = (timer_clock / freq) - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim2, period);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, period / 2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void buzzer_stop(void)
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

/* LCD Functions */
void lcd_send_cmd(uint8_t cmd)
{
    uint8_t data_u = (cmd & 0xF0);
    uint8_t data_l = ((cmd << 4) & 0xF0);
    uint8_t data_arr[4] = {data_u | 0x0C, data_u | 0x08, data_l | 0x0C, data_l | 0x08};
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, 4, 100);
}

void lcd_send_data(uint8_t data)
{
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);
    uint8_t data_arr[4] = {data_u | 0x0D, data_u | 0x09, data_l | 0x0D, data_l | 0x09};
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
    HAL_Delay(2);
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

/* Servo Functions */
void servo_set_angle(uint8_t angle)
{
    if(angle > 180) angle = 180;
    uint32_t pulse = 1000 + ((uint32_t)angle * 1000) / 180;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse);
}

/* Fan Control Functions */
void fan_start(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void fan_set_speed(uint16_t duty_cycle)
{
    if (duty_cycle > 20000) duty_cycle = 20000;
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, duty_cycle);
}

void fan_stop(void)
{
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
    fan_timer.running = 0;
}

void fan_set_duration(uint16_t duration_ms, uint16_t speed)
{
    fan_start();
    fan_set_speed(speed);
    fan_timer.duration_ms = duration_ms;
    fan_timer.start_time = HAL_GetTick();
    fan_timer.running = 1;
}

void fan_update_timer(void)
{
    if (!fan_timer.running) return;
    uint32_t elapsed = HAL_GetTick() - fan_timer.start_time;
    if (elapsed >= fan_timer.duration_ms) {
        fan_stop();
    }
}

/* MFRC522 Functions */
void MFRC522_WriteReg(uint8_t addr, uint8_t val)
{
    uint8_t data[2];
    data[0] = (addr << 1) & 0x7E;
    data[1] = val;
    RFID_CS_LOW();
    HAL_SPI_Transmit(&hspi2, data, 2, HAL_MAX_DELAY);
    RFID_CS_HIGH();
}

uint8_t MFRC522_ReadReg(uint8_t addr)
{
    uint8_t tx = ((addr << 1) & 0x7E) | 0x80;
    uint8_t rx;
    RFID_CS_LOW();
    HAL_SPI_Transmit(&hspi2, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, &rx, 1, HAL_MAX_DELAY);
    RFID_CS_HIGH();
    return rx;
}

void MFRC522_Init(void)
{
    RFID_RST_HIGH();
    HAL_Delay(50);
    MFRC522_WriteReg(CommandReg, PCD_RESETPHASE);
    MFRC522_WriteReg(TxASKReg, 0x40);
    MFRC522_WriteReg(ModeReg, 0x3D);
    MFRC522_WriteReg(TxControlReg, 0x83);
}

uint8_t MFRC522_Request(uint8_t *tagType)
{
    MFRC522_WriteReg(CommandReg, PCD_IDLE);
    MFRC522_WriteReg(ComIrqReg, 0x7F);
    MFRC522_WriteReg(FIFOLevelReg, 0x80);
    MFRC522_WriteReg(BitFramingReg, 0x07);
    MFRC522_WriteReg(FIFODataReg, PICC_REQIDL);
    MFRC522_WriteReg(CommandReg, PCD_TRANSCEIVE);
    MFRC522_WriteReg(BitFramingReg, 0x87);

    uint16_t n = 0;
    while (n < 1000) {
        uint8_t irq = MFRC522_ReadReg(ComIrqReg);
        if (irq & 0x30) break;
        n++;
        HAL_Delay(1);
    }

    MFRC522_WriteReg(BitFramingReg, 0x07);
    if (n == 1000) return 0;

    uint8_t err = MFRC522_ReadReg(ErrorReg);
    if (err & 0x1B) return 0;

    uint8_t fifo_len = MFRC522_ReadReg(FIFOLevelReg);
    if (fifo_len < 2) return 0;

    tagType[0] = MFRC522_ReadReg(FIFODataReg);
    tagType[1] = MFRC522_ReadReg(FIFODataReg);
    return 1;
}

uint8_t MFRC522_Anticoll(uint8_t *uid)
{
    MFRC522_WriteReg(CommandReg, PCD_IDLE);
    MFRC522_WriteReg(ComIrqReg, 0x7F);
    MFRC522_WriteReg(FIFOLevelReg, 0x80);
    MFRC522_WriteReg(BitFramingReg, 0x00);
    MFRC522_WriteReg(FIFODataReg, PICC_ANTICOLL);
    MFRC522_WriteReg(FIFODataReg, 0x20);
    MFRC522_WriteReg(CommandReg, PCD_TRANSCEIVE);
    MFRC522_WriteReg(BitFramingReg, 0x80);

    uint16_t n = 0;
    while (n < 1000) {
        uint8_t irq = MFRC522_ReadReg(ComIrqReg);
        if (irq & 0x30) break;
        n++;
        HAL_Delay(1);
    }

    MFRC522_WriteReg(BitFramingReg, 0x00);
    if (n == 1000) return 0;

    uint8_t err = MFRC522_ReadReg(ErrorReg);
    if (err & 0x1B) return 0;

    uint8_t rx_last_bits = MFRC522_ReadReg(ControlReg) & 0x07;
    if (rx_last_bits != 0) return 0;

    uint8_t fifo_len = MFRC522_ReadReg(FIFOLevelReg);
    if (fifo_len < 5) return 0;

    for (int i = 0; i < 5; i++)
        uid[i] = MFRC522_ReadReg(FIFODataReg);

    return 1;
}

void RFID_Init(void)
{
    MFRC522_Init();
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* PA0: ADC1_IN0 (LM35) */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA15: TIM2_CH1 (Buzzer) */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PB0: TIM3_CH3 (Servo) */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PE3: Accelerometer CS */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    ACC_CS_HIGH();

    /* PE6: LED OFF Button */
    GPIO_InitStruct.Pin = LED_OFF_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* PB6/7: I2C1 (LCD) */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PD15: TIM4_CH4 (Fan) */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* PA5/6/7: SPI1 (Accelerometer) */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PD8/9: RFID CS, RST */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    RFID_CS_HIGH();
    RFID_RST_HIGH();

    /* PB13/14/15: SPI2 (RFID) */
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PC6: RFID IRQ */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC7: IR Sensor */
    GPIO_InitStruct.Pin = IR_SENSOR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC8: Exit Button */
    GPIO_InitStruct.Pin = EXIT_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC9: LED Light */
    GPIO_InitStruct.Pin = LED_LIGHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    light_off();
}

/* SPI1 Init (Accelerometer) - 5.25MHz */
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

/* SPI2 Init (RFID) - 656kHz */
static void MX_SPI2_Init(void)
{
    __HAL_RCC_SPI2_CLK_ENABLE();
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    HAL_SPI_Init(&hspi2);
}

/* TIM2 Init (Buzzer) - 1MHz timer */
static void MX_TIM2_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    TIM_OC_InitTypeDef sConfigOC = {0};
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 84 - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1000 - 1;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim2);
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/* TIM3 Init (Servo) - 50Hz, 1-2ms pulse */
static void MX_TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();
    TIM_OC_InitTypeDef sConfigOC = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 84 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 20000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

/* TIM4 Init (Fan) - 50Hz, 0-100% duty */
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
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

/* I2C1 Init (LCD) - 100kHz */
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

/* DMA Init for ADC */
static void MX_DMA_Init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adc1);
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
}

/* ADC1 Init (LM35) - 12bit, DMA, 21MHz */
static void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    HAL_ADC_Init(&hadc1);
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

void Error_Handler(void)
{
    __disable_irq();
    while(1) {}
}

/* EXTI Init */
static void MX_EXTI_Init(void)
{
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/* System Clock: HSI 16MHz → PLL 168MHz */
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
