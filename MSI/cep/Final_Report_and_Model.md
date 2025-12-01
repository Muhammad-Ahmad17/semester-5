# **Final Report & System Model**

## **Project Title**
- **Smart Room Automation Module (Single-Room) — STM32F407**

## **Abstract**
- **Goal:** Build a beginner-friendly, deployable smart room controller using bare-metal C on STM32F407 that provides RFID-based access control, temperature-based fan control, lighting automation, door safety, and earthquake/vibration alerting.
- **Scope:** Single-room prototype (cardboard demo → production enclosure). No RTOS; simple super-loop + interrupts. Uses a 5V DC fan for simplified power.

## **Key Objectives**
- **RFID Access Control:** Toggle entry/exit with EM18 reader (<1s response).
- **Lighting Automation:** Lights on when occupied, off when empty.
- **Temperature-Based Fan Control:** LM35 temperature sensing; fan ON when temp ≥ 28°C and only when occupied.
- **Earthquake Alert:** Onboard LIS3DSH interrupt triggers buzzer + LED (<100ms).
- **Door Safety:** IR obstacle sensor prevents locking when obstruction detected.

## **One-line Design Summary**
- Bare-metal STM32F407 reads LM35 (ADC), EM18 (UART), IR sensor (GPIO), uses servo (PWM) and direct GPIO to control a 5V fan and relay for lights; accelerometer interrupt for alarms.

## **Bill of Materials (Final)**
- **STM32F407VGT6 Discovery Board** — main controller — $15
- **LM35DZ** — temperature sensor (analog, 10mV/°C) — $2
- **EM18 RFID Reader** — 125kHz UART reader — $4
- **EM4100 RFID Cards (5x)** — cards — $2
- **IR Obstacle Sensor (FC-51 or similar)** — GPIO — $2
- **SG90 Micro Servo** — door lock — $3
- **5V DC Fan (80×80×25, ~0.5A)** — cooling fan — $4
- **1-channel 5V Relay Module** — light switching (10A) — $2
- **16×2 LCD (HD44780, 4-bit)** — UI — $3
- **Piezo Buzzer (active, 5V)** — alarm — $1
- **5V 2A Power Adapter (barrel jack)** — main power — $4
- **LM1117-3.3V Regulator (if needed)** — for 3.3V logic — $1
- **Jumpers, breadboard, connectors, enclosure materials** — $6
- **Estimated Total:** ~$49–55

## **Pin Allocation (STM32F407)**
- **PA0 (ADC1_CH0)** — LM35 Vout (ADC)
- **PA3 (USART2_RX)** — EM18 UART TX → MCU RX
- **PC0** — 5V Fan control (GPIO output) — direct or via MOSFET for full 5V
- **PC1** — Relay IN (lights)
- **PA6 / TIM3_CH1** — Servo PWM (SG90)
- **PB0** — IR obstacle sensor (GPIO input)
- **Onboard LIS3DSH INT** — External interrupt pin (configured via Discovery board mapping)
- **I2C (optional)** — PB8/PB9 for OLED (if used)

## **System Architecture (ASCII)**

```
                              +----------------+
                              | 5V 2A Adapter  |
                              +--------+-------+
                                       |
                        +--------------+---------------+
                        |                              |
                   +----v----+                   +-----v-----+
                   | 5V Rail |                   | 3.3V Reg   |
                   +----+----+                   +-----+-----+
                        |                              |
     +------------------+--+            +--------------+--+
     |                     |            |                 |
 +---v---+  +--------------v----+   +----v----+     +------v-----+
 |5V Fan |  | 1-Channel Relay    |   | STM32   |     |  LM1117-3.3|
 |(80mm) |  | (Light Switch)     |   |F407     |     | (Optional)  |
 +---+---+  +--------------------+   +----+----+     +------------+
     |                                     | 
     |                                     | 
  (GND)                                 (3.3V)
                                         |
                       +-----------------+---------------------+
                       |                 |                     |
                 +-----v----+      +-----v-----+         +-----v-----+
                 | LM35 ADC |      | EM18 UART |         | SG90 PWM  |
                 | PA0      |      | PA3 RX    |         | PA6/TIM3  |
                 +----------+      +-----------+         +-----------+
```

