
#include "bsp_adc.h"
#include "systick.h"


// adc only
void adc_init(uint32_t adc_periph, uint8_t adc_channel) {
    //配置ADC为独立模式
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);

    //使能扫描模式
    adc_special_function_config(adc_periph, ADC_SCAN_MODE, ENABLE);

    //数据右对齐
    adc_data_alignment_config(adc_periph, ADC_DATAALIGN_RIGHT);

    //ADC0设置为12位分辨率
    adc_resolution_config(adc_periph, ADC_RESOLUTION_12B);

    //ADC0设置为规则组  一共使用 1 个通道
    adc_channel_length_config(adc_periph, ADC_REGULAR_CHANNEL, 2);

    //ADC外部触发禁用, 即只能使用软件触发
    adc_external_trigger_config(adc_periph, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    //ADC0使能
    adc_enable(adc_periph);

    //开启ADC自校准
    adc_calibration_enable(adc_periph);

    //配置ADC时钟
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
}

/**********************************************************
 * 函 数 名 称：Get_ADC_Value
 * 函 数 功 能：读取ADC值
 * 传 入 参 数：ADC_CHANNEL_x=要采集的通道
 * 函 数 返 回：测量到的值
 * 作       者：LC
 * 备       注：无
**********************************************************/
uint16_t adc_get_value(uint32_t adc_periph, uint8_t adc_channel) {
    uint16_t adc_value = 0;
    //设置采集通道
    adc_regular_channel_config(adc_periph, 0, adc_channel, ADC_SAMPLETIME_15);
    //开始软件转换
    adc_software_trigger_enable(adc_periph, ADC_REGULAR_CHANNEL);
    //等待 ADC 采样完成
    while (adc_flag_get(adc_periph, ADC_FLAG_EOC) == RESET )
    {
        ;
    }
    //读取采样值
    adc_value = adc_regular_data_read(adc_periph);
    //返回采样值
    return adc_value;
}
