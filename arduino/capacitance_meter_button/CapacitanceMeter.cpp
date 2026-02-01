#include "CapacitanceMeter.h"

CapacitanceMeter::CapacitanceMeter(uint8_t chargePin, uint8_t dischargePin,
                                   uint8_t analogPin, float resistorOhms)
    : _chargePin(chargePin)
    , _dischargePin(dischargePin)
    , _analogPin(analogPin)
    , _resistorOhms(resistorOhms)
    , _threshold(647)           // 63.2% of 1023
    , _timeoutUs(5000000UL)     // 5 seconds
{
}

void CapacitanceMeter::begin() {
    pinMode(_chargePin, INPUT);
    pinMode(_dischargePin, INPUT);
}

void CapacitanceMeter::setThreshold(int threshold) {
    _threshold = threshold;
}

void CapacitanceMeter::setTimeoutMicros(unsigned long timeout) {
    _timeoutUs = timeout;
}

void CapacitanceMeter::discharge() {
    pinMode(_chargePin, INPUT);
    pinMode(_dischargePin, OUTPUT);
    digitalWrite(_dischargePin, LOW);

    unsigned long startTime = millis();
    while (analogRead(_analogPin) > 10 && millis() - startTime < 5000) {
        // Wait for discharge
    }

    pinMode(_dischargePin, INPUT);
    delay(50);
}

float CapacitanceMeter::measure() {
    // First, discharge the capacitor
    discharge();

    // Start charging and measure time to reach threshold
    unsigned long startTime = micros();
    pinMode(_chargePin, OUTPUT);
    digitalWrite(_chargePin, HIGH);

    while (analogRead(_analogPin) < _threshold) {
        if (micros() - startTime > _timeoutUs) {
            // Timeout - capacitor too large or not connected
            digitalWrite(_chargePin, LOW);
            pinMode(_chargePin, INPUT);
            return -1;
        }
    }

    unsigned long chargeTime = micros() - startTime;

    // Stop charging
    digitalWrite(_chargePin, LOW);
    pinMode(_chargePin, INPUT);

    // Filter out noise / no capacitor
    if (chargeTime < 200) {
        return -1;
    }

    // C = τ / R, where τ is in microseconds and R is in ohms
    // Result is in microfarads
    return (float)chargeTime / _resistorOhms;
}
