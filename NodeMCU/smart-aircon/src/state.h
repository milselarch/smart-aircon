#include <OneWire.h>
#include <DallasTemperature.h>

#define ON_STATE 1
#define OFF_STATE 0
#define AUTO_ON_STATE 2
#define AUTO_OFF_STATE 3

class AirconState {
    DallasTemperature sensors;
    OneWire onewire;
    unsigned int temp;
    int state;

    AirconState (unsigned int wirePin) {
        this->oneWire = new OneWire(wirePin);
        this->sensors = new DallasTemperature(&oneWire);
        this->state = OFF_STATE;
        this->sensors.begin();
    }

    float getTemp() {
        sensors.requestTemperatures();
        delay(10);
        return sensors.getTempCByIndex(0)
    }
}