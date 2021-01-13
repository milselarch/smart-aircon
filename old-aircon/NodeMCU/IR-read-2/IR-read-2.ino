const unsigned long READPIN = A0;
unsigned long lastChange = -1;
# define MAX_LEN 800
# define THRESHOLD 250
# define HIGH_THRES 255
# define LOW_THRES 195

long readings[MAX_LEN];
long timings[MAX_LEN];

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); //change BAUD rate as required
    pinMode(READPIN, INPUT);
    Serial.println("meh");
    Serial.println("meh");
}

int getLevel(unsigned long value) {
    delay(0);
    return value > THRESHOLD;
}

void printArray(long arr[], int index) {
    delay(10);
    delay(0);
    Serial.print("[");
    delay(1);
    delay(0);
    
    for (int k = 0; k < index - 1; k++) {
        // Serial.print(getLevel(readings[k]));
        yield();
        delay(0);
        delay(0);
        yield();
        
        Serial.print(arr[k]);
        if (k != index - 2) {
            Serial.print(",");
        }
        
        yield();
        delay(0);
        delay(0);
    }
    
    yield();
    delay(0);
    Serial.print("]");
    delay(1);
    delay(0);
    Serial.println("");
    delay(1);
    delay(0);
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

    int oldValue = 0;
    bool inProgress = false;
    unsigned long lastChange = micros();
    unsigned long timeNow;
    int index = 0;
    
    while (true) {      
        timeNow = micros();    
        if ((timeNow - lastChange > 1000 * 1000) && inProgress) {
            delay(0);
            Serial.println("SPEW RECORDING");
            
            printArray(readings, index);
            delay(100);
            Serial.println("SPEW TIMINGS");
            yield();
            printArray(timings, index);
            delay(100);
            yield();
            Serial.print("LENGTH ");
            Serial.print(index);
            Serial.println("");
            
            inProgress = false;
            oldValue = 0;
            index = 0;
            delay(0);
            
            Serial.println("END SPEW RECORDING");
            delay(0);
        }

        val = analogRead(READPIN);
        
        if (
            ((val > HIGH_THRES) && (oldValue < LOW_THRES)) ||
            ((val < LOW_THRES) && (oldValue > HIGH_THRES))
        ) {
            if (!inProgress) {
                Serial.println("START RECORDING");
                inProgress = true;
            }

            readings[index] = val;
            // delay(0);
            
            if (index != 0) {
                timings[index - 1] = (timeNow - lastChange);
            } else {
                timings[0] = 0;
            }

            // first timing is duration of initial change (will always be HIGH is base is LOW)
            // delay(0);
            oldValue = val;
            lastChange = timeNow;
            index++;
        }
        
        // delay(0);
        // delay(0);
        // delay(0);
        yield();
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
