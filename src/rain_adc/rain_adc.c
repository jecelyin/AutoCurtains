//
// Created by Jecelyin Peng on 2023/10/12.
//

#include "rain_adc.h"
#include "systick.h"
#include "bsp_dma.h"
#include "bsp_adc.h"


#define RAIN_RCU_GPIO     RCU_GPIOF
#define RAIN_RCU_ADC      RCU_ADC2
#define RAIN_ADC_CH       ADC2
#define RAIN_GPIO      GPIOF
#define RAIN_PIN      GPIO_PIN_8

//所在通道
#define RAIN_ADC_CHANNEL    ADC_CHANNEL_6

void rain_init(void)
{
    /* 使能引脚时钟 */
    rcu_periph_clock_enable(RAIN_RCU_GPIO);

    /* 使能ADC时钟 */
    rcu_periph_clock_enable(RAIN_RCU_ADC);

    /* 配置PC1(AO)为浮空模拟输入模式        */
    gpio_mode_set(RAIN_GPIO, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, RAIN_PIN);
    adc_init(RAIN_ADC_CH, RAIN_ADC_CHANNEL);
}

/******************************************************************
 * 函 数 名 称：get_rain_adc_value
 * 函 数 说 明：对DMA保存的数据进行平均值计算后输出
 * 函 数 形 参：CHx 第几个扫描的数据
 * 函 数 返 回：对应扫描的ADC值
 * 作       者：LC
 * 备       注：返回值最低0  最高4095
******************************************************************/
uint16_t get_rain_adc_value()
{
    return adc_get_value(RAIN_ADC_CH, RAIN_ADC_CHANNEL);
}

/******************************************************************
 * 函 数 名 称：get_rain_percentage_value
 * 函 数 说 明：读取光敏电阻值，并且返回百分比
 * 函 数 形 参：无
 * 函 数 返 回：返回百分比
 * 作       者：LC
 * 备       注：最亮100  最暗0
******************************************************************/
uint16_t get_rain_percentage_value(void)
{
    //GD32F470和GD32F450的ADC精度都是12位
    //2的12次方 = 4096
    //因为单片机是从0开始算，所以要4096-1=4095
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = get_rain_adc_value();
    //百分比 = （ 当前值 / 最大值 ）* 100
    Percentage_value = ( 1 - ( (float)adc_new / adc_max ) ) * 100;
    return Percentage_value;
}
