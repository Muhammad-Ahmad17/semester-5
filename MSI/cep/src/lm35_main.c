/*
STM32F407 Smart Room System
Main control firmware
*/

#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
I2C_HandleTypeDef hi2c1;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ======================== ADC DATA BUFFER ======================== */
volatile uint32_t adc_value = 0;  /* Stores raw ADC value */
static uint32_t adc_avg = 0;      /* Moving average for filtering */

/* ======================== LCD CONFIGURATION ======================== */
#define LCD_ADDR (0x27 << 1)  /* PCF8574 Address with 7-bit left shift */

/* ======================== BUZZER FREQUENCIES ======================== */
#define NOTE_E4        330    /* Neutral tone */
#define NOTE_AFFIRMATIVE 523  /* Higher tone - SUCCESS (C5) */
#define NOTE_ALERT     165    /* Lower tone - FAILURE/WARNING (E3) */
#define NOTE_EARTHQUAKE 1047  /* Very high tone - CRITICAL (C6) */

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

#define LIS302DL_CTRL_REG1 0x20
#define LIS302DL_OUT_X     0x29
#define LIS302DL_OUT_Y     0x2B
#define LIS302DL_OUT_Z     0x2D

#define ACC_CS_LOW()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define ACC_CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

#define RFID_CS_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)
#define RFID_CS_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)

#define RFID_RST_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define RFID_RST_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)

#define IR_SENSOR_PIN       GPIO_PIN_7
#define EXIT_BUTTON_PIN     GPIO_PIN_8
#define LED_LIGHT_PIN       GPIO_PIN_9
#define LED_OFF_BUTTON_PIN  GPIO_PIN_6

#define ir_sensor_read()       HAL_GPIO_ReadPin(GPIOC, IR_SENSOR_PIN)
#define exit_button_read()     (HAL_GPIO_ReadPin(GPIOC, EXIT_BUTTON_PIN) == GPIO_PIN_RESET)
#define led_off_button_read()  (HAL_GPIO_ReadPin(GPIOE, LED_OFF_BUTTON_PIN) == GPIO_PIN_RESET)
#define light_on()             HAL_GPIO_WritePin(GPIOC, LED_LIGHT_PIN, GPIO_PIN_SET)
#define light_off()            HAL_GPIO_WritePin(GPIOC, LED_LIGHT_PIN, GPIO_PIN_RESET)

/* ======================== DATA STRUCTURES ======================== */
typedef struct {
    int8_t x, y, z;
    uint16_t motion_count;
    uint32_t last_motion_time;
    int8_t baseline_x, baseline_y, baseline_z;  /* Baseline position */
    uint8_t calibrated;  /* 1 if baseline is set */
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

void EXTI9_5_IRQHandler(void);

static uint8_t int_to_string(int32_t value, char *buffer)
{
    uint8_t i = 0;
    uint8_t is_negative = 0;
    
    /* Handle negative numbers */
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }
    
    /* Handle zero case */
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return i;
    }
    
    /* Convert digits (reverse order) */
    uint8_t start = i;
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    /* Add negative sign */
    if (is_negative) {
        buffer[i++] = '-';
    }
    
    /* Reverse the string */
    uint8_t end = i - 1;
    start = is_negative ? 1 : 0;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    
    buffer[i] = '\0';
    return i;
}

static void float_to_string(float value, char *buffer, uint8_t decimals)
{
    /* Handle negative values */
    uint8_t is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value = -value;
        *buffer++ = '-';
    }
    
    /* Extract integer part */
    int32_t int_part = (int32_t)value;
    
    /* Extract fractional part */
    float frac_part = value - (float)int_part;
    
    /* Convert integer part */
    uint8_t len = int_to_string(int_part, buffer);
    buffer += len;
    
    /* Add decimal point and fractional part */
    if (decimals > 0) {
        *buffer++ = '.';
        
        /* Scale fractional part based on decimal places */
        if (decimals == 1) {
            frac_part *= 10.0f;
        } else if (decimals == 2) {
            frac_part *= 100.0f;
        }
        
        int32_t frac_int = (int32_t)(frac_part + 0.5f);  /* Round */
        
        /* Handle carry-over from rounding */
        if (decimals == 1 && frac_int >= 10) {
            frac_int = 0;
        } else if (decimals == 2 && frac_int >= 100) {
            frac_int = 0;
        }
        
        /* Convert fractional digits */
        if (decimals == 1) {
            *buffer++ = '0' + (frac_int % 10);
        } else if (decimals == 2) {
            *buffer++ = '0' + ((frac_int / 10) % 10);
            *buffer++ = '0' + (frac_int % 10);
        }
    }
    
    *buffer = '\0';
}

