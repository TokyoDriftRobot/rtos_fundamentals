#include "utils.h"
#include "MKL25Z4.h" 

void utils_debug_output() {
	// enable clock on port
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// set pin to gpio
	PORTB->PCR[UTILS_DEBUG_OUPUT_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[UTILS_DEBUG_OUPUT_PIN] |= PORT_PCR_MUX(1);
	
	// set direction of pin
	PTB->PDDR |= MASK(UTILS_DEBUG_OUPUT_PIN);
	
	// set voltage to high
	PTB->PDOR = MASK(UTILS_DEBUG_OUPUT_PIN);
}
