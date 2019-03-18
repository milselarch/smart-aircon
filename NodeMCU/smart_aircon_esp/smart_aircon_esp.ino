#include "HVAC_LIB_ESP8266.h";
#include <ESP8266WebServer.h>    // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager WiFi Configuration Magic

WiFiManager wifiManager;
ESP8266WebServer server(80);
HvacMitsubishi AirController((unsigned int) D2);
boolean sendMessage = false;

void handle_OnConnect () {
    Serial.print("ON-CONNECT");
    String page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
    sendMessage = true;
    server.send(200, "text/html", page);
}

void setup () {
    Serial.begin(115200);

    Serial.print("Connecting Wifi: ");
    wifiManager.autoConnect("AutoConnectAP");

    delay(1000);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);

    Serial.println("AIRCON TEST START");

    server.on("/", handle_OnConnect);
    server.begin();
}

void loop () {
    server.handleClient();

    if (sendMessage) {
        Serial.print("SEND-IR: ");
        Serial.println(millis());
        AirController.sendHvac(HVAC_AUTO, 25, FAN_SPEED_AUTO, VANNE_AUTO_MOVE, true);
        sendMessage = false;
    }
}
