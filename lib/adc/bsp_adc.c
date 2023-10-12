
#include "bsp_adc.h"
#include "systick.h"

void adc_init(uint32_t adc_periph, uint8_t adc_channel, uint32_t channel_length) {
    /* 配置ADC时钟        */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);

    /* 配置ADC为独立模式        */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);

    /* 使能连续转换模式        */
    adc_special_function_config(adc_periph, ADC_CONTINUOUS_MODE, ENABLE);

    /* 使能扫描模式        */
    adc_special_function_config(adc_periph, ADC_SCAN_MODE, ENABLE);

    /* 数据右对齐        */
    adc_data_alignment_config(adc_periph, ADC_DATAALIGN_RIGHT);

    /* ADC0设置为规则组  一共使用 LIGHT_CHANNEL_NUM 个通道                */
    adc_channel_length_config(adc_periph, ADC_REGULAR_CHANNEL, channel_length);

    /* ADC规则通道配置：ADC0的通道11的扫描顺序为0；采样时间：15个周期                */
    /* DMA开启之后 gt_adc_val[x][0] = PC1的数据   */
    adc_regular_channel_config(adc_periph, 0, adc_channel, ADC_SAMPLETIME_15);//PC1

    /* ADC0设置为12位分辨率                */
    adc_resolution_config(adc_periph, ADC_RESOLUTION_12B);

    /* ADC外部触发禁用, 即只能使用软件触发                */
    adc_external_trigger_config(adc_periph, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    /* 使能规则组通道每转换完成一个就发送一次DMA请求                */
    adc_dma_request_after_last_enable(adc_periph);

    /* 使能DMA请求                */
    adc_dma_mode_enable(adc_periph);

    /* 使能DMA                */
    adc_enable(adc_periph);

    /* 等待ADC稳定                */
    delay_1ms(1);

    /* 开启ADC自校准                */
    adc_calibration_enable(adc_periph);
}

void adc_start(uint32_t adc_periph) {
    /* 开启软件触发ADC转换                */
    adc_software_trigger_enable(adc_periph, ADC_REGULAR_CHANNEL);
}

void adc_config(void) {
    //使能引脚时钟
    rcu_periph_clock_enable(RCU_GPIOC);
    //使能ADC时钟
    rcu_periph_clock_enable(RCU_ADC0);
    //配置ADC时钟
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);

    //配置引脚为模拟输入模式
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);

    //配置ADC为独立模式
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);

    //使能扫描模式
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

    //数据右对齐
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    //ADC0设置为12位分辨率
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);

    //ADC0设置为规则组  一共使用 1 个通道
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);

    //ADC外部触发禁用, 即只能使用软件触发
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    //ADC0使能
    adc_enable(ADC0);

    //开启ADC自校准
    adc_calibration_enable(ADC0);
}

/**********************************************************
 * 函 数 名 称：Get_ADC_Value
 * 函 数 功 能：读取ADC值
 * 传 入 参 数：ADC_CHANNEL_x=要采集的通道
 * 函 数 返 回：测量到的值
 * 作       者：LC
 * 备       注：无
**********************************************************/
unsigned int Get_ADC_Value(uint8_t ADC_CHANNEL_x) {
    unsigned int adc_value = 0;
    //设置采集通道
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_x, ADC_SAMPLETIME_15);
    //开始软件转换
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    //等待 ADC0 采样完成 
    while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET) { ;
    }
    //读取采样值
    adc_value = adc_regular_data_read(ADC0);
    //返回采样值
    return adc_value;
}
