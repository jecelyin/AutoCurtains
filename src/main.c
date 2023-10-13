//
// Created by Jecelyin Peng on 2023/8/23.
//
#include "gd32f4xx.h"
#include "bsp_usart.h"
#include "systick.h"
#include "bsp_pwm.h"
#include "../lib/dma/bsp_dma.h"
#include "stdlib.h"
#include <stdio.h>
#include "string.h"
#include "../lib/adc/bsp_adc.h"
#include "light_adc/light_adc.h"


int main(void) {
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    systick_config();
    usart_gpio_config(115200U);  // 串口0初始化
    pwm_config(200, 10000); // PWM初始化
    dma_config(); // DMA配置
    light_init();

    while (1) {
        /* 等待数据传输完成 */
        if (g_recv_complete_flag)                                        // 数据接收完成
        {
            g_recv_complete_flag = 0;                   // 等待下次接收
            printf("g_recv_length:%d \n", g_recv_length);  // 打印接收的数据长度
            printf("g_recv_buff:%s\n", g_recv_buff);        // 打印接收的数据
            memset(g_recv_buff, 0, g_recv_length);                // 清空数组
            g_recv_length = 0;                                                    // 清空长度
        }
        // 呼吸灯
        pwm_breathing_lamp();

        printf("ADC-%d\r\n", get_light_adc_value(4) );
        printf("light-%d%%\r\n", get_light_percentage_value() );
        delay_1ms(300);
    }
}

void NMI_Handler(void) {}

void HardFault_Handler(void) {
    while (1);
}

void MemManage_Handler(void) {
    while (1);
}

void BusFault_Handler(void) {
    while (1);
}

void UsageFault_Handler(void) {
    while (1);
}

void SVC_Handler(void) {
}

void DebugMon_Handler(void) {
}

void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
    delay_decrement();
}
