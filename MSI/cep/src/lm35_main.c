/*
================================================================================
 STM32F407 LM35 Temperature Sensor + ADC + I2C LCD (16x2) Display
================================================================================

PIN CONFIGURATION:
==================
BUZZER:
  - PB0 -> TIM3_CH3 (PWM Output)

ACCELEROMETER LIS302DL (SPI1):
  - PA5 -> SCK
  - PA6 -> MISO
  - PA7 -> MOSI
  - PE3 -> CS

LCD 16x2 (I2C PCF8574):
  - PB6 -> I2C1 SCL
  - PB7 -> I2C1 SDA
  - Address = 0x27

SERVO:
  - PD14 -> TIM4_CH3

RFID (RC522) - SPI2:
  - PB13 -> SCK
  - PB14 -> MISO
  - PB15 -> MOSI

  - PD8  -> CS
  - PD9  -> RST
  
  - PC6  -> IRQ (EXTI9_5, Falling Edge)

LM35 TEMPERATURE SENSOR:
  - PA0 -> ADC1_IN0 (Analog Input)
  - Vcc -> 3.3V
  - GND -> GND
  - OUT -> PA0

ADC CONFIGURATION:
  - ADC1 Channel 0 (PA0)
  - 12-bit resolution
  - Continuous mode
  - Regular conversion
  - DMA enabled for continuous reading
  - Sample rate: 84 MHz / Prescaler / (cycles)

TEMPERATURE CONVERSION FORMULA:
  - LM35 Output: 10 mV per °C
  - ADC Reference: 3.3V
  - ADC Resolution: 12-bit (0-4095)
  - Temperature (°C) = (ADC_Value * 3.3 / 4095) * 100
  - Simplified: Temperature = (ADC_Value * 330) / 4095

================================================================================
*/

#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ======================== PERIPHERAL HANDLES ======================== */
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim3;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim4;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ======================== ADC DATA BUFFER ======================== */
volatile uint32_t adc_value = 0;  /* Stores raw ADC value */
static uint32_t adc_avg = 0;      /* Moving average for filtering */

/* ======================== LCD CONFIGURATION ======================== */
#define LCD_ADDR (0x27 << 1)  /* PCF8574 Address with 7-bit left shift */

/* ======================== BUZZER FREQUENCY ======================== */
#define NOTE_E4 330

/* ======================== MFRC522 REGISTERS/COMMANDS ======================== */
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

/* ======================== ACCELEROMETER REGISTERS ======================== */
#define LIS302DL_CTRL_REG1 0x20
#define LIS302DL_OUT_X     0x29
#define LIS302DL_OUT_Y     0x2B
#define LIS302DL_OUT_Z     0x2D

/* ======================== CHIP SELECT MACROS ======================== */
#define ACC_CS_LOW()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define ACC_CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

#define RFID_CS_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)
#define RFID_CS_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)

#define RFID_RST_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define RFID_RST_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)

/* ======================== DATA STRUCTURES ======================== */
typedef struct {
    int8_t x, y, z;
    uint16_t motion_count;
    uint32_t last_motion_time;
} MotionData;

typedef struct {
    uint8_t uid[10];
    uint8_t uid_len;
    uint8_t card_detected;
    uint32_t last_detect_time;
} RFIDData;

MotionData motion = {0, 0, 0, 0, 0};
RFIDData rfid_data = {0};
volatile uint8_t card_interrupt_flag = 0;

/* ======================== FUNCTION PROTOTYPES ======================== */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_DMA_Init(void);
static void MX_EXTI_Init(void);

/* Accelerometer Functions */
void ACC_Init(void);
void ACC_WriteReg(uint8_t reg, uint8_t data);
int8_t ACC_ReadAxis(uint8_t reg);

/* LCD Functions */
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_clear(void);
void lcd_print(char *str);
void lcd_set_cursor(uint8_t row, uint8_t col);

/* Buzzer Functions */
void buzzer_play(uint32_t freq);
void buzzer_stop(void);

/* Servo Functions */
void servo_set_angle(uint8_t angle);

/* LM35 Temperature Functions */
float LM35_GetTemperature(void);
void LM35_StartConversion(void);
uint32_t LM35_ReadADC(void);

/* MFRC522 Functions (inlined) */
void MFRC522_Init(void);
uint8_t MFRC522_ReadReg(uint8_t addr);
void MFRC522_WriteReg(uint8_t addr, uint8_t val);
uint8_t MFRC522_Request(uint8_t *tagType);
uint8_t MFRC522_Anticoll(uint8_t *uid);
/* Compatibility alias */
void RFID_Init(void);

/* Interrupt */
void EXTI9_5_IRQHandler(void);


