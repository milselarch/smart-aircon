#include <Arduino.h>

class ClapDetector {
    unsigned int sensorPin;
    unsigned int lastSoundValue;
    unsigned int soundValue;
    unsigned int currentNoiseTime;
    unsigned int lastNoiseTime;
    unsigned int lastLightChange;
    bool clapDetected = false;

    public:
    ClapDetector(unsigned int sensorPin) {
        this->sensorPin = sensorPin;
        pinMode(sensorPin, INPUT);
    }

    unsigned int getSoundValue() {
        return this->soundValue;
    }

    void update() {
        this->soundValue = analogRead(this->sensorPin);
        this->currentNoiseTime = millis();

        if (this->soundValue > 768) {
            // if there is currently a noise
            if (
                (this->currentNoiseTime > this->lastNoiseTime + 200) && 
                // to debounce a sound occurring in more than a loop cycle as a single noise
                (this->lastSoundValue < 256) &&  
                // if it was silent before
                (this->currentNoiseTime < this->lastNoiseTime + 800) && 
                // if current clap is less than 0.8 seconds after the first clap
                (this->currentNoiseTime > this->lastLightChange + 1000)
                // to avoid taking a third clap as part of a pattern
            ) {
                this->clapDetected = true;
                this->lastLightChange = this->currentNoiseTime;
            }

            this->lastNoiseTime = this->currentNoiseTime;
        }

        this->lastSoundValue = this->soundValue;
    }

    bool isClapDetected() {
        return this->clapDetected;
    }

    void resetClapDetection() {
        this->clapDetected = false;
    }
};