static void temp_to_string(float temp, char *buffer)
{
    float_to_string(temp, buffer, 1);
    
    /* Find end of string */
    while (*buffer != '\0') {
        buffer++;
    }
    
    /* Add 'C' suffix */
    *buffer++ = 'C';
    *buffer = '\0';
}

int main2(void)
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


/* ======================== MAIN FUNCTION (FULL AUTOMATION) ======================== */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Initialize all peripherals in order */
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

    /* Initialize devices */
    RFID_Init();
    ACC_Init();
    lcd_init();
    LM35_StartConversion();  /* Start ADC */

    /* LCD Welcome Screen */
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

    /* Lock door initially */
    servo_set_angle(0);  /* 0° = Locked */
    
    /* ======================== MAIN AUTOMATION LOOP ======================== */
    
    uint8_t room_occupied = 0;
    uint8_t tagType[2]; // tagType stores the Answer To reQuest A response 
    uint8_t uid[10];
    uint8_t light_manual_control = 0;  /* Track if user manually controlled light */
    char temp_str[16];
    char display_buf[32];
    uint8_t lcd_state = 0;  /* Sliding display state (0=temp, 1=motion, 2=light) */
    uint32_t last_lcd_update = 0;
    
    while(1)
    {
        /* ==================== STATE 1: WAITING FOR ENTRY ==================== */
        if (!room_occupied) {
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("Scan RFID Card");
            lcd_set_cursor(1, 0);
            lcd_print("To Enter...");
            
            /* Wait for RFID card */
            if (MFRC522_Request(tagType) == 1) { // Card detected
                if (MFRC522_Anticoll(uid) == 1) {
                    /* Validate against whitelist cards */
                    uint8_t valid_card = 0;
                    uint8_t card1[] = {0x63, 0x95, 0x4e, 0x56};  /* 63954e56 */
                    uint8_t card2[] = {0xd0, 0x6e, 0x6d, 0x32};  /* d06e6d32 */
                    
                    /* Check if UID matches card1 */
                    if (uid[0] == card1[0] && uid[1] == card1[1] && 
                        uid[2] == card1[2] && uid[3] == card1[3]) {
                        valid_card = 1;
                    }
                    
                    /* Check if UID matches card2 */
                    if (uid[0] == card2[0] && uid[1] == card2[1] && 
                        uid[2] == card2[2] && uid[3] == card2[3]) {
                        valid_card = 1;
                    }
                    
                    if (!valid_card) {
                        /* Invalid card - Access Denied */
                        lcd_clear();
                        lcd_set_cursor(0, 0);
                        lcd_print("Access DENIED!");
                        
                        /* Rejection beep  */
                        buzzer_play(165);  
                        HAL_Delay(300);
                        buzzer_stop();
                        HAL_Delay(2000);
                    } else {
                        /* Valid card detected - Access Granted */
                        lcd_clear();
                        lcd_set_cursor(0, 0);
                        lcd_print("Access Granted!");
                        
                        /* Beep confirmation */
                        buzzer_play(NOTE_E4);
                        HAL_Delay(200);
                        buzzer_stop();
                        
                        /* Unlock door */
                        servo_set_angle(90);  
                        HAL_Delay(1000);
                    
                    /* Wait for person to enter (IR sensor) */
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_print("Door Open");
                    lcd_set_cursor(1, 0);
                    lcd_print("Enter Now...");
                    
                    uint32_t wait_start = HAL_GetTick();
                    uint32_t last_ir_detect_time = HAL_GetTick();  /* Track last IR detection */
                    uint8_t person_entered = 0;
                    uint8_t door_timeout = 0;  /* Flag for no-motion timeout */
                    
                    while ((HAL_GetTick() - wait_start) < 5000) {  /* Max 5 sec safety timeout */
                        /* ===== NO MOTION TIMEOUT: Close door if no IR detection for 5 seconds ===== */
                        uint32_t no_motion_duration = HAL_GetTick() - last_ir_detect_time;
                        if (no_motion_duration > 5000) {  /* 5 seconds with NO IR detection */
                            door_timeout = 1;
                            break;  /* Exit immediately - close door for security */
                        }
                        
                        if (ir_sensor_read() == GPIO_PIN_SET) {
                            /* Person detected passing through */
                            last_ir_detect_time = HAL_GetTick();  /* Reset no-motion timer */
                            HAL_Delay(2000);  /* Wait for person to fully pass */
                            
                            /* Wait until path is clear (WITH TIMEOUT to prevent infinite wait) */
                            uint32_t clear_start = HAL_GetTick();
                            while ((HAL_GetTick() - clear_start) < 3000) {  /* 3 sec max to clear path */
                                if (ir_sensor_read() == GPIO_PIN_RESET) {
                                    /* Path is clear */
                                    person_entered = 1;
                                    break;
                                }
                                HAL_Delay(50);
                            }
                            break;
                        }
                        HAL_Delay(50);
                    }
                    
                    /* Close door after delay */
                    HAL_Delay(3000);  /* 3 sec delay before closing */
                    servo_set_angle(0);  /* Lock door */
                    
                    if (person_entered) {
                        /*  PERSON SUCCESSFULLY ENTERED - AFFIRMATIVE TONE */
                        buzzer_play(NOTE_AFFIRMATIVE);
                        HAL_Delay(150);
                        buzzer_stop();
                        HAL_Delay(100);
                        buzzer_play(NOTE_AFFIRMATIVE);
                        HAL_Delay(150);
                        buzzer_stop();
                        
                        /* Activate room systems */
                        room_occupied = 1;
                        light_on();  /* Turn on light by default */
                        light_manual_control = 0;
                        
                        lcd_clear();
                        lcd_set_cursor(0, 0);
                        lcd_print("Welcome!");
                        HAL_Delay(1500);
                        } else if (door_timeout) {
                            /* NO MOTION DETECTED FOR 5 SEC - SECURITY CLOSE */
                            buzzer_play(NOTE_ALERT);
                            HAL_Delay(300);
                            buzzer_stop();
                            HAL_Delay(100);
                            buzzer_play(NOTE_ALERT);
                            HAL_Delay(300);
                            buzzer_stop();
                            
                            /* No motion detected */
                            lcd_clear();
                            lcd_set_cursor(0, 0);
                            lcd_print("No Motion!");
                            lcd_set_cursor(1, 0);
                            lcd_print("Door Locked");
                            HAL_Delay(2000);
                        } else {
                            /* ENTRY FAILED/TIMEOUT - ALERT TONE */
                            buzzer_play(NOTE_ALERT);
                            HAL_Delay(300);
                            buzzer_stop();
                            HAL_Delay(100);
                            buzzer_play(NOTE_ALERT);
                            HAL_Delay(300);
                            buzzer_stop();
                            
                            /* Timeout - no one entered */
                            lcd_clear();
                            lcd_set_cursor(0, 0);
                            lcd_print("Entry Failed");
                            lcd_set_cursor(1, 0);
                            lcd_print("No Detection");
                            HAL_Delay(2000);
                        }
                    }
                }
            }
            
            HAL_Delay(100);
        }
        
        /* ==================== STATE 2: ROOM OCCUPIED - MONITORING ==================== */
        if (room_occupied) {
            /* Read temperature */
            float temp = LM35_GetTemperature();
            
            /* Fan control: step-based speed ramping with 1-2°C increments */
            /* OFF: ≤20°C, 25%: 20-21°C, 50%: 21-22°C, 75%: 22-23°C, 100%: ≥23°C */
            uint16_t fan_speed = 0;
            
            if (temp > 23.0f) {
                /* ≥23°C - Full speed (100%) */
                fan_speed = 20000;
            } else if (temp > 22.0f) {
                /* 22-23°C - 75% speed */
                fan_speed = 15000;
            } else if (temp > 21.0f) {
                /* 21-22°C - 50% speed */
                fan_speed = 10000;
            } else if (temp > 20.0f) {
                /* 20-21°C - 25% speed */
                fan_speed = 5000;
            } else {
                /* ≤20°C - Fan OFF */
                fan_speed = 0;
            }
            
            /* Apply fan speed */
            if (fan_speed > 0) {
                /* Turn ON fan if not already running */
                if (!fan_timer.running) {
                    fan_start();
                }
                fan_set_speed(fan_speed);
            } else {
                /* Turn OFF fan when temperature drops */
                if (fan_timer.running) {
                    fan_set_speed(0);
                    HAL_Delay(50);
                    fan_stop();
                }
            }
            
            /* Manual light control */
            if (led_off_button_read()) {
                /* Toggle light*/
                if (HAL_GPIO_ReadPin(GPIOC, LED_LIGHT_PIN) == GPIO_PIN_SET) {
                    light_off();  /* Light is ON, turn OFF */
                } else {
                    light_on();   /* Light is OFF, turn ON */
                }
                light_manual_control = 1;
                HAL_Delay(300);  /* Debounce */
            }
            
            /* Motion detection with baseline comparison */
            motion.x = ACC_ReadAxis(LIS302DL_OUT_X);
            motion.y = ACC_ReadAxis(LIS302DL_OUT_Y);
            motion.z = ACC_ReadAxis(LIS302DL_OUT_Z);
            
            /* Calculate delta from baseline */
            int16_t delta_x = abs(motion.x - motion.baseline_x);
            int16_t delta_y = abs(motion.y - motion.baseline_y);
            int16_t delta_z = abs(motion.z - motion.baseline_z);
            int16_t total_delta = delta_x + delta_y + delta_z;
            
            /* ========== DEMONSTRATION MODE: MORE SENSITIVE ========== */
            /* Minor fluctuation detection (10-40 range) - LOWERED for demo */
            if (total_delta > 10 && total_delta < 40) {
                /* Minor movement detected - rapid alert beeps */
                uint32_t current_time = HAL_GetTick();
                if (current_time - motion.last_motion_time > 3000) {  /* Alert every 3 sec max */
                    /* 3 quick beeps for attention - using neutral tone */
                    for (int i = 0; i < 3; i++) {
                        buzzer_play(NOTE_E4);
                        HAL_Delay(80);
                        buzzer_stop();
                        HAL_Delay(80);
                    }
                    motion.last_motion_time = current_time;
                }
            }
            
            /* Earthquake detection (major fluctuation > 35) - LOWERED from 60 for demo */
            if (total_delta > 35) {
                /*  EARTHQUAKE ALERT - HIGH SENSITIVITY /
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("*** ALERT! ***");
                lcd_set_cursor(1, 0);
                lcd_print("QUAKE DETECTED!");
                
                /* EARTHQUAKE TONE PATTERN - Very distinctive */
                for (int i = 0; i < 8; i++) {
                    buzzer_play(NOTE_EARTHQUAKE);
                    HAL_Delay(150);
                    buzzer_stop();
                    HAL_Delay(80);
                }
                
                HAL_Delay(1000);
            }
            
            /* ========== SLIDING LCD DISPLAY (3 states, 2 sec each) ========== */
            uint32_t current_time = HAL_GetTick();
            if (current_time - last_lcd_update > 2000) {  /* Update every 2 seconds */
                lcd_state = (lcd_state + 1) % 3;  /* Cycle 0->1->2->0 */
                last_lcd_update = current_time;
                
                lcd_clear();
                
                if (lcd_state == 0) {
                    /* Display 1: Temperature & Fan */
                    lcd_set_cursor(0, 0);
                    lcd_print("Temp: ");
                    temp_to_string(temp, display_buf);
                    lcd_print(display_buf);
                    
                    lcd_set_cursor(1, 0);
                    if (!fan_timer.running) {
                        lcd_print("Fan: OFF");
                    } else if (temp > 23.0f) {
                        lcd_print("Fan: ON 100%");
                    } else if (temp > 22.0f) {
                        lcd_print("Fan: ON 75%");
                    } else if (temp > 21.0f) {
                        lcd_print("Fan: ON 50%");
                    } else {
                        lcd_print("Fan: ON 25%");
                    }
                    
                } else if (lcd_state == 1) {
                    /* Display 2: Motion Status */
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
                    /* Display 3: Light & System Status */
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
            
            /* Check exit button (PC8) */
            if (exit_button_read()) {
                /* Person wants to exit */
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("Exiting...");
                lcd_set_cursor(1, 0);
                lcd_print("Please Wait");
                
                /* Unlock door */
                servo_set_angle(90);
                HAL_Delay(3000);
                
                /* Shutdown all systems */
                light_off();
                if (fan_timer.running) {
                    fan_set_speed(0);
                    HAL_Delay(100);
                    fan_stop();
                }
                
                /* Wait for person to exit (IR sensor) */
                lcd_clear();
                lcd_set_cursor(0, 0);
                lcd_print("Door Open");
                lcd_set_cursor(1, 0);
                lcd_print("Exit Now...");
                
                uint32_t exit_wait = HAL_GetTick();
                uint32_t last_exit_ir_time = HAL_GetTick();  /* Track last IR detection */
                uint8_t person_exited = 0;
                uint8_t exit_timeout = 0;  /* Flag for no-motion timeout */
                
                while ((HAL_GetTick() - exit_wait) < 15000) {  /* Max 15 sec safety timeout */
                    /* ===== NO MOTION TIMEOUT: Close door if no IR detection for 5 seconds ===== */
                    uint32_t exit_no_motion = HAL_GetTick() - last_exit_ir_time;
                    if (exit_no_motion > 5000) {  /* 5 seconds with NO IR detection */
                        exit_timeout = 1;
                        break;  /* Exit immediately - close door for security */
                    }
                    
                    if (ir_sensor_read() == GPIO_PIN_SET) {
                        last_exit_ir_time = HAL_GetTick();  /* Reset no-motion timer */
                        HAL_Delay(1000);  /* Wait 1 sec for person to start exiting */
                        
                        /* Wait until path is clear (WITH EXTENDED TIMEOUT for slow exits) */
                        uint32_t exit_clear_start = HAL_GetTick();
                        uint8_t path_clear_detected = 0;
                        
                        while ((HAL_GetTick() - exit_clear_start) < 5000) {  /* 5 sec max to clear path */
                            if (ir_sensor_read() == GPIO_PIN_RESET) {
                                /* Path is clear */
                                path_clear_detected = 1;
                                person_exited = 1;
                                break;
                            }
                            HAL_Delay(50);
                        }
                        
                        /* If we detected motion but path didn't clear in 5 sec, still count as exiting */
                        if (!path_clear_detected) {
                            person_exited = 1;  /* Person is exiting, even if sensor still detects them */
                        }
                        
                        break;  /* Exit main loop */
                    }
                    HAL_Delay(50);
                }
                
                /* Close door */
                HAL_Delay(3000);
                servo_set_angle(0);  /* Lock */
                
                if (person_exited) {
                    /* PERSON SUCCESSFULLY EXITED - AFFIRMATIVE TONE */
                    buzzer_play(NOTE_AFFIRMATIVE);
                    HAL_Delay(150);
                    buzzer_stop();
                    HAL_Delay(100);
                    buzzer_play(NOTE_AFFIRMATIVE);
                    HAL_Delay(150);
                    buzzer_stop();
                    
                    /* Reset to IDLE state - person actually left */
                    room_occupied = 0;
                    
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_print("Goodbye!");
                    lcd_set_cursor(1, 0);
                    lcd_print("Room Secured");
                    HAL_Delay(2000);
                } else if (exit_timeout) {
                    /* NO MOTION DETECTED FOR 5 SEC - UNDO EXIT */
                    buzzer_play(NOTE_ALERT);
                    HAL_Delay(300);
                    buzzer_stop();
                    HAL_Delay(100);
                    buzzer_play(NOTE_ALERT);
                    HAL_Delay(300);
                    buzzer_stop();
                    
                    /* UNDO: Lock door, turn light back ON, stay in room_occupied */
                    servo_set_angle(0);  /* Lock door */
                    light_on();          /* Turn light back ON */
                    
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_print("Exit Cancelled");
                    lcd_set_cursor(1, 0);
                    lcd_print("Room Re-Active");
                    HAL_Delay(2000);
                } else {
                    /* EXIT FAILED/TIMEOUT - UNDO EXIT */
                    buzzer_play(NOTE_ALERT);
                    HAL_Delay(300);
                    buzzer_stop();
                    HAL_Delay(100);
                    buzzer_play(NOTE_ALERT);
                    HAL_Delay(300);
                    buzzer_stop();
                    
                    /* UNDO: Lock door, turn light back ON, stay in room_occupied */
                    servo_set_angle(0);  /* Lock door */
                    light_on();          /* Turn light back ON */
                    
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_print("Exit Failed");
                    lcd_set_cursor(1, 0);
                    lcd_print("Room Re-Active");
                    HAL_Delay(2000);
                }
            }
        }
        
        HAL_Delay(500);  /* Main loop delay - increased for stable display */
    }
}


/* ======================== LM35 TEMPERATURE FUNCTIONS ======================== */

void LM35_StartConversion(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_value, 1);
}

