//
// Created by Jecelyin Peng on 2023/10/14.
//

#include "ir_receive.h"
#include "systick.h"
#include "stdio.h"

#define IR_RCU              RCU_GPIOF
#define IR_PORT             GPIOF
#define IR_PIN              GPIO_PIN_9

#define EXTI_X              EXTI_9
#define EXTI_IRQ            EXTI5_9_IRQn
#define EXTI_SOURCE_PORT    EXTI_SOURCE_GPIOF
#define EXTI_SOURCE_PIN     EXTI_SOURCE_PIN9

typedef struct ir_rx_data{

    uint8_t AddressCode;            //地址码
    uint8_t AddressInverseCode;     //地址反码
    uint8_t CommandCode;            //命令码
    uint8_t CommandInverseCode;     //命令反码

}ir_rx_data_t;


ir_rx_data_t InfraredData;

//红外引脚初始化
void ir_rx_init(void)
{
    //开启引脚时钟
    rcu_periph_clock_enable(IR_RCU);
    //开启系统配置时钟
    rcu_periph_clock_enable(RCU_SYSCFG);

    //配置引脚为上拉输入模式
    gpio_mode_set(IR_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, IR_PIN);

    /* 使能NVIC中断 中断分组为2位抢占优先级，2位子优先级 */
    nvic_irq_enable(EXTI_IRQ,2U,2U);  // 抢占优先级2，子优先级2
    /* 连接中断线到GPIO */
    syscfg_exti_line_config(EXTI_SOURCE_PORT,EXTI_SOURCE_PIN);
    /* 初始化中断线下降沿触发 */
    exti_init(EXTI_X,EXTI_INTERRUPT,EXTI_TRIG_FALLING);
    /* 使能中断 */
    exti_interrupt_enable(EXTI_X);
    /* 清除中断标志位 */
    exti_interrupt_flag_clear(EXTI_X);
}

//获取红外低电平时间
//以微秒us作为时间参考
void get_infrared_low_time( uint32_t *low_time )
{
    uint32_t time_val = 0;

    while( gpio_input_bit_get(IR_PORT, IR_PIN) == 0 )
    {
        if( time_val>= 500 )
        {
            *low_time = time_val;
            return;
        }
        delay_1us(20);
        time_val++;
    }
    *low_time = time_val;
}

//获取红外高电平时间
//以微秒us作为时间参考
void get_infrared_high_time(uint32_t *high_time)
{
    uint32_t time_val = 0;
    while( gpio_input_bit_get(IR_PORT, IR_PIN) == 1 )
    {
        if( time_val >= 250 )
        {
            *high_time = time_val;
            return;
        }
        delay_1us(20);
        time_val++;
    }
    *high_time = time_val;
}

/******************************************************************
 * 函 数 名 称：guide_and_repeat_code_judgment
 * 函 数 说 明：引导 和 重复 码 判断
 * 函 数 形 参：无
 * 函 数 返 回：1：不是引导码   2：重复码  0：引导码
 * 作       者：LC
 * 备       注：以20微秒us作为时间参考
                引导码：由一个 9ms 的低电平和一个 4.5ms 的高电平组成
                重复码：由一个 9ms 的低电平和一个 2.5ms 的高电平组成
******************************************************************/
uint8_t guide_and_repeat_code_judgment(void)
{
    uint32_t out_time=0;
    get_infrared_low_time(&out_time);
    //time>10ms             time <8ms
    if((out_time > 500) || (out_time < 400))
    {
        return 1;
    }
    get_infrared_high_time(&out_time);
    // x>5ms  或者 x<2ms
    if((out_time > 250) || (out_time < 100))
    {
        return 1;
    }

    //如果是重复码  2ms < time < 3ms
    if((out_time > 100) && (out_time < 150))
    {
        return 2;
    }

    return 0;
}

//红外数据是否正确判断
uint8_t infrared_data_true_judgment(uint8_t *value)
{
    //判断地址码是否正确
    if( value[0] != (uint8_t)(~value[1]) )  return 0;
    //判断命令码是否正确
    if( value[2] != (uint8_t)(~value[3]) )  return 1;


    printf("IR[RX]: %x %x %x %x\r\n",value[0],value[1],value[2],value[3]);
    //保存正确数据
    InfraredData.AddressCode        = value[0];
    InfraredData.AddressInverseCode = value[1];
    InfraredData.CommandCode        = value[2];
    InfraredData.CommandInverseCode = value[3];
    return 0;
}

//接收红外数据
void receiving_infrared_data(void)
{
    uint16_t group_num = 0,data_num = 0;
    uint32_t time=0;
    uint8_t bit_data = 0;
    uint8_t ir_value[5] = {0};

    uint8_t guide_and_repeat_code = 0;

    //等待引导码
    guide_and_repeat_code = guide_and_repeat_code_judgment();
    //如果不是引导码则结束解析
    if(  guide_and_repeat_code == 1 )
    {
        return;
    }

    //共有4组数据
    //地址码+地址反码+命令码+命令反码
    for(group_num = 0; group_num < 4; group_num++ )
    {
        //接收一组8位的数据
        for( data_num = 0; data_num < 8; data_num++ )
        {
            //接收低电平
            get_infrared_low_time(&time);
            //如果不在0.56ms内的低电平,数据错误
            if((time > 60) || (time < 20))
            {
                return ;
            }

            time = 0;
            //接收高电平
            get_infrared_high_time(&time);
            //如果是在1200us<t<2000us范围内则判断为1
            if((time >=60) && (time < 100))
            {
                bit_data = 1;
            }
                //如果是在200us<t<1000us范围内则判断为0
            else if((time >=10) && (time < 50))
            {
                bit_data = 0;
            }

            //groupNum表示第几组数据
            ir_value[ group_num ] <<= 1;

            //接收的第1个数为高电平;在第二个for循环中，数据会向右移8次
            ir_value[ group_num ] |= bit_data;

            //用完时间要重新赋值
            time=0;
        }
    }
    //判断数据是否正确，正确则保存数据
    infrared_data_true_judgment(ir_value);
}

//获取红外发送过来的命令
uint8_t get_infrared_command(void)
{
    return InfraredData.CommandCode;
}
//清除红外发送过来的数据
void clear_infrared_command(void)
{
    InfraredData.CommandCode = 0x00;
}


void EXTI5_9_IRQHandler(void)
{
    if(exti_interrupt_flag_get(EXTI_X) == SET)   // 中断标志位为1
    {
        if(gpio_input_bit_get(IR_PORT,IR_PIN) == RESET)  // 如果是低电平
        {
            //接收一次红外数据
            receiving_infrared_data();
        }
        exti_interrupt_flag_clear(EXTI_X);            // 清中断标志位
    }
}