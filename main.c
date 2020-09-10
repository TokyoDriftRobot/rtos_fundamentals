#include <MKL25Z4.H>
#include "led_service.h"
#define PTB0_Pin 0
#define PTB1_Pin 1
int main (void) {
	 /* Disable global IRQs */
	__disable_irq();       

	/* Enable clock on PTB */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	/* Select PTB0 to connect to TPM */
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	/* Select PTB1 to connect to TPM */
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);	
	
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
	
	/* Select counter to operate in up counting mode */
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

  /* Set timer to edge-aligned pwm mode */
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	/* Enable global IRQs */
	__enable_irq();  
	
	TPM1_C0V = 0x0EA6;
	TPM1_C1V = 0x753;

	while (1);
}

void TPM1_IRQHandler(void) {
	led_color = led_control(led_color);
	/* Clear TOF */
//	TPM1->SC |= TPM_SC_TOF(1);
  TPM1_C0SC |= TPM_CnSC_CHF(1);  
}
