//
// Created by Jecelyin Peng on 2023/10/15.
//

#include "rcwl0516.h"
#include "stdio.h"

#define BSP_KEY_RCU   RCU_GPIOA
#define BSP_KEY_PORT  GPIOA
#define BSP_KEY_PIN   GPIO_PIN_1

#define BSP_KEY_EXTI_IRQN			 		EXTI0_IRQn
#define BSP_KEY_EXTI_PORT_SOURCE 	EXTI_SOURCE_GPIOA
#define BSP_KEY_EXTI_PIN_SOURCE  	EXTI_SOURCE_PIN1
#define BSP_KEY_EXTI_LINE 				EXTI_0
#define BSP_KEY_EXTI_IRQHANDLER   EXTI0_IRQHandler

static uint8_t _has_human = 0;

void human_detector_init(void)
{
    /* 开启时钟 */
    rcu_periph_clock_enable(BSP_KEY_RCU);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* 配置GPIO的模式 */
    gpio_mode_set(BSP_KEY_PORT,GPIO_MODE_INPUT,GPIO_PUPD_PULLDOWN,BSP_KEY_PIN);

    nvic_irq_enable(BSP_KEY_EXTI_IRQN,3U,3U);
    syscfg_exti_line_config(BSP_KEY_EXTI_PORT_SOURCE,BSP_KEY_EXTI_PIN_SOURCE);


    exti_init(BSP_KEY_EXTI_LINE,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
    exti_interrupt_enable(BSP_KEY_EXTI_LINE);
    exti_interrupt_flag_clear(BSP_KEY_EXTI_LINE);
}


void BSP_KEY_EXTI_IRQHANDLER(void)
{
    if(exti_interrupt_flag_get(BSP_KEY_EXTI_LINE) == SET)
    {
        _has_human = gpio_input_bit_get(BSP_KEY_PORT,BSP_KEY_PIN) == SET;
        exti_interrupt_flag_clear(BSP_KEY_EXTI_LINE);
    }
}


uint8_t has_human(void)
{
    return _has_human;
}