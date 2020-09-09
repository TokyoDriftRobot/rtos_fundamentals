#include "led_service.h"
#include "delay_service.h" 
#include "delay_service.h"
#include "utils.h"
#include "lab_service.h"
#include "switch_service.h"
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
