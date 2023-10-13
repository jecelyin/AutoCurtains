  /********************************************************************************
   * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
   * 版 本 号: V1.0
   * 修改作者: LCKFB
   * 修改日期: 2023年08月04日
   * 功能介绍:      
   ******************************************************************************
   * 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源  
   * 开发板官网：www.lckfb.com   
   * 技术支持常驻论坛，任何技术问题欢迎随时交流学习  
   * 立创论坛：club.szlcsc.com   
   * 其余模块移植手册：https://dri8c0qdfb.feishu.cn/docx/EGRVdxunnohkrNxItYTcrwAnnHe
   * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
   * 不靠卖板赚钱，以培养中国工程师为己任
 *********************************************************************************/

#ifndef _BSP_ADC_H__
#define	_BSP_ADC_H__

#include "gd32f4xx.h"

  void adc_init(uint32_t adc_periph, uint8_t adc_channel);
  uint16_t adc_get_value(uint32_t adc_periph, uint8_t adc_channel);

#endif
