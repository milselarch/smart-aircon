#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <PCM.h>

#include "ClapDetector.h"
#include "melody.h"
#include "RGBLed.h"
#include "state.h"
;
ESP8266WebServer server(80);
ClapDetector clapDetector(A0);
AirconState aircon(D1, D2, D4);
rgbLed RGBLed(D5, D6, D7);
MelodyPlayer player(D8);

void homePage() {
    server.send(200, "text/plain", "Hello Wrld Owo");
}

void statusPage() {
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>();
    root["state"] = aircon.getState();
    root["maxTemp"] = aircon.getMaxBackgroundTemp();
    root["fanspeed"] = aircon.getFanspeed();
    root["targetTemp"] = aircon.getTargetTemp();
    root["temp"] = aircon.getTemp();

    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
}

void setStatusPage() {
    int args = server.args();
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
        }
    }

    server.send(200, "application/json", "{status: 'ok'}");
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
    Serial.begin(115200);
    delay(50);
    
    // put your setup code here, to run once:
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.autoConnect("AutoConnectAP");
    delay(100);

    if (WiFi.status() != WL_CONNECTED){
        Serial.println("failed to connect, finishing setup anyway");
    } else {
        Serial.print("local ip: ");
        Serial.println(WiFi.localIP());
    }

    Serial.println("TESTING1");
    Serial.println("TESTING2");

    aircon.update();
    server.on("/", homePage);
    server.on("/status", statusPage);
    server.on("/set-status", setStatusPage);
    server.begin();
}

void loop() {
    server.handleClient();
    clapDetector.update();
    if (clapDetector.isClapDetected()) {
        clapDetector.resetClapDetection();
        aircon.setPeopleDetected(true);
    }

    unsigned int red = 0, green = 0, blue = 0;
    int updateStatus = aircon.update();
    if (updateStatus != 0) {
        if (updateStatus == 1) {
            player.play();
        }

        blue = aircon.getPeopleDetected() ? 128: 0;
        switch (aircon.getState()) {
            case ON_STATE:
                green = 128;
                break;
            case OFF_STATE:
                red = 128;
                break;
            case AUTO_STATE:
                red = 128;
                green = 128;
                break;
            default:
                Serial.println("STATE UNKNOWN");
        }

        RGBLed.write(red, green, blue);
    }
}
