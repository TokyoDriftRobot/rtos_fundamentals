#define LED_RED_PIN 18 // PortB Pin 18
#define LED_GREEN_PIN 19 // PortB Pin 19
#define LED_BLUE_PIN 1 // PortD Pin 1

typedef enum {
    LED_RED_COLOR,
    LED_BLUE_COLOR,
    LED_GREEN_COLOR,
} color_t;

extern color_t led_color;
color_t led_control(color_t);

void led_init_gpio(void);
void led_red_on(void);
void led_blue_on(void);
void led_green_on(void);
