#include <MKL25Z4.H>
#include "led_service.h"

int main (void) {
	 /* Disable global IRQs */
	__disable_irq();       

	led_init_gpio();
	led_color = led_control(LED_BLUE_COLOR);
	
	/* Select MCGFLLCLK as timer counter clock */
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
  /* Enable clock to TPM1 */
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	/* Disable timer while configuring */
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	
	/* Select prescaler for timer */
	TPM1->SC|= TPM_SC_PS(7);
	/* Select counter increment on counter clock */
	TPM1->SC |= TPM_SC_CMOD(1);
	
	/* Select half of max modulo value */
	TPM1->MOD = 0x7FFF;       
	
	/* Enable timer overflow interrupt */
	TPM1->SC |= TPM_SC_TOIE(1);         
	
//	/* Select counter to operate in up counting mode */
//	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
//	
//	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
//	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
//	
	/* Enable IRQ18 for TPM1 */
	NVIC_SetPriority(TPM1_IRQn, 0);
	NVIC_ClearPendingIRQ(TPM1_IRQn);
	NVIC_EnableIRQ(TPM1_IRQn); // ALT: NVIC->ISER[0] |= 0x00040000; // enable IRQ18 (bit 18 of ISER[0])
	
	/* Enable global IRQs */
	__enable_irq();         

	while (1);
}

void TPM1_IRQHandler(void) {
	led_color = led_control(led_color);
	/* Clear TOF */
	TPM1->SC |= TPM_SC_TOF(1);  
}
