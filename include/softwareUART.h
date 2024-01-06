#include <stdint.h>

typedef struct {
    uint16_t start_bit  : 1;
    uint16_t payload    : 8;
    uint16_t parity_bit : 1; 
    uint16_t stop_bit   : 2;
} UARTPacket;


typedef struct {

} UARTConfig;