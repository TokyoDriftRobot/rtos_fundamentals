#define PWM_PTB0_PIN 0
#define PWM_PTB1_PIN 1

#define PWM_NOTE_C 262
#define PWM_NOTE_D 294
#define PWM_NOTE_E 330
#define PWM_NOTE_F 349
#define PWM_NOTE_G 392
#define PWM_NOTE_A 440
#define PWM_NOTE_B 494

int pwn_calculate_period(int, int, int);
int pwm_calculate_duty_cycle(int, int, int, float );
void pwm_init(int, int);