/* ======================== MAIN FUNCTION ======================== */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Initialize all peripherals in order */
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_TIM3_Init();
    MX_I2C1_Init();
    MX_EXTI_Init();

    /* Initialize devices */
    RFID_Init();
    lcd_init();

    /* LCD Welcome Screen */
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("STM32F407");
    lcd_set_cursor(1, 0);
    lcd_print("RFID READER");
    HAL_Delay(2000);

    /* ---- RC522: Read and show silicon version ---- */
    uint8_t rc522_ver = MFRC522_ReadReg(VersionReg);
    uint8_t rfid_ok = (rc522_ver != 0xFF && rc522_ver != 0x00);
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("RC522 Ver:");
    char ver_str[16];
    sprintf(ver_str, "0x%02X", rc522_ver);
    lcd_set_cursor(1, 0);
    lcd_print(ver_str);
    HAL_Delay(1500);

    /* ======================== MAIN LOOP ======================== */
    uint8_t last_uid[4] = {0};
    uint8_t last_uid_len = 0;

    while(1)
    {
        uint8_t tag_type[2] = {0};
        uint8_t uid[5] = {0};

        if (rfid_ok && MFRC522_Request(tag_type)) {
            if (MFRC522_Anticoll(uid)) {
                uint8_t uid_len = 4; /* First 4 bytes are UID */

                /* Beep only when new UID detected */
                uint8_t changed = (uid_len != last_uid_len);
                if (!changed) {
                    for (int i = 0; i < uid_len; i++) {
                        if (uid[i] != last_uid[i]) { changed = 1; break; }
                    }
                }
                if (changed) {
                    buzzer_play(NOTE_E4);
                    HAL_Delay(100);
                    buzzer_stop();
                    last_uid_len = uid_len;
                    for (int i = 0; i < uid_len; i++) last_uid[i] = uid[i];
                }

                /* Display UID */
                lcd_clear();
                lcd_set_cursor(0, 0);
                char id_str[16] = {0};
                sprintf(id_str, "ID:%02X%02X%02X%02X", uid[0], uid[1], uid[2], uid[3]);
                lcd_print(id_str);
                lcd_set_cursor(1, 0);
                lcd_print("Card detected");
                HAL_Delay(100);
            }
        } else {
            /* No card present */
            lcd_clear();
            lcd_set_cursor(0, 0);
            if (!rfid_ok) {
                lcd_print("RFID: NOT FOUND");
            } else {
                lcd_print("Waiting for card");
            }
            last_uid_len = 0;
            HAL_Delay(100);
        }
    }
}


/* ======================== LM35 TEMPERATURE FUNCTIONS ======================== */

/**
 * @brief Start ADC continuous conversion with DMA
 * @note Call this once during initialization
 */
void LM35_StartConversion(void)
{
    /* Start ADC1 conversion with DMA */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_value, 1);
}

/**
 * @brief Read raw ADC value
 * @return Raw ADC value (0-4095 for 12-bit)
 */
uint32_t LM35_ReadADC(void)
{
    return adc_value;
}

/**
 * @brief Convert ADC value to temperature in Celsius
 * @return Temperature in °C
 * 
 * FORMULA:
 *   Voltage = ADC_Value * (Vref / Resolution)
 *   Voltage = ADC_Value * (3.3 / 4095)
 *   Temperature = Voltage / 0.01  (LM35: 10mV per °C)
 *   Temperature = (ADC_Value * 3.3 / 4095) / 0.01
 *   Temperature = (ADC_Value * 330) / 4095
 */
float LM35_GetTemperature(void)
{
    /* Get current ADC value from DMA buffer */
    uint32_t raw_adc = LM35_ReadADC();

    /* Smooth exponential filter: 31/32 (slower response ~30ms, very smooth) */
    adc_avg = (adc_avg * 31 + raw_adc) / 32;

    /* Convert ADC value to voltage (0-3.3V) */
    float voltage = (adc_avg * 3.3f) / 4095.0f;

    /* Convert voltage to temperature (LM35: 10mV per °C) */
    float temperature = voltage * 100.0f;

    return temperature;
}


/* ======================== ACCELEROMETER FUNCTIONS ======================== */
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


/* ======================== BUZZER FUNCTIONS ======================== */
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


/* ======================== LCD FUNCTIONS ======================== */
void lcd_send_cmd(uint8_t cmd)
{
    uint8_t data_u = (cmd & 0xF0);
    uint8_t data_l = ((cmd << 4) & 0xF0);

    uint8_t data_arr[4] = {
        data_u | 0x0C,  /* EN=1, RS=0 (Command), Backlight=1 */
        data_u | 0x08,  /* EN=0 */
        data_l | 0x0C,  /* EN=1 */
        data_l | 0x08   /* EN=0 */
    };

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, 4, 100);
}

