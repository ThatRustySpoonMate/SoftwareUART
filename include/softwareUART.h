#pragma once

#include <stdint.h>
#include "esp32-hal-gpio.h"


//#define NOP __asm__ __volatile__ ("nop\n\t")

#define millisecondsInSecond 1000000
#define bitTime(baudRate) millisecondsInSecond / baudRate

typedef struct {
    //uint16_t start_bit  : 1;
    uint16_t payload    : 8;
    uint16_t parity_bit : 1; 
    //uint16_t stop_bit   : 2;
} UARTPacket;


// tx and rx are oriented to this device.
// I.E tx_pin is the pin that THIS device will send data through
typedef struct {
    uint32_t baud_rate;
    uint8_t tx_pin;
    uint8_t rx_pin; 
} UARTConfig;


typedef enum {
    GENERIC_ERROR = 0,
    SUCCESS = 1,
    TIMEOUT_EXCEEDED = 2
} UARTStatusType;

void _calculate_parity(UARTPacket *pkt);

UARTStatusType SUART_init(UARTConfig *conf);

UARTStatusType UART_transmit(UARTConfig *conf, UARTPacket *pkt);

UARTStatusType UART_receive(UARTConfig *conf, UARTPacket *pkt, uint32_t timeout);