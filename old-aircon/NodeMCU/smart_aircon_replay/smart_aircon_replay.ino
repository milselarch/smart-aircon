#define SENDPIN D2

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200); //change BAUD rate as required
    pinMode(SENDPIN, OUTPUT);

    int timings[] = {3391,1668,446,1323,384,1318,385,430,367,421,499,303,426,1356,430,380,421,380,500,1185,499,1220,385,428,446,1231,499,385,385,385,497,1221,386,1322,382,383,502,1199,503,1210,384,502,382,385,500,1207,385,498,382,379,499,1184,499,380,382,499,385,381,503,383,384,382,503,386,384,499,385,381,501,383,385,500,382,382,497,385,386,385,513,383,385,385,500,385,497,385,386,1323,385,380,497,379,385,1298,384,377,500,1189,503,1201,500,1204,382,500,382,384,497,386,381,504,382,385,500,383,384,1318,385,386,499,385,382,1318,385,385,405,498,386,380,501,379,379,494,382,377,496,382,385,499,384,384,503,382,382,504,385,381,386,502,385,385,503,383,384,500,1199,388,502,385,384,384,503,385,384,502,384,385,502,384,384,382,503,382,384,497,384,380,502,379,384,498,384,409,384,501,385,385,502,382,384,384,500,384,384,502,385,384,499,385,381,500,384,385,500,385,384,385,502,384,384,1327,384,385,502,1209,384,497,384,381,500,379,384,1578,262,1150,412}
    ;
    int arrSize = sizeof(timings) / sizeof(*timings);

    while (true) {
        int value = HIGH;
        for (int k = 0; k < arrSize; k++) {
            digitalWrite(SENDPIN, value);
            delayMicroseconds(timings[k]);

            //Serial.print(timings[k]);
            //Serial.print(" ");
            if (value == HIGH) { value = LOW; }
            else { value = HIGH; }
        }

        Serial.print("FINAL ");
        Serial.println(value);
        Serial.print("\n");
        digitalWrite(SENDPIN, LOW);
        delay(2500);
    }
}


void loop() {
    // put your main code here, to run repeatedly:
}
