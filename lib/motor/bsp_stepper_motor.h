
#ifndef _BSP_STEPPER_MOTOR_H_
#define _BSP_STEPPER_MOTOR_H_

#include "gd32f4xx.h"


#define AP_RCU      RCU_GPIOG
#define AP_PORT     GPIOG
#define AP_PIN      GPIO_PIN_12

#define AM_RCU      RCU_GPIOG
#define AM_PORT     GPIOG
#define AM_PIN      GPIO_PIN_10

#define BP_RCU      RCU_GPIOB
#define BP_PORT     GPIOB
#define BP_PIN      GPIO_PIN_9

#define BM_RCU      RCU_GPIOB
#define BM_PORT     GPIOB
#define BM_PIN      GPIO_PIN_6

#define AP(X)   gpio_bit_write(AP_PORT, AP_PIN, X?SET:RESET)//A+    
#define AM(X)   gpio_bit_write(AM_PORT, AM_PIN, X?SET:RESET)//A-      
#define BP(X)   gpio_bit_write(BP_PORT, BP_PIN, X?SET:RESET)//B+  
#define BM(X)   gpio_bit_write(BM_PORT, BM_PIN, X?SET:RESET)//B-   


extern uint8_t motor_cw_flag;
extern uint8_t motor_ccw_flag;


void stepper_motor_config(void);

void stepper_motor_timer_config(void);

int get_step_count(void);

void set_step_count(int num);

void motor_stop(void);

void limit_judgment(int num);

void curtain_reset(void);

void open_curtain(void);

void close_curtain(void);

#endif
