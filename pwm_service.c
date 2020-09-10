#include "MKL25Z4.H"
#include "pwm_service.h"
#include "led_service.h"
#include "delay_service.h"


int pwn_calculate_period(int clock, int freq, int ps) {
	return clock / (freq * ps);
}  

int pwm_calculate_duty_cycle(int clock, int freq, int ps, float dc) {
	return (int) (dc * (clock / (freq * ps)));
}  

void pwm_init(int pwn_period, int pwm_duty_cycle) {
	/* Enable clock on PTB */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	/* Select PTB0 to connect to TPM */
	PORTB->PCR[PWM_PTB0_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PWM_PTB0_PIN] |= PORT_PCR_MUX(3);
	
	/* Select PTB1 to connect to TPM */
	PORTB->PCR[PWM_PTB1_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PWM_PTB1_PIN] |= PORT_PCR_MUX(3);	

  /* Enable clock to TPM1 */
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	/* Select MCGFLLCLK as timer counter clock */
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	/* Disable timer while configuring */
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	
	/* Select prescaler for timer */
	TPM1->SC|= TPM_SC_PS(7);
	/* Select counter increment on counter clock */
	TPM1->SC |= TPM_SC_CMOD(1);
	
	/* Select counter to operate in up counting mode */
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

	/* Determine EPWM period */
	TPM1->MOD = pwn_period;
	
  /* Set timer to EPWM mode for PTB0 */
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
  /* Set timer to EPWM mode for PTB1 */
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	/* Determine pulse width (duty cycle) */
	TPM1_C0V = pwm_duty_cycle;
	TPM1_C1V = pwm_duty_cycle;
}