// read raw val.
uint32_t LM35_ReadADC(void)
{
    return adc_value;
}
/*
 *   Temperature = Voltage / 0.01  (LM35: 10mV per °C)
 *   Voltage = ADC_Value * (3.3 / 4095)
 *   Temperature = (ADC_Value * 3.3 / 4095) / 0.01
*   Temperature = (ADC_Value * 3.3 / 4095) * 100

 */
float LM35_GetTemperature(void)
{
    /* Get current ADC value from DMA buffer */
    uint32_t raw_adc = LM35_ReadADC();

    /* Smooth exponential filter: 31/32 */
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
// clk = tim2 clk = apb1 (prescale by 2) = 84 mhz
//timer clk = 
void buzzer_play(uint32_t freq)
{
    uint32_t timer_clock = 1000000;
    uint32_t period = (timer_clock / freq) - 1;

    /* CRITICAL: Stop timer before changing parameters */
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    HAL_Delay(5);  /* Brief delay to ensure timer stops */
    
    /* Reset the timer counter to ensure clean restart */
    __HAL_TIM_DISABLE(&htim2);
    htim2.Instance->CNT = 0;  /* Reset counter */
    __HAL_TIM_ENABLE(&htim2);
    
    /* Update timer parameters */
    __HAL_TIM_SET_AUTORELOAD(&htim2, period);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, period / 2); //50% duty cycle 
    
    /* Start PWM */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void buzzer_stop(void)
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
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
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse);
}

