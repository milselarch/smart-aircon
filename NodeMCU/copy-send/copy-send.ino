unsigned int READPIN = D1;
unsigned int WRITEPIN = D5;
unsigned const int MAX_LEN = 2000;

unsigned int timings[MAX_LEN];
unsigned int values[MAX_LEN];
int k = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); //change BAUD rate as required
    pinMode(READPIN, INPUT);
    pinMode(WRITEPIN, OUTPUT);
    Serial.println("meh");
    
}

void loop() {
    Serial.println("START-LOOP");
    int val = LOW;
    int index = 0;

    unsigned long startTime = micros();
    unsigned long lastTime = startTime;
    timings[index] = 0;
    values[index] = val;
    index++;

    while (micros() - startTime < 5000 * 1000) {
        int newVal = digitalRead(READPIN);
        
        if (newVal != val) {
            unsigned long nowTime = micros();
            timings[index] = nowTime - lastTime;
            lastTime = nowTime;
            
            values[index] = newVal;
            val = newVal;
            index++;
        }

        if (index == MAX_LEN) {
            Serial.println("BREACHED MAX");
        }
        
        delayMicroseconds(5);
        delay(0);
        yield();
    }

    Serial.print("[");
    for (int k = 0; k <= index; k++) {
        Serial.print(timings[k]);
        Serial.print(",");
        delayMicroseconds(5);
        delay(0);
        yield();
    }
    Serial.println("]");

    Serial.print("[");
    for (int k = 0; k <= index; k++) {
        Serial.print(values[k]);
        Serial.print(",");
        delayMicroseconds(5);
        delay(0);
        yield();
    }
    Serial.print("]");
    Serial.println("");
    delay(0);
    yield();
}


