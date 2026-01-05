# STM32F407 - Component Details

---

## KY-006 Buzzer

Buzzer is device which makes sound by oscillating very fast at particular frequency. In our project we use it with PWM signal from PA15 pin which is connected to TIM2 Channel 1. When we change frequency of PWM, the pitch of sound changes. Higher frequency make higher pitch, lower frequency make lower pitch. We use 50% duty cycle for clear sound output. To stop sound, we simply stop the PWM signal.

**PWM Configuration :**
- PA15 → Alternate Function: TIM2_CH1
- Clock Source: APB1 Timer Clock = 84 MHz
- Prescaler: 84 − 1 = 83 (84 MHz / 84 = 1 MHz timer clock)
- Counter Mode: Up
- Period: Dynamic (set by buzzer_play function)
- PWM Mode: PWM Mode 1
- Channel: Channel 1
- Output Compare Preload: Enable
- Polarity: Active High

**Frequency Calculation:**
- Frequency = 1,000,000 / (Period + 1)
- Examples:
  - Period = 3030: 330 Hz (NOTE_E4)
  - Period = 3787: 264 Hz (NOTE_C4)
  - Period = 4545: 220 Hz (NOTE_A3)



---

Servo motor is device which rotate to specific angle based on pulse width of PWM signal. It have three wires - power, ground, and control signal. Control signal is connected to PB0 pin which is connected to TIM3 Channel 3. The servo need 50Hz frequency (20ms period). Pulse width between 1ms to 2ms control the angle. 1ms give 0 degree, 1.5ms give 90 degree, 2ms give 180 degree. We calculate pulse width using angle value and set PWM accordingly.

**PWM Configuration (IOC):**
- PB0 → Alternate Function: TIM3_CH3
- Clock Source: APB1 Timer Clock = 84 MHz
- Prescaler: 84 − 1 = 83 (84 MHz / 84 = 1 MHz timer clock)
- Counter Mode: Up
- Period (ARR): 20000 − 1 = 19999
- PWM Frequency: 50 Hz (1 MHz / 20000)
- PWM Mode: PWM Mode 1
- Channel: Channel 3
- Output Compare Preload: Enable
- Polarity: Active High

**Servo Pulse Calculation:**
- Pulse = 1000 + (angle × 1000 / 180)
- 0°: 1000 µs (1 ms)
- 90°: 1500 µs (1.5 ms) - neutral
- 180°: 2000 µs (2 ms)

---

## 5V DC PWM Fan

Fan motor is simple DC motor which speed is controlled by average voltage applied to it. We use PWM signal from PD15 pin which is connected to TIM4 Channel 4. The PWM frequency is 50Hz. The duty cycle control the speed - higher duty cycle mean higher speed, lower duty cycle mean lower speed. When duty cycle is 0%, motor not spin. When duty cycle is 100%, motor spin at full speed. We can set any value between 0 to 19999 to get any speed between 0% to 100%.

**PWM Configuration (IOC):**
- Clock Source: APB1 Timer Clock = 84 MHz
- Prescaler: 84 − 1 = 83 (84 MHz / 84 = 1 MHz timer clock)
- Counter Mode: Up
- Period (ARR): 20000 − 1 = 19999
- PWM Frequency: 50 Hz (1 MHz / 20000)
- PWM Mode: PWM Mode 1
- Channel: Channel 4
- Duty Cycle Control: CCR4 (0–19999)
- Polarity: Active High

**Speed control:**
- Duty Cycle Range: 0-19999 (0%-100%)
- Speed Control:
  - 0 = 0% (stop)
  - 5000 = 25% (slow)
  - 10000 = 50% (medium)
  - 15000 = 75% (fast)
  - 19999 = 100% (full speed)

---

## Temperature Sensor : LM35

Temperature sensor LM35 is analog device which provides voltage proportional to temperature. It have three pins - pin 1 to VCC (3.3V), pin 2 to ADC input (PA0 of STM32F407), pin 3 to GND. We not use DMA in this sensor because we read it continuously in main loop and process it immediately. We use ADC (Analog to Digital Converter) with polling method. Every sample we read 12-bit value (0-4095) which represent voltage 0-3.3V. Then we convert this digital value to temperature using formula. The LM35 give 10mV per degree Celsius, so at 25 degree it output 250mV, at 50 degree it output 500mV. Its ranges from −55°C to 150°C. It only draw 60μA current , so it has very low self heating.

