/********************************************************************************
  * 文 件 名: bsp_pwm.c
  * 版 本 号: 初版
  * 修改作者: LC
  * 修改日期: 2022年04月19日
  * 功能介绍:          
  ******************************************************************************
  * 注意事项:
*********************************************************************************/
#include "bsp_pwm.h"

/************************************************
函数名称 ： pwm_gpio_config
功    能 ： pwm引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
static void pwm_gpio_config(void)
{
	  /* 使能时钟 */
	rcu_periph_clock_enable(BSP_PWM_RCU);
	/* 配置GPIO的模式 */
	gpio_mode_set(BSP_PWM_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,BSP_PWM_PIN);
	/* 配置GPIO的输出 */
	gpio_output_options_set(BSP_PWM_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_PWM_PIN);
	/* 配置GPIO的复用 */
	gpio_af_set(BSP_PWM_PORT,BSP_PWM_AF,BSP_PWM_PIN);	
}

/************************************************
函数名称 ： pwm_config
功    能 ： PWM配置 
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
void pwm_config(uint16_t pre,uint16_t per)
{
	timer_parameter_struct timere_initpara;                 // 定义定时器结构体
	timer_oc_parameter_struct timer_ocintpara;					    //定时器比较输出结构体
	
	pwm_gpio_config();																			// 使能GPIO
	
	rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);							// 开启定时器时钟
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);		// 配置定时器时钟
	
	/* 配置定时器参数 */
	timer_deinit(BSP_PWM_TIMER);														// 复位定时器
	timere_initpara.prescaler = pre-1;                   		// 时钟预分频值   PSC_CLK= 200MHZ / 200 = 1MHZ       
	timere_initpara.alignedmode = TIMER_COUNTER_EDGE;      	// 边缘对齐                
	timere_initpara.counterdirection = TIMER_COUNTER_UP;    // 向上计数    	                    
	timere_initpara.period = per-1;      									  // 周期    T = 10000 * 1MHZ = 10ms  f = 100HZ   
	/* 在输入捕获的时候使用  数字滤波器使用的采样频率之间的分频比例 */	
	timere_initpara.clockdivision = TIMER_CKDIV_DIV1;    	  // 分频因子
	/* 只有高级定时器才有 配置为x，就重复x+1次进入中断 */
	timere_initpara.repetitioncounter = 0;                  // 重复计数器 0-255  
	timer_init(BSP_PWM_TIMER,&timere_initpara);							// 初始化定时器
	
	/* 配置输出结构体 */
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;   				// 有效电平的极性
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;						// 配置比较输出模式状态 也就是使能PWM输出到端口
	/* 配置定时器输出功能 */
	timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,&timer_ocintpara);

	/* 配置占空比 */
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,0);									// 配置定时器通道输出脉冲值
	timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,TIMER_OC_MODE_PWM0);				// 配置定时器通道输出比较模式
	timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,TIMER_OC_SHADOW_DISABLE);// 配置定时器通道输出影子寄存器
	
	/* 只有高级定时器使用 */
//	timer_primary_output_config(TIMER0,ENABLE);
	timer_auto_reload_shadow_enable(BSP_PWM_TIMER);
	 /* 使能定时器 */
	timer_enable(BSP_PWM_TIMER);
}

/************************************************
函数名称 ： pwm_breathing_lamp
功    能 ： 呼吸灯
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
void pwm_breathing_lamp(void)
{
	static uint8_t direct = 0; // 方向
	static uint16_t value = 0; // 脉冲值
	
	if(direct == 0)					 // 逐渐变亮 
	{
		value += 300;					 // 值越大 越亮 
		if(value > 10000)		 
		   direct = 1;         // 改变方向
	}else // 逐渐变暗
	{ 
		value -= 300;          // 值越小 越暗
		if(value <= 0)
			direct = 0;
	}
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,value); // 配置定时器通道输出脉冲值
	delay_1ms(50);                                                                // 延时50ms
}