/* ======================== FAN CONTROL FUNCTIONS ======================== */


void fan_start(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    fan_timer.running = 1;
}

void fan_set_speed(uint16_t duty_cycle)
{
    // duty cycle max 20000
    if (duty_cycle > 20000) duty_cycle = 20000;
    
    /* Set PWM pulse width */
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, duty_cycle);
}


void fan_stop(void)
{
    MX_TIM4_Init();
//stop
}

// void fan_set_duration(uint16_t duration_ms, uint16_t speed)
// {
//     fan_start();
//     fan_set_speed(speed);
//     fan_timer.duration_ms = duration_ms;
//     fan_timer.start_time = HAL_GetTick();
//     fan_timer.running = 1;
// }
 
// void fan_update_timer(void)
// {
//     if (!fan_timer.running) return;
    
//     // if timer not change 
//     if (fan_timer.duration_ms == 0) return;  /* No auto-stop set */
    
//     uint32_t elapsed = HAL_GetTick() - fan_timer.start_time;
    
//     if (elapsed >= fan_timer.duration_ms) {
//         fan_set_speed(0);
//         fan_stop();  /* This will also clear running flag */
//     }
// }


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

void RFID_Init(void)
{
    MFRC522_Init();
}



/* ======================== PERIPHERAL INITIALIZATION ======================== */

