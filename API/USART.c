#include "USART.h"






/**
 * @brief 串口初始化
 * 
 */
void USART_Config(void)
{
    //nvic_irq_enable(USART0_IRQn, 16, 0);// 设置优先级分组，全为抢占式优先级
    rcu_periph_clock_enable(RCU_GPIOA);// 开启GPIOA时钟
    rcu_periph_clock_enable(RCU_USART0);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1_5BIT);
    // usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
    // usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
    usart_enable(USART0);

}


int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}






