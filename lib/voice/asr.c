//
// Created by Jecelyin Peng on 2023/10/15.
//

#include "asr.h"

#define ASR_OPEN_GPIO   GPIOE
#define ASR_OPEN_PIN    GPIO_PIN_5
#define ASR_CLOSE_GPIO   GPIOE
#define ASR_CLOSE_PIN   GPIO_PIN_2
#define ASR_WARN_RCU_GPIO   RCU_GPIOB
#define ASR_WARN_GPIO   GPIOB
#define ASR_WARN_PIN    GPIO_PIN_8

void asr_init(void) {
    rcu_periph_clock_enable(RCU_GPIOE);
    gpio_mode_set(ASR_OPEN_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, ASR_OPEN_PIN);
    gpio_mode_set(ASR_CLOSE_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, ASR_CLOSE_PIN);
    /* 使能时钟 */
    rcu_periph_clock_enable(ASR_WARN_RCU_GPIO);
    /* 配置GPIO的模式 */
    gpio_mode_set(ASR_WARN_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,ASR_WARN_PIN);
    /* 配置GPIO的输出 */
    gpio_output_options_set(ASR_WARN_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,ASR_WARN_PIN);
}

asr_cmd asr_check_command(void) {
    if (gpio_input_bit_get(ASR_OPEN_GPIO, ASR_OPEN_PIN) == SET) {
        return ASR_OPEN;
    }
    if (gpio_input_bit_get(ASR_CLOSE_GPIO, ASR_CLOSE_PIN) == SET) {
        return ASR_CLOSE;
    }
    return ASR_NONE;
}

void asr_clean_command(void) {
    while (gpio_input_bit_get(ASR_OPEN_GPIO, ASR_OPEN_PIN) == SET
    || gpio_input_bit_get(ASR_CLOSE_GPIO, ASR_CLOSE_PIN) == SET) {}
}

void asr_play_warn(void) {
    gpio_bit_toggle(GPIOB, GPIO_PIN_8);
    delay_1ms(100);
    gpio_bit_toggle(GPIOB, GPIO_PIN_8);
}