/**
 * @brief GPIO Initialization
 * PA0  -> ADC1_IN0 (Analog Input for LM35)
 * PA5, PA6, PA7 -> SPI1 (Accelerometer)
 * PA15 -> TIM2_CH1 (Buzzer)
 * PE3  -> ACC CS
 * PB0  -> TIM3_CH3 (Servo)
 * PB6, PB7 -> I2C1 (LCD)
 * PB13, PB14, PB15 -> SPI2 (RFID)
 * PD15 -> TIM4_CH4 (Fan PWM)
 * PD8, PD9 -> RFID CS, RST
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

    /* ---- PA15: TIM2_CH1 (Buzzer PWM) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ---- PB0: TIM3_CH3 (Servo PWM) ---- */
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

    /* ---- PE6: LED OFF Button (Input with Pull-up) ---- */
    GPIO_InitStruct.Pin = LED_OFF_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  /* Pull-up, button connects to GND */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* ---- PB6, PB7: I2C1 (LCD) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ---- PD15: TIM4_CH4 (Fan PWM) ---- */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
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

    /* ======================== NEW COMPONENTS ======================== */
    
    /* ---- PC7: IR Sensor (Input) ---- */
    GPIO_InitStruct.Pin = IR_SENSOR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;  /* Pull-down for active-high IR sensor */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ---- PC8: Exit Button (Input with Pull-up) ---- */
    GPIO_InitStruct.Pin = EXIT_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  /* Pull-up, button connects to GND */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ---- PC9: LED Light (Output) ---- */
    GPIO_InitStruct.Pin = LED_LIGHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* Initialize LED to OFF state */
    light_off();
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
 * @brief TIM2 Initialization (Buzzer PWM)
 * 
 * TIMER CONFIGURATION:
 * ====================
 * Clock: APB1 = 84 MHz
 * Prescaler: 84 - 1 = 83 → Timer Clock = 1 MHz
 * Period: Dynamic (set by buzzer_play function)
 * Channel 1: Buzzer PWM on PA15
 * 
 * FREQUENCY CALCULATION:
 * Frequency = 1,000,000 / (Period + 1)
 * Examples:
 * - Period = 3030: 330 Hz (NOTE_E4)
 * - Period = 3787: 264 Hz (NOTE_C4)
 * - Period = 4545: 220 Hz (NOTE_A3)
 */
