#include <AutoConnect.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include "sender.h"

ESP8266WebServer WebServer;
AutoConnect Portal(WebServer);
PulseSender sender(D1);
AirconState aircon();

unsigned int temp;
int state = OFF_STATE;

void homePage() {
    WebServer.send(200, "text/plain", "Hello Wrld Owo");
}

void statusPage() {
    DynamicJsonDocument doc(512);
    JsonObject root = jsonBuffer.createObject();
    root["state"] = state;
    root["temp"] = temp;

    String output;
    root.printTo(output);
    WebServer.send(200, "application/json", output);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(50);

    Serial.println("TESTING1");
    Serial.println("TESTING2");

    WebServer.on("/", homePage);
    WebServer.on("/status", statusPage);
    Portal.begin();
}

void loop() {
    // put your main code here, to run repeatedly:

    Portal.handleClient();

    /*
    if (Serial.available()) {
        String line = Serial.readString();
        Serial.print("SENDING ");
        Serial.println(line);
        sender.sendSequence(line);
    }
    */
}
