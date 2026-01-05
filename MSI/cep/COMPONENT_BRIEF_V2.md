# STM32F407 - Technical Reference Brief

---

## 1. BUZZER (PA15 → TIM2_CH1)

**Function:** Produce sound using rapid power switching at different frequency level.

**Technical:** 
- Pin: PA15 | Timer: TIM2_CH1 | Frequency: 20Hz-20kHz | Clock: 1MHz
- Frequency calculation: 1,000,000 / (Period + 1)

**Operation:** PWM frequency determine pitch. High frequency = high pitch. Set period value, use 50% duty cycle for clear sound. Stop PWM to silence.

**Code:** `buzzer_play(330);` `buzzer_stop();`

---

## 2. SERVO (PB0 → TIM3_CH3)

**Function:** Rotate to precise angle based on pulse width input.

**Technical:**
- Pin: PB0 | Timer: TIM3_CH3 | Frequency: 50Hz (fixed) | Period: 20000
- Angle formula: Pulse = 1000 + (angle × 1000 / 180)
- 1ms = 0°, 1.5ms = 90°, 2ms = 180°

**Operation:** Convert angle to pulse value. Set PWM. Servo move to target angle automatically.

**Code:** `servo_set_angle(0);` `servo_set_angle(90);` `servo_set_angle(180);`

---

## 3. FAN (PD15 → TIM4_CH4)

**Function:** Control DC motor speed with duty cycle variation.

**Technical:**
- Pin: PD15 | Timer: TIM4_CH4 | Frequency: 50Hz | Period: 20000
- Range: 0-20000 (0%-100%)

**Speed Values:**
- 0 → 0% | 5000 → 25% | 10000 → 50% | 15000 → 75% | 20000 → 100%

**Operation:** Higher PWM value = longer ON time = more average voltage = faster speed. Motor inertia smooth switching.

**Code:** `fan_start();` `fan_set_speed(20000);` `fan_set_speed(10000);` `fan_stop();`

---

## 4. TEMPERATURE SENSOR (PA0 → ADC1_IN0)

**Function:** Measure ambient temperature using linear voltage output.

**Technical:**
- Pin: PA0 | Sensor: LM35 | Output: 10mV/°C | Range: 0-100°C
- ADC: 12-bit (0-4095) | Reference: 3.3V
- Formula: Temperature = (ADC_Value × 330) / 4095

**Operation:** ADC sample voltage continuously. DMA save data automatically. Apply noise filter. Convert to temperature.

**Code:** `LM35_StartConversion();` `float temp = LM35_GetTemperature();`

---

## 5. LCD DISPLAY (PB6, PB7 → I2C1)

**Function:** Display 16×2 character text using two-wire serial protocol.

**Technical:**
- Pins: PB6 (SCL), PB7 (SDA) | Protocol: I2C 100kHz
- Module: PCF8574 | Address: 0x27
- Communication: Command byte (position) → Data byte (character)

**Operation:** Initialize with configuration. Set cursor position. Send ASCII character. PCF8574 convert I2C to parallel for display.

**Code:** `lcd_init();` `lcd_set_cursor(0, 0);` `lcd_print("Text");`

---

## 6. ACCELEROMETER (PA5-7, PE3 → SPI1)

**Function:** Detect acceleration in 3 dimension (X, Y, Z axis).

**Technical:**
- Pins: PA5 (SCK), PA6 (MISO), PA7 (MOSI), PE3 (CS) | Protocol: SPI 5.25MHz
- Chip: LIS302DL | Axis: 3 | Range: ±2g or ±8g | Output: 8-bit per axis

**Operation:** Pull CS low. Send register address + read bit. Receive acceleration value. Pull CS high. Repeat for each axis.

**Code:** `ACC_Init();` `int8_t x = ACC_ReadAxis(LIS302DL_OUT_X);`

---

## 7. RFID READER (PB13-15, PD8-9, PC6 → SPI2)

**Function:** Detect and read unique ID (UID) from RFID card.

**Technical:**
- Pins: PB13 (SCK), PB14 (MISO), PB15 (MOSI), PD8 (CS), PD9 (RST), PC6 (IRQ)
- Protocol: SPI 5.25MHz | Frequency: 13.56MHz | Range: ~10cm | UID: 4-7 byte

**Operation:** Reset chip. Send REQUEST command. If card detected, send ANTICOLLISION to read UID. Extract 5 byte response.

**Code:** `MFRC522_Init();` `MFRC522_Request();` `MFRC522_Anticoll(uid);`

---

## CLOCK STRUCTURE

**Source:** HSI 16MHz internal oscillator

**Path:** 16MHz ÷16 → 1MHz ×336 → 336MHz PLL ÷2 → SYSCLK 168MHz
- APB1: 42MHz (SPI2, I2C1, TIM2-7)
- APB2: 84MHz (SPI1, ADC, TIM1, TIM8-11)

**Timer Clock:** With prescaler 84-1 → 1MHz (84MHz ÷ 84)

---

## QUICK REFERENCE

| Component | Pin | Protocol | Speed | Purpose |
|-----------|-----|----------|-------|---------|
| Buzzer | PA15 | PWM (TIM2) | 20-20k Hz | Audio tone |
| Servo | PB0 | PWM (TIM3) | 50Hz | Angle control |
| Fan | PD15 | PWM (TIM4) | 50Hz | Speed control |
| Temperature | PA0 | ADC | 169kHz | Temperature |
| LCD | PB6,7 | I2C | 100kHz | Display |
| Accelerometer | PA5-7,PE3 | SPI1 | 5.25MHz | Motion detect |
| RFID | PB13-15,PD8-9,PC6 | SPI2 | 5.25MHz | Card ID |

---

## INITIALIZATION ORDER

1. HAL_Init()
2. SystemClock_Config() 
3. MX_GPIO_Init()
4. MX_SPI2_Init()
5. MX_TIM2_Init()
6. MX_TIM3_Init()
7. MX_TIM4_Init()
8. MX_I2C1_Init()
9. MX_EXTI_Init()
10. RFID_Init()
11. lcd_init()

