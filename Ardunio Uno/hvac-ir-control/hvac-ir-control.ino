#include "HVAC_LIB_ESP8266.h";

HvacMitsubishi AirController(13);
boolean sendMessage = false;

void setup () {
    Serial.begin(115200);
    Serial.println("AIRCON TEST START");
}

void loop () {
    while (Serial.read() != -1) {
        sendMessage = true;
    }

    if (sendMessage) {
        Serial.print("SEND-IR: ");
        Serial.println(millis());
        AirController.sendHvac(HVAC_AUTO, 25, FAN_SPEED_AUTO, VANNE_AUTO_MOVE, true);
        sendMessage = false;
    }
}
