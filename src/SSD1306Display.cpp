#include "SSD1306Display.h"
#include <math.h>

SSD1306Display::SSD1306Display(uint8_t width, uint8_t height,
                               uint8_t address, int8_t resetPin)
    : _display(width, height, &Wire, resetPin)
    , _address(address)
{
}

bool SSD1306Display::begin() {
    return _display.begin(SSD1306_SWITCHCAPVCC, _address);
}

void SSD1306Display::showReady() {
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);

    _display.setTextSize(2);
    _display.setCursor(5, 5);
    _display.println(F("Capacitor"));
    _display.setCursor(30, 25);
    _display.println(F("Meter"));

    _display.setTextSize(1);
    _display.setCursor(5, 50);
    _display.println(F("Press button to test"));

    _display.display();
}

void SSD1306Display::showMeasuring() {
    _display.clearDisplay();
    _display.setTextSize(2);
    _display.setCursor(10, 25);
    _display.println(F("Measuring"));
    _display.setCursor(40, 45);
    _display.println(F("..."));
    _display.display();
}

void SSD1306Display::showCapacitance(float cap_uF) {
    _display.clearDisplay();

    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println(F("CAPACITANCE"));
    _display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    _display.setTextSize(3);
    _display.setCursor(0, 18);

    if (cap_uF >= 1000) {
        _display.print(cap_uF / 1000.0, 1);
        _display.setTextSize(2);
        _display.print(F(" mF"));
    } else if (cap_uF >= 1) {
        _display.print(cap_uF, 1);
        _display.setTextSize(2);
        _display.print(F(" uF"));
    } else if (cap_uF >= 0.001) {
        _display.print(cap_uF * 1000.0, 0);
        _display.setTextSize(2);
        _display.print(F(" nF"));
    } else {
        _display.print(cap_uF * 1000000.0, 0);
        _display.setTextSize(2);
        _display.print(F(" pF"));
    }

    // Bar graph
    _display.setTextSize(1);
    _display.setCursor(0, 54);
    _display.print(F("10nF"));
    _display.setCursor(98, 54);
    _display.print(F("1mF"));

    float logVal = log10(cap_uF);
    int barW = constrain(map(logVal * 100, -200, 300, 0, 124), 0, 124);
    _display.drawRect(2, 46, 124, 6, SSD1306_WHITE);
    _display.fillRect(2, 46, barW, 6, SSD1306_WHITE);

    _display.display();
}

void SSD1306Display::showError() {
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println(F("CAPACITANCE"));
    _display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    _display.setTextSize(2);
    _display.setCursor(10, 20);
    _display.println(F("No cap or"));
    _display.setCursor(10, 38);
    _display.println(F("out of range"));

    _display.setTextSize(1);
    _display.setCursor(5, 56);
    _display.println(F("Press button to retry"));

    _display.display();
}
