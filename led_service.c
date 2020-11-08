#include "MKL25Z4.h" 
#include "RTE_Components.h"
#include "cmsis_os2.h"
#include "led_service.h"
#include "utils.h"
#include "delay_service.h"
#include "lab_service.h"

void led_init_gpio(void) {
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[LED_RED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LED_RED_PIN] |= PORT_PCR_MUX(1);
	PORTB->PCR[LED_GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LED_GREEN_PIN] |= PORT_PCR_MUX(1);
	PORTD->PCR[LED_BLUE_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED_BLUE_PIN] |= PORT_PCR_MUX(1);
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(LED_RED_PIN) | MASK(LED_GREEN_PIN));
	PTD->PDDR |= MASK(LED_BLUE_PIN);
}

void led_blue_on() {
	PTD->PDOR = ~MASK(LED_BLUE_PIN); 
	PTB->PDOR = (MASK(LED_RED_PIN) | MASK(LED_GREEN_PIN)); 
}

void led_red_on() {
	PTB->PDOR = ~MASK(LED_RED_PIN);
	PTD->PDOR = MASK(LED_BLUE_PIN);
}

void led_green_on() {
	PTD->PDOR = MASK(LED_BLUE_PIN);
	PTB->PDOR = ~MASK(LED_GREEN_PIN);
}

color_t led_control(color_t led_color) {
    if (led_color == LED_BLUE_COLOR) {
        led_blue_on();
        return LED_RED_COLOR;
    }
    if (led_color == LED_RED_COLOR){
        led_red_on();
        return LED_GREEN_COLOR;
    }
    if (led_color == LED_GREEN_COLOR) {
        led_green_on();
        return LED_BLUE_COLOR;
    }
		return LED_BLUE_COLOR;
}

void led_off_rgb() {
	PTD->PDOR = MASK(LED_BLUE_PIN);
	PTB->PDOR = (MASK(LED_RED_PIN) | MASK(LED_GREEN_PIN)); 
}

void led_red_thread(void *argument) {
	for (;;) {
		PTB->PDOR |= (MASK(LED_RED_PIN));
		delay_program(0x80000);
		PTB->PDOR &= (~MASK(LED_RED_PIN));
		delay_program(0x80000);
	}
}

void led_green_thread(void *argument) {
	for (;;) {
		PTB->PDOR |= (MASK(LED_GREEN_PIN));
		delay_program(0x80000);
		PTB->PDOR &= (~MASK(LED_GREEN_PIN));
		delay_program(0x80000);
	}
}

void led_red_mutex(void *argument) {
	for (;;) {
		osMutexAcquire(myMutex, osWaitForever);
		PTB->PDOR |= (MASK(LED_RED_PIN));
		osDelay(1000);
		PTB->PDOR &= (~MASK(LED_RED_PIN));
		osDelay(1000);
		osMutexRelease(myMutex);
	}
}

void led_green_mutex(void *argument) {
	for (;;) {
		osMutexAcquire(myMutex, osWaitForever);
		PTB->PDOR |= (MASK(LED_GREEN_PIN));
		osDelay(1000);
		PTB->PDOR &= (~MASK(LED_GREEN_PIN));
		osDelay(1000);
		osMutexRelease(myMutex);
	}
}

void led_red_semaphore(void *argument) {
	for (;;) {
		osSemaphoreAcquire(mySem, osWaitForever);
		PTB->PDOR |= (MASK(LED_RED_PIN));
		osDelay(1000);
		PTB->PDOR &= (~MASK(LED_RED_PIN));
		osDelay(1000);
		// osSemaphoreRelease(mySem);
	}
}

void led_green_semaphore(void *argument) {
	for (;;) {
		osSemaphoreAcquire(mySem, osWaitForever);
		PTB->PDOR |= (MASK(LED_GREEN_PIN));
		osDelay(1000);
		PTB->PDOR &= (~MASK(LED_GREEN_PIN));
		osDelay(1000);
		// osSemaphoreRelease(mySem);
	}
}

void led_red_communication(void *argument) {
	myDataPkt myData;
	for (;;) {
		osMessageQueueGet(redMsg, &myData, NULL, osWaitForever);
		if (myData.cmd == 0x01 && myData.data == 0x01) {
		  //osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
		  //osEventFlagsWait(led_flag, 0x0001, osFlagsWaitAny, osWaitForever);
		  PTB->PDOR &= (~MASK(LED_RED_PIN));
		  osDelay(1000);
		  PTB->PDOR |= (MASK(LED_RED_PIN));
			osDelay(1000);
		}
	}
}

void led_green_communication(void *argument) {
	myDataPkt myData;
	for (;;) {
		osMessageQueueGet(greenMsg, &myData, NULL, osWaitForever);
		if (myData.cmd == 0x01 && myData.data == 0x01) {
		  //osThreadFlagsWait(0x0002, osFlagsWaitAny, osWaitForever);
		  //osEventFlagsWait(led_flag, 0x0002, osFlagsWaitAny, osWaitForever);
		  PTB->PDOR &= (~MASK(LED_GREEN_PIN));
		  osDelay(1000);
		  PTB->PDOR |= (MASK(LED_GREEN_PIN));
			osDelay(1000);
		}
	}
}

void led_blue_communication(void *argument) {
	myDataPkt myData;
	for (;;) {
		osMessageQueueGet(blueMsg, &myData, NULL, osWaitForever);
		if (myData.cmd == 0x01 && myData.data == 0x01) {
		  //osThreadFlagsWait(0x0004, osFlagsWaitAny, osWaitForever);
		  //osEventFlagsWait(led_flag, 0x0003, osFlagsWaitAny, osWaitForever);
		  PTD->PDOR &= (~MASK(LED_BLUE_PIN));
		  osDelay(1000);
		  PTD->PDOR |= (MASK(LED_BLUE_PIN));
			osDelay(1000);
		}
	}
}

