# define READPIN A0

unsigned long lastChange = -1;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); //change BAUD rate as required
    pinMode(READPIN, INPUT);
    Serial.println("meh");
}

void loop() {
    // put your main code here, to run repeatedly:
    float val = 0;
    /*
    int val = 0;
    
    while (val == 0) {
        val = analogRead(READPIN);
        delay(1);
    }

    for (int k = 0; k < 100; k++) {
        Serial.println(0);
        Serial.print(" ");
        // delay(1);
    }

    lastChange = millis();
    while (millis() - lastChange < 1000) {
    */

    while (true) {
        val = analogRead(READPIN);
        // val = digitalRead(READPIN);
        Serial.println(val);
        
        // delay(0);
        delay(0);
    }

    /*
    for (int k = 0; k < 100; k++) {
        Serial.println(0);
        Serial.print(" ");
        // delay(1);
    }

    while (true) { delay(20); }
    */
}
