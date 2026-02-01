#ifndef SSD1306_DISPLAY_H
#define SSD1306_DISPLAY_H

#include "Display.h"
#include <Adafruit_SSD1306.h>

/**
 * SSD1306 OLED display implementation (128x64, I2C).
 */
class SSD1306Display : public Display {
public:
    SSD1306Display(uint8_t width = 128, uint8_t height = 64,
                   uint8_t address = 0x3C, int8_t resetPin = -1);

    bool begin() override;
    void showReady() override;
    void showMeasuring() override;
    void showCapacitance(float capacitance_uF) override;
    void showError() override;

private:
    Adafruit_SSD1306 _display;
    uint8_t _address;
};

#endif
