//
// Created by Jecelyin Peng on 2023/10/12.
//

#ifndef F470_LIGHT_ADC_H
#define F470_LIGHT_ADC_H

#include "gd32f4xx.h"

void light_init(void);
uint16_t get_light_adc_value();
uint16_t get_light_percentage_value(void);

#endif //F470_LIGHT_ADC_H