## **Wiring Summary**
- LM35: VCC → 3.3V, Vout → PA0 (ADC1_CH0), GND → GND.
- EM18: VCC → 5V, TX → PA3 (USART2_RX), GND → GND.
- IR sensor: VCC → 5V, OUT → PB0 (GPIO in), GND → GND.
- Servo (SG90): VCC → 5V, Signal → PA6 (TIM3_CH1 PWM), GND → GND.
- Fan: Red → PC0 (GPIO) or MOSFET driver for full 5V; Black → GND. If using direct GPIO, fan will run at ~3.3V speed; preferred: use N-channel MOSFET (e.g., IRLZ44N/logic-level) with gate at PC0 and source GND, drain to fan negative, fan positive to 5V.
- Relay IN: PC1 → Relay IN (lights), Relay COM/NO wired in series with room light live.
- Buzzer: VCC → 5V, IN → any GPIO, GND → GND.

## **Control Logic (Concise)**

- Main loop runs every 100ms. Critical events handled by interrupts (accelerometer, RFID data available).

- LM35 reading → ADC → voltage → temp (°C):

  temp = ((ADC_value / 4096.0) * 3.3) * 100.0;

- Filtering: temp_avg = 0.9 * temp_avg + 0.1 * new_temp;

- Fan decision (only when room_occupied):

  - If temp_avg >= 28.0°C → fan ON (PC0 HIGH / MOSFET gate HIGH)
  - If temp_avg <= 27.5°C → fan OFF (hysteresis)

- RFID EM18: each valid card toggles room_occupied. On entry: servo unlock, lights ON. On exit: lights OFF, fan OFF, servo unlock.

- IR obstacle: prevents servo locking while PB0 indicates obstruction.

- Accelerometer interrupt: immediate buzzer & LED alert; requires manual silence.

## **Operation Sequences**
- **Entry:** Present authorized card → EM18 sends UID → MCU verifies → IR clear? → Servo unlock 5s → Lights ON → Set room_occupied = true.
- **During Occupancy:** Every 100ms read LM35 → update temp_avg → control fan based on threshold and occupancy → update LCD.
- **Exit:** Present card → verify → Lights OFF → Fan OFF → Servo unlock 5s → Set room_occupied = false.

## **Prototype Model Diagram (Cardboard Demo)**

```
Top view (40cm × 40cm cardboard demo)

 -------------------------------------------------
| OLED  [LM35]            [LED Strip]   [Fan]    |
|                                             [] |
|                                             [] |
|  [STM32 Board]  [Relay]  [EM18 RFID]  [IR]  [Door]
|                                             [] |
|                                             [] |
 -------------------------------------------------

Demonstration steps: boot → show sensors → heat LM35 → fan on → cover LDR (if present) → lights on → tap board → alarm → present RFID → entry/exit flow.
```

## **Testing & Validation**
- **Sensor tests:** Validate LM35 linearity against reference thermometer (±1°C target).
- **RFID:** 10 read attempts per card with >99% success rate.
- **Fan control:** Verify hysteresis prevents chattering around 28°C.
- **Accelerometer:** Simulate vibration to trigger interrupt; measure latency (<100ms).
- **Safety:** IR must prevent servo operation when obstruction present.

## **Timeline (12 weeks)**
- Weeks 1–2: Procurement & hardware verification.
- Weeks 3–4: Core firmware (ADC, UART, PWM drivers).
- Weeks 5–6: Integration (access control, safety checks).
- Weeks 7–8: Validation & 48-hour stability test.
- Weeks 9–10: Production prototype & enclosure.
- Weeks 11–12: Documentation, report, final demo.

## **Deliverables (Single File Package)**
- Source code (STM32CubeIDE or Makefile project)
- Circuit schematic + wiring diagram
- Breadboard demo photos & video
- This final report & model (`Final_Report_and_Model.md`)

## **Notes & Recommendations**
- For reliable full-speed fan control use a logic-level N-MOSFET instead of direct GPIO. Connect MOSFET gate to PC0 via 100Ω, put a 10k pull-down on gate, and a flyback diode if using brushed motors (not necessary for brushless fans).
- EM18 is simple UART-based reader—place at door frame for reliable read range. Keep RX line idle-safe and add a small RC filter if noisy.
- Power: if additional peripherals added (OLED, many LEDs), use 5V 2A; otherwise 1A may suffice. Use a separate 3.3V regulator for STM32 if powering from the 5V adapter.

## **Next Steps**
- Procure listed parts and assemble the breadboard prototype. Test each subsystem independently, then integrate following operation sequences above. Record test logs for the final report.

---
*Prepared by: Project Team*  
*Date: November 30, 2025*
