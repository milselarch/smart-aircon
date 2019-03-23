#define PERIOD 26
#define MAX_LEN 800

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
    static const unsigned int commands = 64; 

    int temps[commands] = {
        16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 21, 
        21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 
        26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 29, 30, 30, 30, 30, 31, 31, 31,
        31
    };
    int fanSpeeds[commands] = {
        0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 
        0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 1, 2, 3, 0, 1, 2, 3
    };
    
    const char * codes[commands] = {
        "000003020001020001000202110201000006000005002700030000", "00000302000102000100020211020100000600000005260100020000",
        "0000030200010200010002021102010000060000000400270000020000", "000003020001020001000202110201000006000000040129010000",
        "00000302000102000100020211020100000700002d01020000", "0000030200010200010002021102010000070000052800020000",
        "00000302000102000100020211020100000700000400260100020000", "0000030200010200010002021102010000070000040125000000020000",
        "00000302000102000100020211020100000601002c02020000", "000003020001020001000202110201000006010005260001020000",
        "000003020001020001000202110201000006010004002800020000", "0000030001000102000100020211020100000601000401260000020000",
        "000003020001020001000202110201000008002f020000", "00000302000102000100020211020100000800052701020000",
        "000003020001020001000202110201000008000400260001020000", "000003020001020001000202110201000008000401250100020000",
        "00000302000102000100020211020100000600012c0000030000", "0000030200010200010002021102010000060001052602020000",
        "000003020001020001000202110201000006000104002701020000", "000003020001020001000202110201000006000104012700020000",
        "000003020001020001000202110201000007012d00030000", "000003020001020001000202110201000007010529020000",
        "0000030200010200010002021102010000070104002602020000", "000003020001020001000202110201000007010401250001020000",
        "000003020001020001000202110201000006022c01030000", "0000030200010200010002021102010000060205260000030000",
        "00000302000102000100020211020100000602040029020000", "0000030200010200010002021102010000060204012601020000",
        "0000030200010200010002021102010000092e030000", "000003020001020001000202110201000009052700030000", 
        "0000030200010200010002021102010000090400260000030000", "00000302000102000100020211020100000904012502020000",
        "00000302000102000100020211020100000600002d00040000", "0000030200010200010002021102010000060000062601030000",
        "000003020001020001000202110201000006000005002700030000", "0000030200010200010002021102010000060000050128020000",
        "000003020001020001000202110201000007002e040000", "000003020001020001000202110201000007000628030000",
        "0000030200010200010002021102010000070005002601030000", "000003020001020001000202110201000007000501250000030000",
        "000002020001020001000202110201000006012d050000", "00000302000102000100020211020100000601062600040000", 
        "00000302000102000100020211020100000601050028030000", "0000030200010200010002021102010000060105012600030000", 
        "000003020001020001000202110201000008330000", "0000030200010200010002021102010000080627040000", 
        "00000302000102000100020211020100000805002600040000", "00000302000102000100020211020100000805012501030000", 
        "000003020001020001000202110201000006002e00000000000100", "000003020001020001000202110201000006000726050000", 
        "00000302000102000100020211020100000600060027040000", "00000302000102000100020211020100000600060127030000", 
        "0000030200010200010002021102010000072f000000000100", "000003020001020001000202110201000007072c0000", 
        "000003020001020001000202110201000007060026050000", "00000302000102000100020211020100000706012500040000", 
        "0000030200010200010002021102010000062f010000000100", "000003020001020001000202110201000006082600000000000100", 
        "00000302000102000100020211020100000607002c0000", "000003020001020001000202110201000006070126040000", 
        "0000030200010200010002021102010000380000000100", "00000302000102000100020211020100000f27000000000100", 
        "00000302000102000100020211020100000e002600000000000100", "00000302000102000100020211020100000e0125050000"
    };
    
    public:
    PulseSender (unsigned int pin) {
        pinMode(pin, OUTPUT);
        this->sendPin = pin;
    }

    bool sendCommand (bool isOn) {
        this->sendCommand(isOn, 25, 2);
    }

    bool sendCommand (bool isOn, int targetTemp, int targetFanSpeed) {
        if (isOn == false) {
            // send code to off aircon
            this->sendSequence("000003020001020001000202140100000e0125010000000100");
            return true;
        }
        
        for (int k = 0; k < this->commands; k++) {
            int temp = this->temps[k];
            int fanSpeed = this->fanSpeeds[k];
            String command = String(this->codes[k]);

            if (temp == targetTemp and fanSpeed == targetFanSpeed) {
                this->sendSequence(command);
                return true;
            }
        }

        return false;
    }

    int hexCharToInt (char hexChar) {
        switch (hexChar) {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'A': return 10;
            case 'B': return 11;
            case 'C': return 12;
            case 'D': return 13;
            case 'E': return 14;
            case 'F': return 15;
            default:
                Serial.println("ERROR INVALUD HEX CHAR");                
        }
    }

    void sendSequence(String sequence) {
        sequence.toUpperCase();
        unsigned int index = 0;
        
        for (int k = 0; k < sequence.length() - 1; k += 2) {
            char currentChar = sequence.charAt(k);
            char nextChar = sequence.charAt(k+1);
            int zeroLength = (
                this->hexCharToInt(nextChar) + 
                16 * this->hexCharToInt(currentChar)
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
    };
}

;
