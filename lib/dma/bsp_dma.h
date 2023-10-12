#ifndef _BSP_DMA_H
#define _BSP_DMA_H

#include "gd32f4xx.h"
#include "systick.h"

 
/* USART0_RX DMA1_CH2 */
#define BSP_DMA_RCU   	       RCU_DMA1                  // DMA时钟
#define BSP_DMA  				       DMA1 										 // DMA
#define BSP_DMA_CH  		       DMA_CH2									 // DMA通道
#define BSP_DMA_CH_IRQ  			 DMA1_Channel2_IRQn				 // DMA中断
#define BSP_DMA_CH_IRQ_HANDLER DMA1_Channel2_IRQHandler  // DMA中断服务函数名

void dma_config(void);   // 配置DMA
void dma_init(uint32_t dma_periph, dma_channel_enum channelx, uint32_t periph_addr, uint32_t *data_addr, uint32_t data_length);

#endif  /* BSP_DMA_H */