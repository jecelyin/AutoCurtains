//
// Created by Jecelyin Peng on 2023/10/12.
//

#include "light_adc.h"
#include "systick.h"
#include "bsp_dma.h"
#include "bsp_adc.h"

//PF6
//    Alternate: TIMER9_CH0, SPI4_NSS, UART6_RX, EXMC_NIORD, EVENTOUT
//    Additional: ADC2_IN4
//#define RCU_ILLUME_GPIO_AO     RCU_GPIOC
#define RCU_LIGHT_GPIO_PORT     RCU_GPIOF

#define RCU_LIGHT_ADC      RCU_ADC0
#define RCU_LIGHT_DMA      RCU_DMA1

#define PORT_DMA            DMA1
#define CHANNEL_DMA         DMA_CH0

#define PORT_ADC            ADC2
#define CHANNEL_ADC         ADC_CHANNEL_4

//#define PORT_ILLUME_AO      GPIOC
//#define GPIO_ILLUME_AO        GPIO_PIN_1

#define PORT_LIGHT_PIN      GPIOF
#define GPIO_LIGHT_PIN      GPIO_PIN_6

#define GET_LIGHT_IN   gpio_input_bit_get(PORT_LIGHT_PIN, GPIO_LIGHT_PIN)

//采样次数
#define LIGHT_SAMPLES         30
//采样通道数
#define LIGHT_CHANNEL_NUM     1

//DMA缓冲区
uint16_t gt_adc_val[ LIGHT_SAMPLES ][ LIGHT_CHANNEL_NUM ];



/******************************************************************
 * 函 数 名 称：light_init
 * 函 数 说 明：初始化光敏电阻光照传感器引脚功能
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void light_init(void)
{

    /* 使能引脚时钟 */
//    rcu_periph_clock_enable(RCU_ILLUME_GPIO_AO);
    rcu_periph_clock_enable(RCU_LIGHT_GPIO_PORT);

    /* 使能ADC时钟 */
    rcu_periph_clock_enable(RCU_LIGHT_ADC);

    /* 使能DMA时钟 */
    rcu_periph_clock_enable(RCU_LIGHT_DMA);

    /* 配置PC1(AO)为浮空模拟输入模式        */
//    gpio_mode_set(PORT_LIGHT_PIN, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, PORT_LIGHT_PIN);
    adc_init(PORT_ADC, CHANNEL_ADC, LIGHT_CHANNEL_NUM);
// LIGHT_SAMPLES * LIGHT_CHANNEL_NUM
    dma_init(PORT_DMA, CHANNEL_DMA, PORT_ADC, (uint32_t*)&gt_adc_val, LIGHT_SAMPLES * LIGHT_CHANNEL_NUM);
    adc_start(PORT_ADC);
}


/******************************************************************
 * 函 数 名 称：get_light_adc_value
 * 函 数 说 明：对DMA保存的数据进行平均值计算后输出
 * 函 数 形 参：CHx 第几个扫描的数据
 * 函 数 返 回：对应扫描的ADC值
 * 作       者：LC
 * 备       注：返回值最低0  最高4095
******************************************************************/
unsigned int get_light_adc_value(char CHx)
{
    unsigned char i = 0;
    unsigned int AdcValue = 0;

    /* 因为采集 LIGHT_SAMPLES 次，故循环 LIGHT_SAMPLES 次 */
    for(i=0; i < LIGHT_SAMPLES; i++)
    {
        /*    累加    */
        AdcValue+=gt_adc_val[i][CHx];
    }
    /* 求平均值 */
    AdcValue= AdcValue / LIGHT_SAMPLES;

    return AdcValue;
}

/******************************************************************
 * 函 数 名 称：get_light_percentage_value
 * 函 数 说 明：读取光敏电阻值，并且返回百分比
 * 函 数 形 参：无
 * 函 数 返 回：返回百分比
 * 作       者：LC
 * 备       注：最亮100  最暗0
******************************************************************/
unsigned int get_light_percentage_value(void)
{
    //GD32F470和GD32F450的ADC精度都是12位
    //2的12次方 = 4096
    //因为单片机是从0开始算，所以要4096-1=4095
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = get_light_adc_value(0);
    //百分比 = （ 当前值 / 最大值 ）* 100
    Percentage_value = ( 1 - ( (float)adc_new / adc_max ) ) * 100;
    return Percentage_value;
}

/******************************************************************
 * 函 数 名 称：get_light_input_state
 * 函 数 说 明：读取DO引脚的电平状态
 * 函 数 形 参：无
 * 函 数 返 回：1=检测过亮   0=检测过暗
 * 作       者：LC
 * 备       注：无
******************************************************************/
char get_light_input_state(void)
{
    if(GET_LIGHT_IN == 1 )
    {
        return 1;
    }
    return 0;
}
