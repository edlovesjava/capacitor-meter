#ifndef CAPACITANCE_METER_H
#define CAPACITANCE_METER_H

#include <Arduino.h>

/**
 * Capacitance measurement using RC time constant method.
 *
 * Measures capacitance by timing how long it takes to charge
 * a capacitor through a known resistance to 63.2% (one time constant).
 */
class CapacitanceMeter {
public:
    /**
     * @param chargePin     Pin connected to charge resistor (10K)
     * @param dischargePin  Pin connected to discharge resistor (220 ohm)
     * @param analogPin     Analog pin for voltage measurement
     * @param resistorOhms  Value of charge resistor in ohms
     */
    CapacitanceMeter(uint8_t chargePin, uint8_t dischargePin,
                     uint8_t analogPin, float resistorOhms = 10000.0);

    /**
     * Initialize pin modes.
     */
    void begin();

    /**
     * Measure the capacitance.
     * @return Capacitance in microfarads, or -1 if timeout/no capacitor
     */
    float measure();

    /**
     * Set the ADC threshold for 63.2% charge detection.
     * Default is 647 (63.2% of 1023 for 10-bit ADC).
     */
    void setThreshold(int threshold);

    /**
     * Set the measurement timeout in microseconds.
     * Default is 5,000,000 (5 seconds).
     */
    void setTimeoutMicros(unsigned long timeout);

private:
    void discharge();

    uint8_t _chargePin;
    uint8_t _dischargePin;
    uint8_t _analogPin;
    float _resistorOhms;
    int _threshold;
    unsigned long _timeoutUs;
};

#endif
