#include "RTE_Components.h"
#include "cmsis_os2.h"

#define LED_RED_PIN 18 // PortB Pin 18
#define LED_GREEN_PIN 19 // PortB Pin 19
#define LED_BLUE_PIN 1 // PortD Pin 1

typedef enum {
    LED_RED_COLOR,
    LED_BLUE_COLOR,
    LED_GREEN_COLOR,
} color_t;

extern color_t led_color;
extern osMutexId_t myMutex;

color_t led_control(color_t);

void led_init_gpio(void);
void led_red_on(void);
void led_blue_on(void);
void led_green_on(void);
void led_off_rgb(void);
void led_red_thread(void *);
void led_green_thread(void *);
void led_red_mutex(void *);
void led_green_mutex(void *);