static void MX_TIM2_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 84 - 1;           /* Timer clock = 84MHz / 84 = 1 MHz */
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1000 - 1;            /* Default period */
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim2);

    /* Channel 1: Buzzer */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;                     /* Start silent */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/**
 * @brief TIM3 Initialization (Servo PWM)
 * 
 * TIMER CONFIGURATION:
 * ====================
 * Clock: APB1 = 84 MHz
 * Prescaler: 84 - 1 = 83 → Timer Clock = 1 MHz
 * Period: 20000 - 1 = 19999 → Frequency = 50 Hz
 * Channel 3: Servo PWM on PB0
 * 
 * SERVO PULSE CALCULATION:
 * Pulse = 1000 + (angle * 1000 / 180)
 * - 0°:   1000 µs (1 ms)
 * - 90°:  1500 µs (1.5 ms) - neutral
 * - 180°: 2000 µs (2 ms)
 */
static void MX_TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 84 - 1;           /* Timer clock = 84MHz / 84 = 1 MHz */
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 20000 - 1;           /* 50 Hz frequency (1MHz / 20000) */
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    /* Channel 3: Servo */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1500;                  /* 1.5ms pulse (90° position) */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

/**
 * @brief TIM4 Initialization (Fan PWM)
 * 
 * TIMER CONFIGURATION:
 * ====================
 * Clock Source: APB1 Timer Clock = 84 MHz
 * Prescaler: 84 - 1 = 83 → Timer Clock = 1 MHz
 * Period: 20000 - 1 = 19999 → Frequency = 50 Hz
 * Channel 4: Fan PWM on PD1
 * 
 * DUTY CYCLE CALCULATION:
 * =======================
 * Duty Cycle = (Pulse / Period) × 100%
 * 
 * Examples:
 *   - Pulse = 0,    Duty = 0% (OFF)
 *   - Pulse = 5000, Duty = 25%
 *   - Pulse = 10000, Duty = 50% (half speed)
 *   - Pulse = 15000, Duty = 75%
 *   - Pulse = 20000, Duty = 100% (full speed)
 */
