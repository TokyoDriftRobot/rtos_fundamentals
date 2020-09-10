#define PWM_PTB0_PIN 0
#define PWM_PTB1_PIN 1

int pwn_calculate_period(int, int, int);
int pwm_calculate_duty_cycle(int, int, int, float );
void pwm_init(int, int);
