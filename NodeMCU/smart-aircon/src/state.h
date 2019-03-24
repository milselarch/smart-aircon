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

    unsigned int pirPin;
    unsigned int targetTemp = 25;
    unsigned int targetFanspeed = 1;
    float maxBackgroundTemp = 28;

    float temp = -1;
    bool currentlyOn = false;
    bool peopleDetected = false;
    bool needsFlush = true;
    int state = ON_STATE;

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
    bool getPeopleDetected() { return this->peopleDetected; }
    int getState() { return this->state; };

    bool shouldBeOn() {
        return (
            (this->state == ON_STATE) || (
                (this->state == AUTO_STATE) &&
                (this->temp >= this->maxBackgroundTemp) &&
                this->peopleDetected
            )
        );
    }

    float updateTemp() {
        this->sensors->requestTemperatures();
        delay(10);
        this->temp = this->sensors->getTempCByIndex(0);
        return this->temp;
    };

    bool updatePIR() {
        this->peopleDetected = digitalRead(this->pirPin);
    }

    bool setFanspeed (unsigned int fanspeed) {
        bool pastShouldBeOn = this->shouldBeOn();
        this->targetFanspeed = fanspeed;

        bool needsFlush = (this->shouldBeOn() != pastShouldBeOn);
        if (needsFlush) { this->needsFlush = true; }
        return needsFlush;
    }

    bool setPeopleDetected(bool peopleDetected) {
        bool pastShouldBeOn = this->shouldBeOn();
        this->peopleDetected = peopleDetected;
        bool needsFlush = (this->shouldBeOn() != pastShouldBeOn);
        if (needsFlush) { this->needsFlush = true; }
        return needsFlush;
    }

    bool setTemp(unsigned int temp) {
        bool pastShouldBeOn = this->shouldBeOn();
        this->targetTemp = temp;

        bool needsFlush = (this->shouldBeOn() != pastShouldBeOn);
        if (needsFlush) { this->needsFlush = true; }
        return needsFlush;
    }

    bool setMaxTemp(float maxTemp) {
        bool pastShouldBeOn = this->shouldBeOn();
        this->maxBackgroundTemp = temp;

        bool needsFlush = (this->shouldBeOn() != pastShouldBeOn);
        if (needsFlush) { this->needsFlush = true; }
        return needsFlush;
    }

    bool setState(int state) {
        bool pastShouldBeOn = this->shouldBeOn();
        this->state = state;

        bool needsFlush = (this->shouldBeOn() != pastShouldBeOn);
        if (needsFlush) { this->needsFlush = true; }
        return needsFlush;
    } 

    int update() {
        this->updatePIR();
        this->updateTemp();
        bool onState = this->shouldBeOn();

        if (this->needsFlush) {
            this->sender->sendCommand(
                onState, this->targetTemp, this->targetFanspeed
            );

            return onState;
        } else {
            return -1;
        }
    }
}