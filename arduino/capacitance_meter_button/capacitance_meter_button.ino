/*
 * Capacitance Meter with Button
 * Arduino Nano + SSD1306 OLED
 *
 * Wiring:
 *   D13 ---- 10KΩ ----+---- A0 ---- Cap(+)
 *                     |
 *   D12 ---- 220Ω ----+            Cap(-) ---- GND
 *
 *   D2 ---- [BUTTON] ---- GND
 *
 *   OLED: SDA -> A4, SCL -> A5
 *
 * Press button to measure. Insert cap, press button, read value.
 *
 * Required Libraries (install via Library Manager):
 *   - Adafruit GFX Library
 *   - Adafruit SSD1306
 */

#include <Wire.h>
#include "CapacitanceMeter.h"
#include "SSD1306Display.h"

// Pin definitions
const uint8_t CHARGE_PIN    = 13;
const uint8_t DISCHARGE_PIN = 12;
const uint8_t ANALOG_PIN    = A0;
const uint8_t BUTTON_PIN    = 2;

// Hardware instances
CapacitanceMeter meter(CHARGE_PIN, DISCHARGE_PIN, ANALOG_PIN);
SSD1306Display display;

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    meter.begin();

    if (!display.begin()) {
        // Display init failed - halt
        for (;;);
    }

    display.showReady();
}

void loop() {
    // Wait for button press
    if (digitalRead(BUTTON_PIN) == LOW) {
        delay(50);  // Debounce

        if (digitalRead(BUTTON_PIN) == LOW) {
            display.showMeasuring();

            float capacitance = meter.measure();

            if (capacitance > 0) {
                display.showCapacitance(capacitance);
            } else {
                display.showError();
            }

            // Wait for button release
            while (digitalRead(BUTTON_PIN) == LOW) {
                delay(10);
            }
            delay(50);  // Debounce release
        }
    }
}
