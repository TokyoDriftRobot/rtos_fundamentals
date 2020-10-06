#include "MKL25Z4.h" 

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

extern volatile uint8_t rx_data;
void uart2_init(uint32_t baud_rate);

