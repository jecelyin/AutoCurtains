//
// Created by Jecelyin Peng on 2023/10/14.
//

#ifndef F470_IR_RECEIVE_H
#define F470_IR_RECEIVE_H

#include "gd32f4xx.h"

void ir_rx_init(void);
uint8_t get_infrared_command(void);
void clear_infrared_command(void);

#endif //F470_IR_RECEIVE_H
