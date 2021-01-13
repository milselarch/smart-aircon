const unsigned int READPIN = A0;
const unsigned int MAX_LEN = 2000;
const unsigned long DELAY = 5000;
const unsigned int THRESHOLD = 450;

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
    return readValue() > THRESHOLD;
}

void loop() {
    Serial.println("AWAITING PRESS-SEND");
    // put your main code here, to run repeatedly:
    unsigned int readings[MAX_LEN];
    unsigned int timings[MAX_LEN];

    unsigned int lastReading;
    unsigned int index = 0;
    
    do {
        lastReading = readValue();
        delayMicroseconds(DELAY);
        delay(0);
        //yield();
    } while (lastReading < THRESHOLD);

    unsigned int newReading = lastReading;
    unsigned long timing = micros();
    unsigned long startTime = timing;
    unsigned long lastHighTiming = timing;
    unsigned long lastTiming = timing;
    Serial.println("RECORDING");
    timings[index] = 0;
    readings[index] = lastReading;
    index++;

    while (timing - lastHighTiming < 1000 * 1000) {
        newReading = readValue();
        timing = micros();
         
        if (newReading > THRESHOLD) {
            lastHighTiming = timing;
        }
        
        if ((newReading < THRESHOLD) != (lastReading < THRESHOLD)) {
            lastReading = newReading;
            timings[index] = timing - lastTiming;
            readings[index] = lastReading;
            lastTiming = timing;
            index++;

            //Serial.println(newReading);
            if (index == MAX_LEN) {
                Serial.println("BREACHED MAX LEN");
            }
        }

        delayMicroseconds(DELAY);
        delay(0);
        yield();
    }

    Serial.print("[");
    for (int k = 0; k <= index; k++) {
        Serial.print(timings[k] / 1000);
        Serial.print(",");
        delayMicroseconds(DELAY);
        delay(0);
        //yield();
    }
    Serial.println("]");

    Serial.print("[");
    for (int k = 0; k <= index; k++) {
        Serial.print(readings[k]);
        Serial.print(",");
        delayMicroseconds(DELAY);
        delay(0);
    }
    Serial.print("]");
    Serial.println("");
    delayMicroseconds(DELAY);
    delay(0);
}
