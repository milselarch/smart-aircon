# define sendPin D1
# define PERIOD 26
# define FREQ 1000.0 * 1000.0 / PERIOD 

int k;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("START");
    Serial.println("START");
    Serial.println("START");
    pinMode(sendPin, OUTPUT);
}

void pulse (int iterations) {
    for (k = 0; k < iterations; k++) {
        digitalWrite(sendPin, HIGH);
        delayMicroseconds(PERIOD / 2.0);
        digitalWrite(sendPin, LOW);
        delayMicroseconds(PERIOD / 2.0);
    }
}

void pulseDuration(float duration) {
    // duration in us
    pulse(duration / PERIOD);
}

void spaceDuration(float duration) {
    digitalWrite(sendPin, LOW);
    delayMicroseconds(duration);
};

const int timings[] = {3430,1744,437,1197,437,1306,437,327,437,436,437,436,437,1198,436,437,437,437,436,1200,437,1211,436,437,437,1201,546,328,436,437,437,1201,437,1307,437,327,437,1306,437,1197,437,436,437,436,437,1198,437,436,437,328,436,2161,436,323,543,328,435,434,437,437,437,328,457,435,436,328,546,328,437,436,437,436,436,325,434,436,437,436,436,328,436,436,435,431,434,1193,437,458,373,437,434,1306,435,325,432,1309,436,1199,431,1300,432,325,437,437,437,437,437,436,328,436,437,435,369,1307,436,1198,437,1306,437,437,327,435,436,437,434,437,436,326,436,1309,436,328,546,1199,436,437,432,326,431,437,435,434,434,326,434,436,437,436,437,436,436,325,434,434,434,433,437,325,436,436,437,363,484,436,437,328,436,437,437,437,436,437,437,327,437,437,436,438,436,328,437,437,437,437,437,328,436,437,437,437,437,328,437,436,436,436,437,327,437,436,437,436,436,434,434,325,434,436,434,437,436,328,436,437,437,1295,434,323,432,444,436,1201,546,1198,434}
;

void sendTimings() {
    int len = sizeof(timings) / sizeof(timings[0]);
    
    for (int k = 0; k < len; k++) {
        if (k % 2 == 0) {
            pulseDuration(timings[k]);
        } else {
            spaceDuration(timings[k]);
        }
    }
}

const int signals[] = {
    1, -1, 1, -1, 1, 0, 1, 0, 1, 0, 1, -1, 1, 0, 1, 0, 1, -1, 1, -1, 1, 0, 1, -1, 1, 0, 1, 0, 1, -1, 1, -1, 1, 0, 1, -1, 1, -1, 1, 0,
1, 0, 1, -1, 1, 0, 1, 0, 1, -1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 1, 0, 1, 0, 1, -1, 1, 0, 1, -1, 1, -1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 1, -1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 1, 0, 1, -1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 1, 0, 1, 0, 1, -1,
1, -1, 1
}; 

void sendSignals () {
    const int START_HIGH = 3605;
    const int START_LOW = 1523;
    const int LONG_LOW = 1223 /*1101*/;
    const int LOW_VAL = 430 /*285*/;
    const int HIGH_VAL = 420 /*553*/;

    int len = sizeof(signals) / sizeof(signals[0]);

    pulseDuration(START_HIGH);
    spaceDuration(START_LOW);
    
    for (int k = 0; k < len; k++) {
        const int val = signals[k];

        if (val == 1) {
            pulseDuration(HIGH_VAL);
        } else if (val == 0) {
            spaceDuration(LOW_VAL);
        } else {
            spaceDuration(LONG_LOW);
        }
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    int value = Serial.read();
    char character = char(value);
    
    if (value != -1) {
        sendSignals();
        Serial.print("PRINT ");
        Serial.println(millis());
    }
}