void lcd_send_data(uint8_t data)
{
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);

    uint8_t data_arr[4] = {
        data_u | 0x0D,  /* EN=1, RS=1 (Data), Backlight=1 */
        data_u | 0x09,  /* EN=0 */
        data_l | 0x0D,  /* EN=1 */
        data_l | 0x09   /* EN=0 */
    };

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, 4, 100);
}

void lcd_init(void)
{
    HAL_Delay(50);
    lcd_send_cmd(0x30);  /* 8-bit mode */
    lcd_send_cmd(0x02);  /* Return home */
    lcd_send_cmd(0x28);  /* 4-bit mode, 2 lines, 5x8 font */
    lcd_send_cmd(0x0C);  /* Display ON, cursor OFF, blink OFF */
    lcd_send_cmd(0x06);  /* Increment mode, no shift */
    lcd_send_cmd(0x01);  /* Clear display */
    HAL_Delay(2);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);  /* Clear display command */
    HAL_Delay(2);
}

void lcd_print(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_send_cmd(address);
}


/* ======================== SERVO FUNCTIONS ======================== */
void servo_set_angle(uint8_t angle)
{
    if(angle > 180) angle = 180;
    uint32_t pulse = 1000 + ((uint32_t)angle * 1000) / 180;
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);
}


/* ======================== MFRC522 FUNCTIONS (INLINED) ======================== */
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
    /* Clear interrupts and FIFO */
    MFRC522_WriteReg(CommandReg, PCD_IDLE);
    MFRC522_WriteReg(ComIrqReg, 0x7F);
    MFRC522_WriteReg(FIFOLevelReg, 0x80);

    /* REQA is 7-bit, set TxLastBits=7 and StartSend bit */
    MFRC522_WriteReg(BitFramingReg, 0x07);
    MFRC522_WriteReg(FIFODataReg, PICC_REQIDL);
    MFRC522_WriteReg(CommandReg, PCD_TRANSCEIVE);
    MFRC522_WriteReg(BitFramingReg, 0x87);

    /* Wait for the IRQ or timeout */
    uint16_t n = 0;
    while (n < 1000) {
        uint8_t irq = MFRC522_ReadReg(ComIrqReg);
        if (irq & 0x30) break;
        n++;
        HAL_Delay(1);
    }

    /* Stop sending */
    MFRC522_WriteReg(BitFramingReg, 0x07);

    if (n == 1000) return 0;

    /* Check errors */
    uint8_t err = MFRC522_ReadReg(ErrorReg);
    if (err & 0x1B) return 0;

    /* Ensure we got ATQA (2 bytes) */
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

/* Compatibility: keep old name */
void RFID_Init(void)
{
    MFRC522_Init();
}


/* ======================== INTERRUPT HANDLERS ======================== */
/* EXTI9_5_IRQHandler is defined in stm32f4xx_it.c - do not define here */


/* ======================== PERIPHERAL INITIALIZATION ======================== */

/**
 * @brief GPIO Initialization
 * PA0  -> ADC1_IN0 (Analog Input for LM35)
 * PA5, PA6, PA7 -> SPI1 (Accelerometer)
 * PE3  -> ACC CS
 * PB0  -> TIM3_CH3 (Buzzer)
 * PB6, PB7 -> I2C1 (LCD)
 * PB13, PB14, PB15 -> SPI2 (RFID)
 * PD8, PD9 -> RFID CS, RST
 * PD14 -> TIM4_CH3 (Servo)
 * PC6  -> EXTI (RFID IRQ)
 */
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* ---- PA0: ADC1_IN0 (Analog Input - LM35) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ---- PB0: TIM3_CH3 (Buzzer PWM) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ---- PE3: Accelerometer CS ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    ACC_CS_HIGH();

    /* ---- PB6, PB7: I2C1 (LCD) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ---- PD14: TIM4_CH3 (Servo) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* ---- PA5, PA6, PA7: SPI1 (Accelerometer) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ---- PD8, PD9: RFID CS, RST ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    RFID_CS_HIGH();
    RFID_RST_HIGH();

    /* ---- PB13, PB14, PB15: SPI2 (RFID) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ---- PC6: RFID IRQ (EXTI) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * @brief SPI1 Initialization (Accelerometer LIS302DL)
 * Clock: APB2 = 84 MHz
 * Prescaler: 16 -> ~5.25 MHz
 */
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

/**
 * @brief SPI2 Initialization (RFID RC522)
 * Clock: APB1 = 42 MHz
 * Prescaler: 8 -> ~5.25 MHz
 */
