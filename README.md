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

This is a PlatformIO project.

### Using PlatformIO CLI

```bash
# Build
pio run

# Upload to board
pio run -t upload

# Open serial monitor
pio device monitor
```

### Using VS Code

1. Install the PlatformIO extension
2. Open this folder
3. Click the PlatformIO icon in the sidebar
4. Select "Upload" under Project Tasks

## Usage

1. Power on the device
2. Insert the capacitor to measure (positive lead to A0 junction, negative to GND)
3. Press the button
4. Read the capacitance value on the display

## Measurement Range

- **Minimum:** ~10 nF
- **Maximum:** ~1000 uF
- Values outside this range will show "No cap or out of range"

## License

MIT
