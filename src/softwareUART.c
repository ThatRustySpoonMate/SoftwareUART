#include "softwareUART.h"

void _calculate_parity(UARTPacket *pkt) {
    uint8_t tmp_payload = pkt->payload;

    // Loop through payload and flip parity for each '1' present to ensure an even payload+parity
    for(uint8_t idx = 0; idx < 8; idx++) {
        if(tmp_payload & 0x01) {
            pkt->parity_bit = !pkt->parity_bit;
        }
        tmp_payload = tmp_payload >> 1;
    }
}

UARTStatusType SUART_init(UARTConfig *conf) {
    pinMode(conf->rx_pin, INPUT);
    pinMode(conf->tx_pin, OUTPUT);

    digitalWrite(conf->tx_pin, HIGH); // TX Line idles high

    return SUCCESS;
}

UARTStatusType UART_transmit(UARTConfig *conf, UARTPacket *pkt) {
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