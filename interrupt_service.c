#include "MKL25Z4.h" 
#include "RTE_Components.h"
#include "cmsis_os2.h"
#include "utils.h"
#include "switch_service.h"
#include "led_service.h"
#include "uart.h"
#include "lab_service.h"

void PORTD_IRQHandler() {
	// Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	if (PORTD->ISFR & MASK(SWITCH_BUTTON_PIN)) {
			led_color = led_control(led_color);
	}
	
	// Clear INT Flag
	PORTD->ISFR |= MASK(SWITCH_BUTTON_PIN);
}

void UART2_IRQHandler(void) {
		NVIC_ClearPendingIRQ(UART2_IRQn);
		if (UART2->S1 & UART_S1_RDRF_MASK) {
			osSemaphoreRelease(mySem);
			rx_data = UART2->D;
			PTB->PTOR |= MASK(LED_RED_PIN);
		}
}
