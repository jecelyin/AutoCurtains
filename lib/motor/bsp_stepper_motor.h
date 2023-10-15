#ifndef _BSP_DRV8833_H
#define _BSP_DRV8833_H


#include "gd32f4xx.h"
#include "systick.h"

#define RCU_AIN1                 RCU_GPIOA
#define PORT_AIN1                GPIOA
#define GPIO_AIN1                GPIO_PIN_2
#define AF_AIN1                  GPIO_AF_1
#define RCU_AIN1_TIMER           RCU_TIMER1
#define BSP_AIN1_TIMER           TIMER1         // 定时器
#define BSP_AIN1_CHANNEL         TIMER_CH_2     // 定时器通道

#define RCU_AIN2                 RCU_GPIOA
#define PORT_AIN2                GPIOA
#define GPIO_AIN2                GPIO_PIN_3
#define AF_AIN2                  GPIO_AF_1
#define RCU_AIN2_TIMER           RCU_TIMER1
#define BSP_AIN2_TIMER           TIMER1         // 定时器
#define BSP_AIN2_CHANNEL         TIMER_CH_3     // 定时器通道

#define RCU_BIN1                 RCU_GPIOB
#define PORT_BIN1                GPIOB
#define GPIO_BIN1                GPIO_PIN_11
#define AF_BIN1                  GPIO_AF_1
#define RCU_BIN1_TIMER           RCU_TIMER1
#define BSP_BIN1_TIMER           TIMER1         // 定时器
#define BSP_BIN1_CHANNEL         TIMER_CH_3     // 定时器通道

#define RCU_BIN2                 RCU_GPIOB
#define PORT_BIN2                GPIOB
#define GPIO_BIN2                GPIO_PIN_10
#define AF_BIN2                  GPIO_AF_1
#define RCU_BIN2_TIMER           RCU_TIMER1
#define BSP_BIN2_TIMER           TIMER1         // 定时器
#define BSP_BIN2_CHANNEL         TIMER_CH_2     // 定时器通道


#define AIN1_OUT(X)  gpio_bit_write(PORT_AIN1, GPIO_AIN1, X?SET:RESET)
#define AIN2_OUT(X)  gpio_bit_write(PORT_AIN2, GPIO_AIN2, X?SET:RESET)

#define BIN1_OUT(X)  gpio_bit_write(PORT_BIN1, GPIO_BIN1, X?SET:RESET)
#define BIN2_OUT(X)  gpio_bit_write(PORT_BIN2, GPIO_BIN2, X?SET:RESET)


void motor_init(uint16_t pre, uint16_t per);
void ao_control(uint8_t dir, uint32_t speed);
void bo_control(uint8_t dir, uint32_t speed);
#endif  /* BSP_DRV8833_H */

