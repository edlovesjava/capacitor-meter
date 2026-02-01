#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * Abstract display interface for capacitance meter.
 * Implement this class to support different display types.
 */
class Display {
public:
    virtual ~Display() = default;

    /**
     * Initialize the display hardware.
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool begin() = 0;

    /**
     * Show the ready/welcome screen.
     */
    virtual void showReady() = 0;

    /**
     * Show that a measurement is in progress.
     */
    virtual void showMeasuring() = 0;

    /**
     * Show the measured capacitance value.
     * @param capacitance_uF Capacitance in microfarads
     */
    virtual void showCapacitance(float capacitance_uF) = 0;

    /**
     * Show an error (no capacitor or out of range).
     */
    virtual void showError() = 0;
};

#endif
