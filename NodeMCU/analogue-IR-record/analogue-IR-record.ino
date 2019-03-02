const unsigned int READPIN = A0;
const unsigned int MAX_LEN = 1000;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); //change BAUD rate as required
    pinMode(READPIN, INPUT);
    Serial.println("meh");
}

float readValue() {
    return analogRead(READPIN) * (25.0/3.3);
}

int isHighValue() {
    return readValue() > 250;
}

void loop() {
    Serial.println("AWAITING PRESS-SEND");
    // put your main code here, to run repeatedly:
    unsigned int readings[MAX_LEN];
    unsigned int timings[MAX_LEN];

    unsigned int lastReading;
    unsigned long lastHighTiming = -1;
    unsigned int index = 0;
    
    do {
        lastReading = isHighValue();
    } while (lastReading == false);

    lastHighTiming = micros();
    timings[index] = micros();
    readings[index] = lastReading;
    index++;

    while (micros() - lastHighTiming < 1000 * 1000) {
        unsigned int newReading = isHighValue();
        unsigned long timing = micros();
         
        if (newReading == 1) {
            lastHighTiming = timing;
        }
        
        if (newReading != lastReading) {
            lastReading = newReading;
            timings[index] = timing;
            readings[index] = lastReading;
            index++;
        }
        
        delay(0);
    }

    Serial.print("[");
    for (int k = 0; k <= index; k++) {
        Serial.print(timings[k]);
        Serial.print(",");
        delay(0);
    }
    Serial.println("]");

    Serial.print("[");
    for (int k = 0; k <= index; k++) {
        Serial.print(readings[k]);
        Serial.print(",");
        delay(0);
    }
    Serial.print("]");
    Serial.println("");
    delay(0);
}
