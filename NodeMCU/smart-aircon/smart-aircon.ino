#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include "sender.h"

ESP8266WebServer WebServer;
AutoConnect Portal(WebServer)
PulseSender sender(D1);

void homePage() {
    WebServer.send(200, "text/plain", "Hello WOrld Owo");
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("TESTING1");
    Serial.println("TESTING2");

    Server.on("/", homePage);
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
