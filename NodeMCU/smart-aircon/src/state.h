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
    const unsigned int TEMP_DELAY = 60 * 1000;

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
        return this->shouldBeOn(false);
    }

    bool shouldBeOn(bool detectPeople) {
        return (
            (this->state == ON_STATE) || (
                (this->state == AUTO_STATE) &&
                (this->temp >= this->maxBackgroundTemp) &&
                (!detectPeople || this->getPeopleDetected())
            )
        );
    }

    float updateTemp() {
        pastShouldBeOn = this->shouldBeOn();
        this->sensors->requestTemperatures();
        this->temp = this->sensors->getTempCByIndex(0);
        if (this->shouldBeOn() != pastShouldBeOn) {
            this->needsFlush = true;
        }

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
        return this->needsFlush;
    }

    bool setMaxTemp(float maxTemp) {
        Serial.print("MAX-TEMP ");
        Serial.print(maxTemp);
        Serial.print(" ");
        Serial.println(this->temp);

        this->pastShouldBeOn = this->shouldBeOn();
        this->maxBackgroundTemp = maxTemp;
        if (this->shouldBeOn() != this->pastShouldBeOn) {
            Serial.println("NEED MAX FLUSH");
            this->needsFlush = true;
        } else {
            Serial.println("NO NEED MAX FLUSH");
        }

        Serial.println(this->state == ON_STATE);
        Serial.println(this->state == AUTO_STATE);
        Serial.println(this->temp >= this->maxBackgroundTemp);

        if (this->shouldBeOn()) {
            Serial.println("SHOULD BE ON");
        } else {
            Serial.println("SHOULD BE OFF");
        }

        return this->needsFlush;
    }

    bool setState(int state) {
        pastShouldBeOn = this->shouldBeOn();
        this->state = state;
        if (this->shouldBeOn() != pastShouldBeOn) {
            this->needsFlush = true;
        }

        return this->needsFlush;
    }

    int update(rgbLed & RGB) {
        return this->update(RGB, true);
    }

    int update(rgbLed & RGB, bool updatePIR) {
        if (updatePIR == true) {
            this->updatePIR();
        }

        if (
            (millis() - this->lastTempUpdate > this->TEMP_DELAY) ||
            (this->lastTempUpdate == -1)
        ) {
            Serial.println("RGB ALL ON");
            RGB.write(50);
            this->lastTempUpdate = millis();
            this->updateTemp();
            RGB.write();
            Serial.println("RGB ALL OFF");
        }

        // if updatePIR is true, detect people should be true
        onState = this->shouldBeOn();
        if (this->needsFlush) {
            this->needsFlush = false;
            this->currentlyOn = onState;
            this->initialPulsed = true;
            Serial.print("SEND ");
            Serial.print(onState);
            this->sender->sendCommand(
                onState, this->targetTemp, this->targetFanspeed
            );

            return onState;
        } else {
            return -1;
        }
    }

    void forceSend() {
        this->needsFlush = true;
    }
}
;