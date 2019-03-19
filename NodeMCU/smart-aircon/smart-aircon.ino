# include "sender.h"

PulseSender sender(D1);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("TESTING1");
    Serial.println("TESTING2");
}

void loop() {
    // put your main code here, to run repeatedly:
    
    if (Serial.available()) {
        String line = Serial.readString();
        Serial.print("SENDING ");
        Serial.println(line);
        sender.sendSequence(line);
    }
}
