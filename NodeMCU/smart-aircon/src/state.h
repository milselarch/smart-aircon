#include <OneWire.h>
#include <DallasTemperature.h>
#include "sender.h"

#define ON_STATE 1
#define OFF_STATE 0
#define AUTO_STATE 2
#define AUTO_ON_STATE 3
#define AUTO_OFF_STATE 4

class AirconState {
    DallasTemperature *sensors;
    PulseSender *sender;
    OneWire *onewire;

    const unsigned int DETECT_DELAY = 10 * 1000;
    const unsigned int TEMP_DELAY = 240 * 1000;

    unsigned int pirPin;
    unsigned int targetTemp = 25;
    unsigned int targetFanspeed = 1;
    unsigned int lastTempUpdate = -1;
    float maxBackgroundTemp = 28;

    float temp = -1;
    bool currentlyOn = false;
    bool initialPulsed = false;
    unsigned long lastPeopleDetected = -1;
    bool needsFlush = true;
    int state = ON_STATE;

    bool pastShouldBeOn;
    bool detected;
    bool onState;

    public:
    AirconState (
        unsigned int wirePin, unsigned int irPin,
        unsigned int pirPin
    ) {
        this->onewire = new OneWire(wirePin);
        this->sensors = new DallasTemperature(this->onewire);
        this->sender = new PulseSender(irPin);
        this->state = OFF_STATE;
        this->pirPin = pirPin;

        pinMode(pirPin, INPUT);
        this->sensors->begin();
        this->updateTemp();
        this->updatePIR();
    };

    float getTemp() { return this->temp; };
    int getTargetTemp() { return this->targetTemp; }
    int getFanspeed() { return this->targetFanspeed; }
    float getMaxBackgroundTemp() { return this->maxBackgroundTemp; }
    int getState() { return this->state; };
    unsigned long detectDelay() { return millis() - this->lastPeopleDetected; }
    bool getPeopleDetected() { 
        return this->detectDelay() < this->DETECT_DELAY;
    }

    bool shouldBeOn() {
        return (
            (this->state == ON_STATE) || (
                (this->state == AUTO_STATE) &&
                (this->temp >= this->maxBackgroundTemp) &&
                this->getPeopleDetected()
            )
        );
    }

    float updateTemp() {
        this->sensors->requestTemperatures();
        this->temp = this->sensors->getTempCByIndex(0);
        return this->temp;
    };

    bool updatePIR() {
        detected = digitalRead(this->pirPin);
        if (detected) {
            this->lastPeopleDetected = millis();
        };
        
        return detected;
    }

    bool setFanspeed (unsigned int fanspeed) {
        pastShouldBeOn = this->shouldBeOn();
        this->targetFanspeed = fanspeed;
        if (this->shouldBeOn() != pastShouldBeOn) {
            this->needsFlush = true;
        }
        return needsFlush;
    }

    bool setTemp(unsigned int temp) {
        pastShouldBeOn = this->shouldBeOn();
        this->targetTemp = temp;
        if (this->shouldBeOn() != pastShouldBeOn) {
            this->needsFlush = true;
        }
        return needsFlush;
    }

    bool setMaxTemp(float maxTemp) {
        pastShouldBeOn = this->shouldBeOn();
        this->maxBackgroundTemp = temp;
        if (this->shouldBeOn() != pastShouldBeOn) {
            this->needsFlush = true;
        }
        return needsFlush;
    }

    bool setState(int state) {
        pastShouldBeOn = this->shouldBeOn();
        this->state = state;
        if (this->shouldBeOn() != pastShouldBeOn) {
            this->needsFlush = true;
        }

        return needsFlush;
    } 

    int update(rgbLed & RGB) {
        this->updatePIR();
        if (millis() - this->lastTempUpdate > this->TEMP_DELAY) {
            Serial.println("RGB ALL ON");
            RGB.write(50);
            this->lastTempUpdate = millis();
            this->updateTemp();
            RGB.write();
            Serial.println("RGB ALL OFF");
        }

        onState = this->shouldBeOn();
        if (this->needsFlush) {
            this->needsFlush = false;
            this->currentlyOn = onState;
            this->initialPulsed = true;
            this->sender->sendCommand(
                onState, this->targetTemp, this->targetFanspeed
            );

            return onState;
        } else {
            return -1;
        }
    }
}