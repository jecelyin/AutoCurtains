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
#define LIGHT_RCU_GPIO     RCU_GPIOF
#define LIGHT_RCU_ADC      RCU_ADC2
#define LIGHT_ADC_CH            ADC2

#define LIGHT_GPIO      GPIOF
#define LIGHT_PIN      GPIO_PIN_6
//所在通道
#define LIGHT_ADC_CHANNEL    ADC_CHANNEL_4

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
    rcu_periph_clock_enable(LIGHT_RCU_GPIO);

    /* 使能ADC时钟 */
    rcu_periph_clock_enable(LIGHT_RCU_ADC);

    /* 使能DMA时钟 */
//    rcu_periph_clock_enable(RCU_LIGHT_DMA);

    /* 配置PC1(AO)为浮空模拟输入模式        */
    gpio_mode_set(LIGHT_GPIO, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, LIGHT_PIN);
    adc_init(LIGHT_ADC_CH, LIGHT_ADC_CHANNEL);
// LIGHT_SAMPLES * LIGHT_CHANNEL_NUM
//    dma_init(PORT_DMA, CHANNEL_DMA, LIGHT_ADC_CH, &gt_adc_val, LIGHT_SAMPLES * LIGHT_CHANNEL_NUM);
//    adc_start(LIGHT_ADC_CH);

}

/******************************************************************
 * 函 数 名 称：get_light_adc_value
 * 函 数 说 明：对DMA保存的数据进行平均值计算后输出
 * 函 数 形 参：CHx 第几个扫描的数据
 * 函 数 返 回：对应扫描的ADC值
 * 作       者：LC
 * 备       注：返回值最低0  最高4095
******************************************************************/
uint16_t get_light_adc_value()
{
    return adc_get_value(LIGHT_ADC_CH, LIGHT_ADC_CHANNEL);
}

/******************************************************************
 * 函 数 名 称：get_light_percentage_value
 * 函 数 说 明：读取光敏电阻值，并且返回百分比
 * 函 数 形 参：无
 * 函 数 返 回：返回百分比
 * 作       者：LC
 * 备       注：最亮100  最暗0
******************************************************************/
uint16_t get_light_percentage_value(void)
{
    //GD32F470和GD32F450的ADC精度都是12位
    //2的12次方 = 4096
    //因为单片机是从0开始算，所以要4096-1=4095
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = get_light_adc_value();
    //百分比 = （ 当前值 / 最大值 ）* 100
    Percentage_value = ( 1 - ( (float)adc_new / adc_max ) ) * 100;
    return Percentage_value;
}
