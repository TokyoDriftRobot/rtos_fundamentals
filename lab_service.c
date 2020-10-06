#include "MKL25Z4.h" 
#include "RTE_Components.h"
#include "cmsis_os2.h"
#include "led_service.h"
#include "delay_service.h" 
#include "utils.h"
#include "lab_service.h"
#include "switch_service.h"
#include "pwm_service.h"
#include "uart.h"

color_t led_color = LED_BLUE_COLOR;
volatile uint8_t rx_data = 0;

void lab_two(void) {
	led_init_gpio();
	while (1) {
		led_color = led_control(led_color);
		delay_program(0x0FFFFF); 
	}
}


void lab_three() {
	switch_button_init();
	led_init_gpio();
	led_color = led_control(led_color);
	while(1);
}

void lab_four (void) {    
	led_init_gpio();
	
	int counter = 0;
	const int n_notes = 7;
	const int ps = 128;
	int notes[n_notes] = {PWM_NOTE_C, PWM_NOTE_D, PWM_NOTE_E, PWM_NOTE_F, PWM_NOTE_G, PWM_NOTE_A, PWM_NOTE_B};
	
	led_color = LED_BLUE_COLOR;
	
	while (1) {
		led_color = led_control(led_color);

		counter = counter % n_notes;
	  int pwm_period = pwn_calculate_period(CLOCK_FREQ, notes[counter], ps); 
    int pwm_duty_cycle = pwm_calculate_duty_cycle(CLOCK_FREQ, notes[counter], ps, 0.1); 
		pwm_init(pwm_period, pwm_duty_cycle);
		counter += 1;

		/* Attempts to delay by 1/3 seconds */
		delay_program(12000000);
	}
}

void lab_five (void) {    
	SystemCoreClockUpdate();
	uart2_init(BAUD_RATE);
	
	rx_data = 0;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[LED_RED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LED_RED_PIN] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(LED_RED_PIN);
	while(1);
}

void lab_six(void) {
	SystemCoreClockUpdate();
	led_init_gpio();
	led_off_rgb();
	osKernelInitialize();
	osThreadNew(led_red_thread, NULL, NULL);
	osThreadNew(led_green_thread, NULL, NULL);
	osKernelStart();
	while(1);
}
