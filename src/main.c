//
// Created by Jecelyin Peng on 2023/8/23.
//
#include "gd32f4xx.h"
#include "bsp_usart.h"
#include "systick.h"
#include "bsp_pwm.h"
#include "../lib/dma/bsp_dma.h"

int main(void)
{
    systick_config();
    usart_gpio_config(9600U);  // 串口0初始化
    pwm_config(200,10000); // PWM初始化
    dma_config();												// DMA配置

    while (1)
    {
        // 呼吸灯
        pwm_breathing_lamp();
    }
}

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
    while (1)
        ;
}

void MemManage_Handler(void)
{
    while (1)
        ;
}

void BusFault_Handler(void)
{
    while (1)
        ;
}

void UsageFault_Handler(void)
{
    while (1)
        ;
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    delay_decrement();
}
