#include "softwareUART.h"

void _calculate_parity(SUARTPacket *pkt) {
    uint8_t tmp_payload = pkt->payload;

    // Loop through payload and flip parity for each '1' present to ensure an even payload+parity
    for(uint8_t idx = 0; idx < 8; idx++) {
        if(tmp_payload & 0x01) {
            pkt->parity_bit = !pkt->parity_bit;
        }
        tmp_payload = tmp_payload >> 1;
    }
}

SUARTStatusType SUART_init(SUARTConfig *conf) {
    pinMode(conf->rx_pin, INPUT);
    pinMode(conf->tx_pin, OUTPUT);

    digitalWrite(conf->tx_pin, HIGH); // TX Line idles high

    return SUCCESS;
}

SUARTStatusType SUART_transmit(SUARTConfig *conf, SUARTPacket *pkt) {
    // Init as much as we can before gpio actions
    uint8_t tmp_payload = pkt->payload;
    _calculate_parity(pkt);
    
    // Start bit
    digitalWrite(conf->tx_pin, LOW);
    delayMicroseconds( bitTime(conf->baud_rate) );

    // Write payload
    for(uint8_t idx = 0; idx < 8; idx++) {
        digitalWrite(conf->tx_pin, tmp_payload & 0x01);
        tmp_payload = tmp_payload >> 1;
        delayMicroseconds( bitTime(conf->baud_rate) );
    }

    // Write parity bit
    digitalWrite(conf->tx_pin, pkt->parity_bit);
    delayMicroseconds( bitTime(conf->baud_rate) );

    // Stop bit/s
    digitalWrite(conf->tx_pin, HIGH);
    delayMicroseconds( 2 * bitTime(conf->baud_rate) );

    return SUCCESS;
}

// Timeout is 32 bit unsigned int in milliseconds
SUARTStatusType SUART_receive(SUARTConfig *conf, SUARTPacket *pkt, uint32_t timeout) {
    static uint32_t entryTime;
    entryTime = millis();
    
    // Wait for RX line to be pulled low
    while(digitalRead(conf->rx_pin) && millis() < entryTime + timeout) {
        NOP();
    }

    // Check if timeout has elapsed
    if(millis() < entryTime + timeout) {
        return TIMEOUT_EXCEEDED;
    }

    // Receive payload
    for(uint8_t idx = 0; idx < 8; idx++) {
        delayMicroseconds( bitTime(conf->baud_rate) );
        pkt->payload = pkt->payload | ( digitalRead(conf->rx_pin) << idx );
    }

    // Receive Parity bit
    delayMicroseconds( bitTime(conf->baud_rate) );
    pkt->parity_bit = digitalRead(conf->rx_pin);

    uint8_t test_end_bit1, test_end_bit2;

    delayMicroseconds( bitTime(conf->baud_rate) );
    test_end_bit1 = digitalRead(conf->rx_pin);

    delayMicroseconds( bitTime(conf->baud_rate) );
    test_end_bit2 = digitalRead(conf->rx_pin);

    return SUCCESS;
}