static void MX_TIM4_Init(void)
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 84 - 1;           /* Timer clock = 84MHz / 84 = 1 MHz */
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 20000 - 1;           /* 50 Hz frequency (1MHz / 20000) */
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim4);

    /* Channel 4: Fan */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;                     /* Start at 0% duty cycle */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
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
 * @brief DMA Initialization for ADC
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
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
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
 * Continuous Mode: Disabled (polling mode)
 * DMA: Disabled (using polling)
 * Sampling Time: 112 cycles (max accuracy)
 * Clock Prescaler: 4 (APB2 84MHz / 4 = 21 MHz)
 * 
 * CONVERSION TIME CALCULATION:
 * ============================
 * Total cycles = Sampling cycles + 12 (conversion)
 * Total cycles = 112 + 12 = 124 cycles
 * Sample time = 124 / 21MHz ≈ 5.9 µs per reading
 */
static void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;  /* 84MHz / 4 = 21MHz */
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;             /* 12-bit resolution */
    hadc1.Init.ScanConvMode = DISABLE;                      /* Single channel */
    hadc1.Init.ContinuousConvMode = ENABLE;                 /* Continuous conversion for DMA */
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;       /* Software trigger */
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;             /* Right aligned */
    hadc1.Init.NbrOfConversion = 1;                         /* 1 channel */
    hadc1.Init.DMAContinuousRequests = ENABLE;              /* Enable DMA continuous requests */
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
