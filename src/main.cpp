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
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CHARGE_PIN    13
#define DISCHARGE_PIN 12
#define ANALOG_PIN    A0
#define BUTTON_PIN    2

#define RESISTOR_OHMS 10000.0
#define THRESHOLD     647
#define TIMEOUT_US    5000000UL

// Function prototypes
void showReady();
void showMeasuring();
void showCapacitance(float cap_uF);
void showError();
float measureCapacitance();

void setup() {
  Serial.begin(115200);

  pinMode(CHARGE_PIN, INPUT);
  pinMode(DISCHARGE_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button to GND, no external resistor needed

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }

  showReady();
}

void loop() {
  // Wait for button press
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);  // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      showMeasuring();
      float cap = measureCapacitance();

      if (cap > 0) {
        showCapacitance(cap);
      } else {
        showError();
      }

      // Wait for button release
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
      delay(50);  // Debounce release
    }
  }
}

float measureCapacitance() {
  // Discharge
  pinMode(CHARGE_PIN, INPUT);
  pinMode(DISCHARGE_PIN, OUTPUT);
  digitalWrite(DISCHARGE_PIN, LOW);

  unsigned long startDis = millis();
  while (analogRead(ANALOG_PIN) > 10 && millis() - startDis < 5000);
  pinMode(DISCHARGE_PIN, INPUT);
  delay(50);

  // Charge and measure
  unsigned long startTime = micros();
  pinMode(CHARGE_PIN, OUTPUT);
  digitalWrite(CHARGE_PIN, HIGH);

  while (analogRead(ANALOG_PIN) < THRESHOLD) {
    if (micros() - startTime > TIMEOUT_US) {
      digitalWrite(CHARGE_PIN, LOW);
      pinMode(CHARGE_PIN, INPUT);
      return -1;  // Timeout
    }
  }

  unsigned long chargeTime = micros() - startTime;
  digitalWrite(CHARGE_PIN, LOW);
  pinMode(CHARGE_PIN, INPUT);

  if (chargeTime < 200) {
    return -1;  // Too small / no cap
  }

  return (float)chargeTime / RESISTOR_OHMS;
}

void showReady() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(5, 5);
  display.println(F("Capacitor"));
  display.setCursor(30, 25);
  display.println(F("Meter"));

  display.setTextSize(1);
  display.setCursor(5, 50);
  display.println(F("Press button to test"));

  display.display();
}

void showMeasuring() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 25);
  display.println(F("Measuring"));
  display.setCursor(40, 45);
  display.println(F("..."));
  display.display();
}

void showCapacitance(float cap_uF) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("CAPACITANCE"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setTextSize(3);
  display.setCursor(0, 18);

  if (cap_uF >= 1000) {
    display.print(cap_uF / 1000.0, 1);
    display.setTextSize(2);
    display.print(F(" mF"));
  } else if (cap_uF >= 1) {
    display.print(cap_uF, 1);
    display.setTextSize(2);
    display.print(F(" uF"));
  } else if (cap_uF >= 0.001) {
    display.print(cap_uF * 1000.0, 0);
    display.setTextSize(2);
    display.print(F(" nF"));
  } else {
    display.print(cap_uF * 1000000.0, 0);
    display.setTextSize(2);
    display.print(F(" pF"));
  }

  // Bar graph
  display.setTextSize(1);
  display.setCursor(0, 54);
  display.print(F("10nF"));
  display.setCursor(98, 54);
  display.print(F("1mF"));

  float logVal = log10(cap_uF);
  int barW = constrain(map(logVal * 100, -200, 300, 0, 124), 0, 124);
  display.drawRect(2, 46, 124, 6, SSD1306_WHITE);
  display.fillRect(2, 46, barW, 6, SSD1306_WHITE);

  display.display();
}

void showError() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("CAPACITANCE"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println(F("No cap or"));
  display.setCursor(10, 38);
  display.println(F("out of range"));

  display.setTextSize(1);
  display.setCursor(5, 56);
  display.println(F("Press button to retry"));

  display.display();
}
