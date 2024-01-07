#include "main.h"


SUARTConfig testConfig;
SUARTPacket testPacket;

void setup() {

    // Configure Software UART
    SUART_Config(&testConfig, 1, 16, 18);

    // Create test packet
    testPacket.payload = 0b10101111;

    pinMode(15, OUTPUT);

    Serial.begin(115200);
}

void loop() {
    SUART_transmit(&testConfig, &testPacket);
    Serial.println(testPacket.parity_bit);

    digitalWrite(15, !digitalRead(15));

    delay(1000 * 3);
}


void SUART_Config(SUARTConfig *conf, uint32_t baud, uint8_t rx, uint8_t tx) {
    conf->baud_rate = baud;
    conf->rx_pin = rx;
    conf->tx_pin = tx;

    SUART_init(conf);
}