**Conversion:**

- Resolution: 12-bit(0-4095)
- Reference Voltage: 3.3V
- Output: 10mV per °C
- Temperature Range: 0-100°C (0V-1V)
* Conversion Formula: 
  Temperature = (ADC_Value × 3.3 × 100) / 4095

**ADC Configuration :**
- Clock: PCLK/4 = 21MHz (84MHz / 4)
- Prescaler: 2 (21MHz / 2)
- Resolution: 12-bit
- Channels: 1 (PA0)
- Sampling Time: 84 cycles
- Data Alignment: Right

It only take 84 clk cycles at 10.5MHz (~8ms)for a single ADC conversion.

## 16x2 LCD with I2C module

LCD display is 16 character × 2 line display which use I2C protocol for communication to utilize STM32F407 pins efficently, without I2C module we have to use 15 pins, by using I2C we just use 4 pins. We connect SCL pin to PB6 and SDA pin to PB7. The display have PCF8574 IC which convert I2C signal to parallel signal for LCD. I2C address is 0x27 . To display character, first we initialize the display with configuration commands. Then we set cursor position (row and column). Then we send ASCII code of character. The PCF8574 IC control LCD backlight and enable signal automatically.

**I2C Configuration:**

- PB6 → I2C1_SCL
- PB7 → I2C1_SDA
- Frequency: 100kHz
- Addressing Mode: 7-bit
- Dual Addressing: Disabled
- General Call: Disabled

**Code Example:**
```c
lcd_init();                    // Initialize with config
lcd_set_cursor(0, 0);          // Row 0, Column 0
lcd_print("Temp: ");
lcd_set_cursor(0, 6);
lcd_print("25.5°C");
```

---

## Accelerometer : LIS302DL (3-Axis)

The LIS302DL is an onboard accelerometer/motion sensor integrated on the STM32F407 Discovery Board. It is a 3-axis digital accelerometer that measures acceleration in three directions - X, Y, and Z axis. Each axis gives 8-bit signed value (±2g or ±8g configurable). We communicate via SPI1 and can detect tilt, motion, or free-fall.

**Specifications:**
- Chip: LIS302DL (3-Axis MEMS Accelerometer)
- Axes: 3 (X, Y, Z)
- Range: ±2g (18 mg/LSB) or ±8g (72 mg/LSB)
- Resolution: 8-bit per axis
- Output Data Rate: 100 Hz or 400 Hz
- Supply Voltage: 2.4V to 3.6V
- Operating Temperature: -40°C to +85°C

**SPI1 Configuration (IOC):**
- Clock Source: APB2 = 84 MHz
- Prescaler: 16 (84MHz → 5.25MHz)
- Mode: Master
- First Bit: MSB
- CPOL: Low
- CPHA: 1 Edge
- Data Size: 8 bits

**Pin Configuration (IOC):**
- PA5 → SPI1_SCK (Alternate Function AF5, Speed High, No pull)
- PA6 → SPI1_MISO (Alternate Function AF5, Speed High, No pull)
- PA7 → SPI1_MOSI (Alternate Function AF5, Speed High, No pull)
- PE3 → GPIO_Output CS (Output, Speed High, No pull, Initial High)

**Code Example:**
```c
ACC_Init();                              // Initialize SPI and chip
int8_t x = ACC_ReadAxis(LIS302DL_OUT_X); // Read X axis
int8_t y = ACC_ReadAxis(LIS302DL_OUT_Y); // Read Y axis
int8_t z = ACC_ReadAxis(LIS302DL_OUT_Z); // Read Z axis
// Acceleration value: at ±2g: output × 0.018g, at ±8g: output × 0.072g
```

---

## RC522 RFID Module

RFID reader is device which detect and read unique ID from RFID card or tag. It communicate using SPI protocol. The SCK pin to PB13, MISO pin to PB14, MOSI pin to PB15, CS pin to PD8, RST pin to PD9, and IRQ pin to PC6. RFID work at 13.56MHz frequency and can detect card within 10cm distance. To read card, first we send REQUEST command. If card detected, we get response. Then we send ANTICOLLISION command to read UID. Each RFID card have unique 5 byte ID (or 7 byte for some card). We extract UID from response data and store it for verification.

