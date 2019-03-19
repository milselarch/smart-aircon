# define PERIOD 26
# define MAX_LEN 800

int signals[MAX_LEN];
int k;

void pulse (int sendPin, int iterations) {
    for (k = 0; k < iterations; k++) {
        digitalWrite(sendPin, HIGH);
        delayMicroseconds(PERIOD / 2.0);
        digitalWrite(sendPin, LOW);
        delayMicroseconds(PERIOD / 2.0);
    }
}

void pulseDuration(int sendPin, float duration) {
    // duration in us
    pulse(sendPin, duration / PERIOD);
}

void spaceDuration(int sendPin, float duration) {
    digitalWrite(sendPin, LOW);
    delayMicroseconds(duration);
};

void sendSignals (int sendPin, int signals[], int len) {
    const int START_HIGH = 3605;
    const int START_LOW = 1523;
    const int LONG_LOW = 1223 /*1223 /*1101*/;
    const int LOW_VAL = 430 /*430 /*285*/;
    const int HIGH_VAL = 553 /*420 /*553*/;

    pulseDuration(sendPin, START_HIGH);
    spaceDuration(sendPin, START_LOW);
    
    for (int k = 0; k < len; k++) {
        const int val = signals[k];

        if (val == 1) {
            pulseDuration(sendPin, HIGH_VAL);
        } else if (val == 0) {
            spaceDuration(sendPin, LOW_VAL);
        } else {
            spaceDuration(sendPin, LONG_LOW);
        }
    }
}

class PulseSender {
    int signals[MAX_LEN];
    unsigned int sendPin;
    
    public:
    PulseSender (unsigned int pin) {
        pinMode(pin, OUTPUT);
        this->sendPin = pin;
    }

    void sendSequence(String sequence) {
        unsigned int index = 0;
        
        for (int k = 0; k < sequence.length() - 1; k += 2) {
            char currentChar = sequence.charAt(k);
            char nextChar = sequence.charAt(k+1);
            int zeroLength = (
                nextChar - '0' + 
                16 * (currentChar - '0')
            );

            int startIndex = index;
            //Serial.print(currentChar);
            //Serial.println(nextChar);
            //Serial.println(String(zeroLength));
            Serial.print("1,");
            this->signals[index] = 1;
            index++;
            
            for (int i = 0; i < zeroLength; i++) {
                Serial.print("0,");
                this->signals[index] = 0;
                index++;
                Serial.print("1,");
                this->signals[index] = 1;
                index++;
            }

            if (k < sequence.length() - 3) {
                Serial.print("-1,");
                this->signals[index] = -1;
                index++;
            };

            //Serial.println(index - startIndex);
        }

        Serial.println("");
        Serial.println(String(index));
        sendSignals(this->sendPin, this->signals, index);
    }
}

;
