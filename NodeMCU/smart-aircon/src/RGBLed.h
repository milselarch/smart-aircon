#include <Arduino.h>

class rgbLed {
    unsigned int red = 0, green = 0, blue = 0;
    unsigned int redPin, greenPin, bluePin;

    public:
    rgbLed(
        unsigned int redPin, unsigned int greenPin,
        unsigned int bluePin
    ) {
        this->redPin = redPin;
        this->greenPin = greenPin;
        this->bluePin = bluePin;

        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);
    }

    void write() {
        this->write(this->red, this->green, this->blue, false);
    }

    void write(int value) {
        this->write(value, value, value, false);
    }

    void write(unsigned int r, unsigned int g, unsigned int b) {
        this->write(r, g, b, false);
    }

    void write(unsigned int r, unsigned int g, unsigned int b, bool save) {
        if (save == true) {
            this->red = r;
            this->green = g;
            this->blue = b;
        }

        digitalWrite(this->redPin, r);
        digitalWrite(this->greenPin, g);
        digitalWrite(this->bluePin, b);
    }

    unsigned int getRed() { return this->red; }
    unsigned int getGreen() { return this->green; }
    unsigned int getBlue() { return this->blue; }
};