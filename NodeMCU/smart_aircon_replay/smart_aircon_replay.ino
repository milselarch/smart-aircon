#define SENDPIN D2

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); //change BAUD rate as required
    pinMode(SENDPIN, OUTPUT);

    int timings[] = {19455, 831, 445, 394, 459, 380, 17551, 94, 472, 367, 444, 395, 457, 382, 17351, 286, 466, 1217, 457, 382, 17335, 294, 456, 383, 446, 394, 456, 383, 17311, 1154, 450, 389, 18169}
    ;
    int arrSize = sizeof(timings) / sizeof(*timings);

    while (true) {
        int value = HIGH;
        for (int k = 0; k < arrSize; k++) {
            digitalWrite(SENDPIN, value);
            delayMicroseconds(timings[k]);

            Serial.print(timings[k]);
            Serial.print(" ");
            if (value == HIGH) { value = LOW; }
            else { value = HIGH; }
        }

        Serial.print("FINAL ");
        Serial.println(value);
        Serial.print("\n");
        digitalWrite(SENDPIN, LOW);
        delay(500);
    }
}


void loop() {
    // put your main code here, to run repeatedly:
}