**Parameters:**
- Protocol: SPI2
- SCK Pin: PB13
- MISO Pin: PB14
- MOSI Pin: PB15
- CS Pin: PD8
- RST Pin: PD9 (GPIO Output)
- IRQ Pin: PC6 (GPIO Input)
- Clock Speed: 5.25MHz (42MHz / 8)
- Frequency: 13.56MHz (module internal)
- Detection Range: ~10cm
- UID Length: 4-7 bytes (usually 5 bytes)
- Card Types: Mifare Classic, Mifare Ultralight, etc.

**Pin Configuration (IOC):**
- PB13 → SPI2_SCK
  - Mode: Alternate Function (AF5)
  - Speed: High
  - Pull: No pull
- PB14 → SPI2_MISO
  - Mode: Alternate Function (AF5)
  - Speed: High
  - Pull: No pull
- PB15 → SPI2_MOSI
  - Mode: Alternate Function (AF5)
  - Speed: High
  - Pull: No pull
- PD8 → GPIO_Output (CS)
  - Mode: Output
  - Speed: High
  - Pull: No pull
  - Initial State: High
- PD9 → GPIO_Output (RST)
  - Mode: Output
  - Speed: High
  - Pull: No pull
  - Initial State: High
- PC6 → GPIO_Input (IRQ)
  - Mode: Input
  - Pull: Pull-down

**SPI Configuration (IOC):**
- Clock Source: APB1 = 42 MHz
- Mode: Master
- Prescaler: 8 (42MHz → 5.25MHz)
- First Bit: MSB
- CPOL: Low
- CPHA: 1 Edge

**Code Example:**
```c
MFRC522_Init();                    // Initialize SPI and RFID
if (MFRC522_Request() == MI_OK) {  // Check if card present
  uint8_t uid[5];
  if (MFRC522_Anticoll(uid) == MI_OK) {  // Read UID
    // Process UID: uid[0], uid[1], uid[2], uid[3], uid[4]
  }
}
```

---

## Clock Configuration Summary

**System Clock Source:** HSI internal oscillator 16MHz

**Clock Path:**
- HSI: 16MHz
- HSI divider: 16 (for PLL input stability)
- PLL input: 1MHz
- PLL multiply: ×336
- PLL output: 336MHz
- System clock divider: ÷2
- SYSCLK: 168MHz
- AHB prescaler: ÷1 (168MHz)
- APB1 prescaler: ÷4 (42MHz for SPI2, I2C, TIM2-7)
- APB2 prescaler: ÷2 (84MHz for SPI1, ADC, TIM1, TIM8-11)

**Timer Clock Calculation:**
- TIM2, TIM3, TIM4 are connected to APB1 (42MHz)
- With prescaler 84-1, clock = 42MHz / 84 = 500kHz... wait, let me recalculate
- Actually APB1 timer clock = 42MHz × 2 = 84MHz (auto multiply by 2 when prescaler > 1)
- With prescaler 84-1, clock = 84MHz / 84 = 1MHz ✓

---

## Initialization Sequence

1. `HAL_Init()` - Initialize HAL library
2. `SystemClock_Config()` - Configure PLL and clock dividers
3. `MX_GPIO_Init()` - Initialize all GPIO pins
4. `MX_ADC1_Init()` - Initialize ADC for temperature sensor
5. `MX_SPI1_Init()` - Initialize SPI1 for accelerometer
6. `MX_SPI2_Init()` - Initialize SPI2 for RFID
7. `MX_TIM2_Init()` - Initialize Timer 2 for buzzer PWM
8. `MX_TIM3_Init()` - Initialize Timer 3 for servo PWM
9. `MX_TIM4_Init()` - Initialize Timer 4 for fan PWM
10. `MX_I2C1_Init()` - Initialize I2C1 for LCD
11. `MFRC522_Init()` - Initialize RFID reader module
12. `ACC_Init()` - Initialize accelerometer chip
13. `lcd_init()` - Initialize LCD display with configuration

