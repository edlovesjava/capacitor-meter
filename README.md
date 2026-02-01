# Capacitance Meter with Button

A simple capacitance meter using an Arduino Nano and SSD1306 OLED display. Press a button to measure capacitance values from nanofarads to millifarads.

## Features

- One-button operation
- 128x64 OLED display with clear readings
- Auto-ranging display (pF, nF, uF, mF)
- Visual bar graph indicator
- Timeout protection for out-of-range values

## Wiring Diagram

![Wiring Diagram](Arduino%20NANO_oled_captest.drawio.png)

### Connections

| Component | Arduino Nano Pin |
|-----------|------------------|
| 10K Resistor | D13 |
| 220Ω Resistor | D12 |
| Analog Sense | A0 |
| Button | D2 (to GND) |
| OLED SDA | A4 |
| OLED SCL | A5 |

**Circuit:**
```
D13 ---- 10KΩ ----+---- A0 ---- Cap(+)
                  |
D12 ---- 220Ω ----+            Cap(-) ---- GND

D2 ---- [BUTTON] ---- GND

OLED: SDA -> A4, SCL -> A5
```

## Hardware Required

- Arduino Nano (ATmega328P)
- SSD1306 128x64 OLED display (I2C)
- 10KΩ resistor
- 220Ω resistor
- Momentary push button
- Breadboard and jumper wires

## Building & Uploading

This project supports both PlatformIO and Arduino IDE.

### Option 1: PlatformIO (Recommended)

**Using CLI:**
```bash
pio run              # Build
pio run -t upload    # Upload to board
pio device monitor   # Serial monitor
```

**Using VS Code:**
1. Install the PlatformIO extension
2. Open this folder
3. Click the PlatformIO icon in the sidebar
4. Select "Upload" under Project Tasks

### Option 2: Arduino IDE

1. Open `arduino/capacitance_meter_button/capacitance_meter_button.ino`
2. Install required libraries via **Sketch > Include Library > Manage Libraries**:
   - Adafruit GFX Library
   - Adafruit SSD1306
3. Select **Tools > Board > Arduino Nano**
4. Select **Tools > Processor > ATmega328P** (or "Old Bootloader" for clones)
5. Select your COM port under **Tools > Port**
6. Click Upload

## Usage

1. Power on the device
2. Insert the capacitor to measure (positive lead to A0 junction, negative to GND)
3. Press the button
4. Read the capacitance value on the display

## Measurement Range

- **Minimum:** ~10 nF
- **Maximum:** ~1000 uF
- Values outside this range will show "No cap or out of range"

## How It Works

### The Basic Idea

A capacitor is like a tiny rechargeable bucket for electricity. When you connect it to a power source through a resistor, it doesn't fill up instantly—it fills gradually, like pouring water through a funnel.

The key insight is: **bigger capacitors take longer to fill**. By measuring how long it takes to charge, we can calculate the capacitance.

### The Math

When charging a capacitor through a resistor, there's a simple relationship:

```
τ = R × C
```

Where:
- **τ** (tau) = the "time constant" in seconds
- **R** = resistance in ohms (Ω)
- **C** = capacitance in farads (F)

The time constant τ is how long it takes the capacitor to reach about 63.2% of full charge. This isn't an arbitrary number—it comes from the mathematical constant *e* (≈ 2.718), where 63.2% = 1 - 1/e.

### Solving for Capacitance

Since we know the resistor value (10,000Ω) and can measure the charge time, we rearrange the formula:

```
C = τ / R
```

The Arduino measures voltage using a 0-1023 scale (10-bit ADC). Full charge (5V) = 1023, so 63.2% = ~647. The code starts a timer, begins charging, and stops when the voltage reaches 647.

**Example calculation:**
- Measured charge time: 470,000 microseconds (0.47 seconds)
- Resistor: 10,000Ω
- Capacitance = 0.47 ÷ 10,000 = 0.000047 F = **47 µF**

### Why Two Resistors?

- **10KΩ resistor (D13):** Used for charging. A larger resistor means slower charging, which gives more accurate timing for small capacitors.
- **220Ω resistor (D12):** Used for discharging between measurements. A smaller resistor drains the capacitor quickly so it's ready for the next test.

## Project Structure

```
capacitance_meter_button/
├── src/                          # PlatformIO source
│   ├── main.cpp
│   ├── Display.h                 # Abstract display interface
│   ├── SSD1306Display.h/.cpp     # OLED implementation
│   └── CapacitanceMeter.h/.cpp   # Measurement logic
├── arduino/                      # Arduino IDE version
│   └── capacitance_meter_button/
│       ├── capacitance_meter_button.ino
│       └── *.h/*.cpp             # Same classes
└── platformio.ini
```

The code uses an object-oriented design with a `Display` interface, making it easy to add support for different display types (LCD, TFT, etc.) by implementing a new display class.

## License

MIT
