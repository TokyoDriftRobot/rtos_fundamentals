#include "MKL25Z4.h" 
#include "lab_service.h"
#include "led_service.h"
#include "utils.h"

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

static void delay(volatile uint32_t nof) {
	while (nof != 0) {
		__asm("NOP");
		nof--;
	}
}

void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);
	
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	bus_clock = (DEFAULT_SYSTEM_CLOCK) / 2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor>>8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
}

void UART2_Transmit_Poll(uint8_t data) {
	while(!(UART2->S1 & UART_S1_TDRE_MASK));
	UART2->D = data;
}

uint8_t UART2_Receive_Poll(void) {
	while(!(UART2->S1 & UART_S1_RDRF_MASK));
	return (UART2->D);
}


int main (void) {    
	uint8_t rx_data = 0x69;
	SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	led_init_gpio();
	int start = 0;
	uint8_t tx_data = 0;
	PTB->PDOR |= (MASK(LED_RED_PIN));
	while(1) {
		// UART2_Transmit_Poll(rx_data);
		tx_data = UART2_Receive_Poll();
		if (start == 1) {
			PTB->PDOR |= (MASK(LED_RED_PIN));
			start = 0;
		} else {
			PTB->PDOR &= (~MASK(LED_RED_PIN));
			start = 1;
		}
	}
}