static void MX_SPI2_Init(void)
{
    __HAL_RCC_SPI2_CLK_ENABLE();

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    /* Try SPI mode 1 (CPHA = 2 Edge) for RC522 alignment */
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    /* Use a slower SPI clock to improve initial link reliability */
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    HAL_SPI_Init(&hspi2);
}

/**
 * @brief TIM3 Initialization (Buzzer PWM)
 * Prescaler: 84-1 (1 MHz timer clock)
 * Period: 1000-1 (1 kHz)
 * Channel 3
 */
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

/**
 * @brief TIM4 Initialization (Servo PWM)
 * Prescaler: 84-1 (1 MHz timer clock)
 * Period: 20000-1 (50 Hz - 20ms servo period)
 * Channel 3
 */
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

/**
 * @brief I2C1 Initialization (LCD Display)
 * Speed: 100 kHz (Standard mode)
 * Address: 7-bit addressing
 */
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

/**
 * @brief DMA Initialization for ADC1
 * DMA2 Stream 0, Channel 0
 * Peripheral -> Memory (ADC1 -> adc_value)
 * Continuous mode
 */
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
    hdma_adc1.Init.Mode = DMA_CIRCULAR;  /* Continuous circular buffer */
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    HAL_DMA_Init(&hdma_adc1);
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
}

/**
 * @brief ADC1 Initialization (LM35 Temperature Sensor)
 * 
 * ADC CONFIGURATION:
 * ==================
 * Channel: ADC1_IN0 (PA0)
 * Resolution: 12-bit
 * Data Alignment: Right aligned
 * Scan Mode: Disabled (single channel)
 * Continuous Mode: Enabled
 * DMA: Enabled with circular buffer
 * Sampling Time: 112 cycles (max accuracy)
 * Clock Prescaler: 4 (APB2 84MHz / 4 = 21 MHz)
 * 
 * CONVERSION TIME CALCULATION:
 * ============================
 * Total cycles = Sampling cycles + 12 (conversion)
 * Total cycles = 112 + 12 = 124 cycles
 * Sample time = 124 / 21MHz ≈ 5.9 µs
 * Conversion rate ≈ 169 kHz
 */
static void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;  /* 84MHz / 4 = 21MHz */
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;             /* 12-bit resolution */
    hadc1.Init.ScanConvMode = DISABLE;                      /* Single channel */
    hadc1.Init.ContinuousConvMode = ENABLE;                 /* Continuous conversion */
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;       /* Software trigger */
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;             /* Right aligned */
    hadc1.Init.NbrOfConversion = 1;                         /* 1 channel */
    hadc1.Init.DMAContinuousRequests = ENABLE;              /* DMA circular mode */
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    HAL_ADC_Init(&hadc1);

    /* Configure ADC1 Channel 0 (PA0) */
    sConfig.Channel = ADC_CHANNEL_0;                        /* ADC1_IN0 = PA0 */
    sConfig.Rank = 1;                                       /* First rank */
    sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;       /* Maximum sampling for accuracy */
    sConfig.Offset = 0;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/**
 * @brief Error Handler - called when assertion fails
 */
void Error_Handler(void)
{
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while(1)
    {
    }
}

/**
 * @brief EXTI Initialization
 * PC6 -> RFID IRQ (EXTI9_5)
 * Falling edge trigger
 */
static void MX_EXTI_Init(void)
{
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/**
 * @brief System Clock Configuration
 * 
 * CLOCK SETUP:
 * ============
 * HSI: 16 MHz (Internal oscillator)
 * PLL:
 *   - PLLM = 16  (16MHz / 16 = 1 MHz)
 *   - PLLN = 336 (1MHz * 336 = 336 MHz)
 *   - PLLP = 2   (336MHz / 2 = 168 MHz) -> SYSCLK
 *   - PLLQ = 7   (336MHz / 7 = 48 MHz) -> USB/SDIO
 * 
 * PERIPHERAL CLOCKS:
 * ==================
 * SYSCLK (AHB): 168 MHz
 * APB1 (Low Speed): 84 MHz (Prescaler 4)
 * APB2 (High Speed): 84 MHz (Prescaler 2)
 * Timers: 168 MHz (multiplied by prescaler when APB != 2)
 * 
 * DERIVED CLOCKS:
 * ===============
 * SPI1 (APB2): 84MHz
 * SPI2 (APB1): 42MHz
 * I2C1 (APB1): 42MHz
 * ADC (APB2): 84MHz (with /4 prescaler = 21MHz ADC clock)
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Oscillator configuration */
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

    /* Clock configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
