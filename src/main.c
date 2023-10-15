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
#include "light/light_adc.h"
#include "rain/rain_adc.h"
#include "ir/ir_receive.h"
#include "../lib/motor/bsp_stepper_motor.h"
#include "../lib/voice/asr.h"


int main(void) {
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    systick_config();
    usart_gpio_config(115200U);  // 串口0初始化
    pwm_config(200, 10000); // PWM初始化
    dma_config(); // DMA配置
    light_init();
    rain_init();
    ir_rx_init();
    // 语音识别模块引脚初始化
    asr_init();
    motor_init(5000,500);

    int i = 0;

    while (1) {
        i += 100;
        if( i >= 8000 ) i = 0;
        ao_control(0,i);//A端电机反转
        bo_control(1,i);//B端电机正转

        delay_1ms(50);
//        switch (asr_check_command()) {
//            case ASR_OPEN:
//                printf("asr open\r\n");
//                asr_clean_command();
//                break;
//            case ASR_CLOSE:
//                printf("asr close\r\n");
//                asr_clean_command();
//                break;
//            default:
//                break;
//        }
//        /* 等待数据传输完成 */
//        if (g_recv_complete_flag)                                        // 数据接收完成
//        {
//            g_recv_complete_flag = 0;                   // 等待下次接收
//            printf("g_recv_length:%d \n", g_recv_length);  // 打印接收的数据长度
//            printf("g_recv_buff:%s\n", g_recv_buff);        // 打印接收的数据
//            memset(g_recv_buff, 0, g_recv_length);                // 清空数组
//            g_recv_length = 0;                                                    // 清空长度
//        }
//        // 呼吸灯
//        pwm_breathing_lamp();
//
//        //如果按下遥控的【1】键
//        uint8_t command = get_infrared_command();
//        if (command > 0) {
//            clear_infrared_command();
//            switch (command) {
//                case 0xA2:
//                    printf("Press the 1 button \r\n");
//                    break;
//                case 0x62:
//                    printf("Press the 2 button \r\n");
//                    break;
//                case 0xe2:
//                    printf("Press the 3 button \r\n");
//                    break;
//                case 0x22:
//                    printf("Press the 4 button \r\n");
//                    break;
//                case 0x2:
//                    printf("Press the 5 button \r\n");
//                    break;
//                case 0xc2:
//                    printf("Press the 6 button \r\n");
//                    break;
//                case 0xe0:
//                    printf("Press the 7 button \r\n");
//                    break;
//                case 0xa8:
//                    printf("Press the 8 button \r\n");
//                    break;
//                case 0x90:
//                    printf("Press the 9 button \r\n");
//                    break;
//                case 0x68:
//                    printf("Press the * button \r\n");
//                    break;
//                case 0x98:
//                    printf("Press the 0 button \r\n");
//                    break;
//                case 0xb0:
//                    printf("Press the # button \r\n");
//                    break;
//                case 0x18:
//                    printf("Press the up button \r\n");
//                    break;
//                case 0x10:
//                    printf("Press the left button \r\n");
//                    break;
//                case 0x38:
//                    printf("Press the ok button \r\n");
//                    break;
//                case 0x5a:
//                    printf("Press the right button \r\n");
//                    break;
//                case 0x4a:
//                    printf("Press the down button \r\n");
//                    break;
//                default:
//                    printf("Unknown IR command: %x \r\n", command);
//            }
//        }

//        unsigned char rx_data = voice_anakysis_data();
//        //接收到语音命令
//        switch (rx_data)//根据语音命令确定对应的动作
//        {
//            case 0x01://开窗帘命令
//                open_curtain();
//                //设置非自动模式
//                set_mode_switch_flag(NO_AUTO_MODE);
//                break;
//
//            case 0x02://关窗帘命令
//                close_curtain();
//                //设置非自动模式
//                set_mode_switch_flag(NO_AUTO_MODE);
//                break;
//
//            case 0x03://自动模式命令
//                set_mode_switch_flag(AUTO_MODE);
//                break;
//
//            case 0x04://手动模式命令
//                set_mode_switch_flag(NO_AUTO_MODE);
//                break;
//        }
//        //步进电机限位判断
//        limit_judgment(get_step_count());
//        printf("light:%d,%d%%\r\n", get_light_adc_value(), get_light_percentage_value() );
//        printf("rain:%d,%d%%\r\n", get_rain_adc_value(), get_rain_percentage_value() );
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
