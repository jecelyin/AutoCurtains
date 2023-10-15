
#include "bsp_stepper_motor.h"
#include "systick.h"
#include "stdio.h"
#include "math.h"

uint8_t phasecw[8] = {0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09};
uint8_t phaseccw[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08};

uint8_t motor_cw_flag = 0;
uint8_t motor_ccw_flag = 0;

// 当前步数
int step_count = 0;

// 最大步数，每走一拍表示一步
// 实测从左边尽头到右边尽头的步数是600步
// 请根据自己设备的最大步数调整此参数
#define MAX_STEPS 90

void stepper_motor_timer_config(void)
{
    /* 一个周期的时间T = 1/f, 定时时间time = T * 周期
    设预分频值位pre,周期位per
    time = (pre + 1) * (per + 1) / psc_clk
    */
    timer_parameter_struct timere_initpara; // 定义定时器结构体
    /* 开启时钟 */
    rcu_periph_clock_enable(RCU_TIMER5); // 开启定时器时钟
    /* CK_TIMERx = 4 x CK_APB1  = 4x50M = 240MHZ */
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 配置定时器时钟
    timer_deinit(TIMER5);                                 // 复位定时器
    /* 配置定时器参数 */
    timere_initpara.prescaler = 2000 - 1;                //  时钟预分频值 0-65535
    timere_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 边缘对齐
    timere_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    timere_initpara.period = 200 - 1;                    // 周期
    timere_initpara.clockdivision = TIMER_CKDIV_DIV1;    // 分频因子
    timere_initpara.repetitioncounter = 0;               // 重复计数器 0-255
    timer_init(TIMER5, &timere_initpara);                // 初始化定时器

    /* 配置中断优先级 */
    nvic_irq_enable(TIMER5_DAC_IRQn, 1, 2); // 设置中断优先级为 3,2
    /* 使能中断 */
    timer_interrupt_enable(TIMER5, TIMER_INT_UP); // 使能更新事件中断
    /* 使能定时器 */
    timer_enable(TIMER5);
}

void stepper_motor_config(void)
{
    rcu_periph_clock_enable(AP_RCU); // 开启时钟
    rcu_periph_clock_enable(AM_RCU); // 开启时钟
    rcu_periph_clock_enable(BP_RCU); // 开启时钟
    rcu_periph_clock_enable(BM_RCU); // 开启时钟

    /* 配置A+推挽输出模式 上拉模式 */
    gpio_mode_set(AP_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, AP_PIN);
    gpio_output_options_set(AP_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AP_PIN);

    /* 配置A-推挽输出模式 上拉模式 */
    gpio_mode_set(AM_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, AM_PIN);
    gpio_output_options_set(AM_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AM_PIN);

    /* 配置B+推挽输出模式 上拉模式 */
    gpio_mode_set(BP_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BP_PIN);
    gpio_output_options_set(BP_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BP_PIN);

    /* 配置B-推挽输出模式 上拉模式 */
    gpio_mode_set(BM_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BM_PIN);
    gpio_output_options_set(BM_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BM_PIN);

    AP(0);
    BP(0);
    AM(0);
    BM(0);
}

void motor_cw(void)
{
    static uint8_t i = 0;
    // 开启了顺时针动作
    if (motor_cw_flag == 1)
    {
        AP((phasecw[i] >> 3) & 0x01);
        BP((phasecw[i] >> 2) & 0x01);
        AM((phasecw[i] >> 1) & 0x01);
        BM((phasecw[i] >> 0) & 0x01);

        // 拍数增加
        i = (i + 1) % 8;
        // 记录当前步数
        step_count++;
//        if (step_count >= MAX_STEPS) {
//            motor_cw_flag = 0;
//            motor_stop();
//        }
    }
}

void motor_ccw(void)
{
    static uint8_t i = 0;
    // 如果开启了逆时针动作
    if (motor_ccw_flag == 1)
    {
        AP((phaseccw[i] >> 3) & 0x01);
        BP((phaseccw[i] >> 2) & 0x01);
        AM((phaseccw[i] >> 1) & 0x01);
        BM((phaseccw[i] >> 0) & 0x01);
        i = (i + 1) % 8;

        // 记录当前步数
        if (step_count <= 1)
            step_count = 1;
        step_count--;
//        if (step_count == 0) {
//            motor_ccw_flag = 0;
//            motor_stop();
//        }
    }
}

void motor_stop(void)
{
    AP(0);
    BP(0);
    AM(0);
    BM(0);
}

void curtain_reset(void)
{
    // 如果步数没有到达最大步数
    while (get_step_count() < MAX_STEPS)
    {
        // 步进电机顺时针旋转，步数累加
        motor_cw_flag = 1;
        motor_ccw_flag = 0;
    }
    // 停止步进电机动作
    motor_cw_flag = 0;
    motor_stop();
}

void open_curtain(void)
{
    // 如果步数没有到达0步
    if (get_step_count() > 0)
    {
        // 步进电机逆时针旋转，步数累减
        motor_ccw_flag = 1;
        motor_cw_flag = 0;
    }
    else
    {
        // 停止步进电机动作
        motor_ccw_flag = 0;
        motor_cw_flag = 0;
        motor_stop();
    }
}

void close_curtain(void)
{
    // 如果步数没有到达最大步数
    if (get_step_count() < MAX_STEPS)
    {
        // 步进电机顺时针旋转，步数累加
        motor_cw_flag = 1;
        motor_ccw_flag = 0;
    }
    else
    {
        // 停止步进电机动作
        motor_cw_flag = 0;
        motor_ccw_flag = 0;
        motor_stop();
    }
}

void limit_judgment(int num)
{
    // 如果当前是顺时针旋转
    if (motor_cw_flag == 1)
    {
        // 如果顺时针旋转的步数已经累加超过最大步数
        if (num >= MAX_STEPS)
        {
            // 停止旋转
            motor_cw_flag = 0;
            motor_stop();
        }
    }
    // 如果当前是逆时针旋转
    if (motor_ccw_flag == 1)
    {
        // 如果逆时针旋转的步数已经累减到0步
        if (num <= 0)
        {
            // 停止旋转
            motor_ccw_flag = 0;
            motor_stop();
        }
    }
}

int get_step_count(void)
{
    return step_count;
}

void set_step_count(int num)
{
    step_count = num;
}

void TIMER5_DAC_IRQHandler(void)
{
    /* 这里是定时器中断 */
    if (timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP) == SET)
    {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP); // 清除中断标志位
        // 顺时针旋转
        motor_cw();
        // 逆时针旋转
        motor_ccw();
    }
}
