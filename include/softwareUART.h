#include <stdint.h>
#include "esp32-hal-gpio.h"

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
    SUCCESS = 1
} UARTStatusType;


UARTStatusType SUART_init(UARTConfig *conf);