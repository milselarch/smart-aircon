#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>

#include "ClapDetector.h"
#include "melody.h"
#include "RGBLed.h"
#include "state.h"

#define MICROPHONE_PIN A0
;
ESP8266WebServer server(80);
ClapDetector clapDetector(MICROPHONE_PIN);
AirconState aircon(D1, D2, D6);
rgbLed RGBLed(D5, D7, D0);
MelodyPlayer player(D8);
DynamicJsonDocument doc(128);
JsonObject root = doc.to<JsonObject>();

void homePage() {
    server.send(200, "text/plain", "Hello Wrld Owo");
}

void statusPage() {
    Serial.println("STATUS PAGE");
    yield();
    yield();
    yield();
    root["state"] = aircon.getState();
    root["maxTemp"] = aircon.getMaxBackgroundTemp();
    root["fanspeed"] = aircon.getFanspeed();
    root["targetTemp"] = aircon.getTargetTemp();
    root["temp"] = aircon.getTemp();

    yield();
    String output;
    yield();
    serializeJson(doc, output);
    yield();
    yield();
    server.send(200, "application/json", output);
    yield();
    yield();
}

void setStatusPage() {
    yield();
    int args = server.args();
    yield();
    Serial.print("ARGS ");
    Serial.println(args);

    for (int k = 0; k < args; k++) {
        const String argName = server.argName(k);
        const String argValue = server.arg(argName);
        
        if (argName == "temp") {
            unsigned int temp = argValue.toInt();
            aircon.setTemp(temp);
        } else if (argName == "fanspeed") {
            unsigned int fanspeed = argValue.toInt();
            aircon.setFanspeed(fanspeed);
        } else if (argName == "maxTemp") {
            float maxTemp = argValue.toFloat();
            aircon.setMaxTemp(maxTemp);
        } else if (argName == "state") {
            int state = argValue.toInt();
            aircon.setState(state);
        } else if (argName == "force") {
            aircon.forceSend();
        }
    }

    yield();
    server.send(200, "application/json", "{status: 'ok'}");
    yield();
}

void configModeCallback (WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveConfigCallback () {
    Serial.println("Should save config");
    Serial.println(WiFi.localIP());
}

void setup() {
    RGBLed.write(255, 0, 0, true);
    Serial.begin(115200);
    delay(50);
    
    // put your setup code here, to run once:
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.autoConnect("AutoConnectAP");
    delay(100);

    int index = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(++index);
        Serial.println(" - failed to connect");
        delay(500);
    }

    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
    Serial.println("TESTING1");
    Serial.println("TESTING2");

    aircon.update(RGBLed);
    server.on("/", homePage);
    server.on("/status", statusPage);
    server.on("/set-status", setStatusPage);
    server.begin();
}

unsigned int lastServerUpdate = 0;
unsigned int red, green, blue;
int updateStatus;

void loop() {
    server.handleClient();
    red = 0;
    green = 0;
    blue = 0;

    //clapDetector.update();    
    //Serial.println(clapDetector.getSoundValue());
    /*
    if (clapDetector.isClapDetected()) {
        clapDetector.resetClapDetection();
        
        if (aircon.getState() == ON_STATE) {
            aircon.setState(OFF_STATE);
            Serial.println("NOW-STATE-OFF");
        } else if (aircon.getState() == OFF_STATE) {
            aircon.setState(AUTO_STATE);
            Serial.println("NOW-STATE-AUTO");
        } else if (aircon.getState() == AUTO_STATE) {
            aircon.setState(ON_STATE);
            Serial.println("NOW-STATE-ON");
        }

        blue = aircon.getPeopleDetected() ? 50: 0;
    }
    */

    updateStatus = aircon.update(RGBLed, false);
    if (updateStatus != -1) {
        // Serial.println("NEEDS UPDATE");
        if (updateStatus == 1) {
            player.play();
        }
    }
    
    switch (aircon.getState()) {
        case ON_STATE:
            red = 0;
            green = 50;
            break;
        case OFF_STATE:
            red = 50;
            green = 0;
            break;
        case AUTO_STATE:
            red = 50;
            green = 50;
            break;
        default:
            Serial.println("STATE UNKNOWN");
    }

    RGBLed.write(red, green, blue, true);
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    delay(100);
    // Serial.print("DETECT-DELAY");
    // Serial.println(aircon.detectDelay());
}
