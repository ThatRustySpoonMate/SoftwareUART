#pragma once

#include <stdint.h>
#include "esp32-hal-gpio.h"

#define millisecondsInSecond 1000000
#define bitTime(baudRate) millisecondsInSecond / baudRate

typedef struct {
    //uint16_t start_bit  : 1;
    uint16_t payload    : 8;
    uint16_t parity_bit : 1; 
    //uint16_t stop_bit   : 2;
} SUARTPacket;


// tx and rx are oriented to this device.
// I.E tx_pin is the pin that THIS device will send data through
typedef struct {
    uint32_t baud_rate;
    uint8_t tx_pin;
    uint8_t rx_pin; 
} SUARTConfig;


typedef enum {
    GENERIC_ERROR = 0,
    SUCCESS = 1,
    TIMEOUT_EXCEEDED = 2
} SUARTStatusType;

void _calculate_parity(SUARTPacket *pkt);

SUARTStatusType SUART_init(SUARTConfig *conf);

SUARTStatusType SUART_transmit(SUARTConfig *conf, SUARTPacket *pkt);

SUARTStatusType SUART_receive(SUARTConfig *conf, SUARTPacket *pkt, uint32_t timeout);