
uint32_t last = millis();
uint8_t buffer[32];
int ch1, ch2, ch3, ch4, ch5, ch6 = 0;
enum states {LENGTH, DATA, DISCARD};
int bufferIndex = 0;
int checksum = 0;
states state;
int length = 0;

void setup() {
    Serial.begin(57600);
    Serial1.begin(115200);
}

void loop() {
    decodeIBus();
    //for (int i=0; i<32; i++) {
    //    Serial.print(buffer[i],HEX);
    //}
    Serial.print("Ch1:");
    Serial.print(ch1);
    Serial.print("   Ch2:");
    Serial.print(ch2);
    Serial.print("   Ch3:");
    Serial.print(ch3);
    Serial.print("   Ch4:");
    Serial.print(ch4);
    Serial.print("   Ch5:");
    Serial.print(ch5);
    Serial.print("   Ch6:");
    Serial.print(ch6);
    
    Serial.println();
}

void decodeIBus() {
    // lenght: 0x20
    // Command: 0x40
    // ch1 LSB+MSB
    // ..
    // ch14 LSB-MSB
    // checksum
    while (Serial1.available() > 0) {
        uint32_t now = millis();
        if (now - last >= 3)
        {
          state = LENGTH;
        }
        last = now;

        uint8_t value = Serial1.read();
        switch(state) {
            case LENGTH:
                if (value <= 0x20) {
                    Serial.print("Found length:");
                    Serial.println(value, HEX);
                    buffer[0] = value;
                    bufferIndex = 1;
                    checksum = value;
                    length = value;
                    state = DATA;
                    
                }
                else {
                    Serial.println("NOT Found length");
                    state = DISCARD;
                }
            break;

            case DISCARD:
            break;

            case DATA:
                buffer[bufferIndex++] = value;
                //Serial.println(bufferIndex);
                if (bufferIndex <= length - 2)
                    checksum += value;
                if (bufferIndex == length) {
                   //Serial.print("checksum:");
                    //Serial.println(checksum, HEX);
                    int expected = buffer[length-1] * 256 + buffer[length-2];
                    if ((expected + checksum) == 0xFFFF) {
                        if (buffer[1] == 0x40) { 
                            ch1 = buffer[3] * 256 + buffer[2];
                            ch2 = buffer[5] * 256 + buffer[4];
                            ch3 = buffer[7] * 256 + buffer[6];
                            ch4 = buffer[9] * 256 + buffer[8];
                            ch5 = buffer[11] * 256 + buffer[10];
                            ch6 = buffer[13] * 256 + buffer[12];
                        }
                    }
                    else {
                        Serial.print("wrong checksum:");
                    }
                    state = DISCARD; 
                }
            break;
        }

    }
}