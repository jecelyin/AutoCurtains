//
// Created by Jecelyin Peng on 2023/10/15.
// 语音识别
//

#ifndef F470_ASR_H
#define F470_ASR_H

#include "gd32f4xx.h"
#include "systick.h"

typedef enum {
    ASR_NONE,
    ASR_OPEN,
    ASR_CLOSE,
} asr_cmd;

void asr_init(void);
asr_cmd asr_check_command(void);
void asr_clean_command(void);
void asr_play_warn(void);

#endif //F470_ASR_H
