
#define IBUS_BUFFER_SIZE 32
static uint8_t buffer[IBUS_BUFFER_SIZE];
uint16_t ch1, ch2, ch3, ch4, ch5, ch6 = 0;
static uint8_t bufferIndex = 0;
uint16_t checksum = 0;

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
        uint8_t value = Serial1.read();
        if (bufferIndex == 0 && value != 0x20) {continue;}
        if (bufferIndex == 1 && value != 0x40) {bufferIndex = 0; continue;}
        if (bufferIndex < IBUS_BUFFER_SIZE) buffer[bufferIndex] = value;
        bufferIndex++;

        if (bufferIndex == IBUS_BUFFER_SIZE) {
            bufferIndex = 0;
            uint16_t expected = buffer[IBUS_BUFFER_SIZE-1] * 256 + buffer[IBUS_BUFFER_SIZE-2];
            checksum =0;
            for (int i=0; i < IBUS_BUFFER_SIZE-2; i++) {checksum+=buffer[i];}
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
        }
    }   
}