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

    void write(unsigned int r, unsigned int g, unsigned int b) {
        this->red = r;
        this->green = g;
        this->blue = b;

        analogWrite(this->redPin, r);
        analogWrite(this->greenPin, g);
        analogWrite(this->bluePin, b);
    }
}