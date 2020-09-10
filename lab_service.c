#include "led_service.h"
#include "delay_service.h" 
#include "delay_service.h"
#include "utils.h"
#include "lab_service.h"
#include "switch_service.h"
#include "pwm_service.h"
#include "MKL25Z4.h" 

color_t led_color = LED_BLUE_COLOR;

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
	const int clock_freq = 48000000;
	const int ps = 128;
	int notes[n_notes] = {262, 294, 330, 349, 392, 440, 494};
	int pwm_period = 0; 
	int pwm_duty_cycle = 0; 
	
	led_color = LED_BLUE_COLOR;
	
	while (1) {
		led_color = led_control(led_color);

		counter = counter % n_notes;
	  pwm_period = pwn_calculate_period(clock_freq, notes[counter], ps); 
    pwm_duty_cycle = pwm_calculate_duty_cycle(clock_freq, notes[counter], ps, 0.1); 
		pwm_init(pwm_period, pwm_duty_cycle);
		counter += 1;

		/* Attempts to delay by 1/3 seconds */
		delay_program(12000000);
	}
}
