/*

STM32F407 Motion Sensor + Buzzer + I2C LCD (16x2)
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
SERVO
-> 5V
-> GND
PD14  -> TIM4_CH1 and PD13(not used)
*/

#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------- HANDLES -------------------- */
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim3;
I2C_HandleTypeDef hi2c1;

/* -------------------- ACC REGISTERS -------------------- */
#define LIS302DL_CTRL_REG1 0x20
#define LIS302DL_OUT_X     0x29
#define LIS302DL_OUT_Y     0x2B
#define LIS302DL_OUT_Z     0x2D

/* -------------------- LCD SETTINGS -------------------- */
#define LCD_ADDR 0x27 << 1

/* -------------------- BUZZER NOTES -------------------- */
#define NOTE_E4 330

/* -------------------- CS MACROS -------------------- */
#define ACC_CS_LOW()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define ACC_CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

/* -------------------- MOTION DATA -------------------- */
typedef struct {
    int8_t x, y, z;
    uint16_t motion_count;
    uint32_t last_motion_time;
} MotionData;

MotionData motion = {0, 0, 0, 0, 0};

/* -------------------- FUNCTION PROTOTYPES -------------------- */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);

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
void servo_neutral(void);

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
    MX_TIM3_Init();
    MX_I2C1_Init();

    /* Initialize devices */
    ACC_Init();
    lcd_init();

    /* Initial LCD welcome screen */
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Motion Detector");
    lcd_set_cursor(1, 0);
    lcd_print("Initializing...");
    HAL_Delay(2000);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Motion: 0");
    lcd_set_cursor(1, 0);
    lcd_print("Ready");
    HAL_Delay(1000);

    motion.last_motion_time = 0;
    motion.motion_count = 0;

    /* ===================== MAIN LOOP ===================== */
    while(1)
    {
        /* Read accelerometer */
        motion.x = ACC_ReadAxis(LIS302DL_OUT_X);
        motion.y = ACC_ReadAxis(LIS302DL_OUT_Y);
        motion.z = ACC_ReadAxis(LIS302DL_OUT_Z);

        /* Calculate acceleration magnitude */
        int16_t accel_mag = (motion.x * motion.x) + 
                            (motion.y * motion.y) + 
                            (motion.z * motion.z);

        /* Motion detection with threshold */
        if(accel_mag > 150)
        {
            uint32_t current_time = HAL_GetTick();

            /* Debounce: 500ms between detections */
            if((current_time - motion.last_motion_time) > 500)
            {
                motion.motion_count++;
                motion.last_motion_time = current_time;

                /* ---- UPDATE LCD ---- */
                lcd_clear();
                
                /* Row 0: Motion count */
                lcd_set_cursor(0, 0);
                lcd_print("Motion: ");
                char count_str[5];
                sprintf(count_str, "%d", motion.motion_count);
                lcd_print(count_str);

                /* Row 1: Acceleration values */
                lcd_set_cursor(1, 0);
                char accel_str[16];
                sprintf(accel_str, "X:%d Y:%d Z:%d", motion.x, motion.y, motion.z);
                lcd_print(accel_str);

                /* ---- SERVO RESPONSE (Only motor, no buzzer) ---- */
                servo_set_angle(90);
                HAL_Delay(300);
                servo_set_angle(0);
                HAL_Delay(300);
                servo_neutral();  // Back to 90°
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
}

void buzzer_stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
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

    // Servo PWM: 20ms period, 1-2ms pulse (1000-2000 µs)
    // For 1MHz timer: 20ms = 20000 counts
    uint32_t pulse = 1000 + ((uint32_t)angle * 1000) / 180;

    __HAL_TIM_SET_AUTORELOAD(&htim3, 20000 - 1);  // 20ms period for servo
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse);
}

void servo_neutral(void)
{
    servo_set_angle(90);
}


/* ===================== PERIPHERAL INIT ===================== */
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* BUZZER/SERVO PB0 - TIM3_CH3 (SHARED) */
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

    /* SPI PA5 (SCK), PA6 (MISO), PA7 (MOSI) */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
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
