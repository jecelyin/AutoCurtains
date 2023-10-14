//
// Created by Jecelyin Peng on 2023/10/12.
//

#ifndef F470_RAIN_ADC_H
#define F470_RAIN_ADC_H

#include "gd32f4xx.h"

void rain_init(void);
uint16_t get_rain_adc_value();
uint16_t get_rain_percentage_value(void);

#endif //F470_RAIN_ADC_H
