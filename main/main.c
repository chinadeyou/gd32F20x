/*!
    \file  main.c
    \brief 示例程序

    \version 2020-11-28, V1.0.0, program for GD32F20x

*/

#include <stdio.h>
#include "gd32f20x.h"
#include "systick.h"
#include "i2c0.h"
#include "i2c1.h"
#include "i2c2.h"
#include "i2c0-slave.h"
#include "i2c1-slave.h"
#include "i2c2-slave.h"
#include "adc.h"
#include "ct75mmr.h"
#include "ltc4280.h"
#include "seeprom.h"

uint8_t i2c_reg[32] = {0};

void uart0_init(void);


/*!
    \brief      计算电压值，并保存到数组。
	\desc		bit[7:4]整数部分， bit[3:0] 小数部分
    \param[in]  none
    \retval     none
*/
void adc_voltage_cal(void)
{
	int i = 0;
	float voltage[4] = {0};
	int8_t integer;
	float decimal;
	uint8_t decimal_hex;

	for(i = 0; i < 4; i++ )
	{
		voltage[i] = adc_value[i]*3.3/4096;

		integer = voltage[i];
		decimal = voltage[i] - integer;

		decimal_hex = decimal/0.0625 + 0.5;
		if(decimal_hex > 15)
			decimal_hex = 15;

		i2c_reg[i] = integer << 4 | decimal_hex;

		printf("voltage[%d] = %f\r\n", i, voltage[i]);
		printf("i2c_reg[%d]= %#x\r\n", i, i2c_reg[i]);
	}
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure systick */
    systick_config();

	uart0_init();
	
	printf("hello\r\n");

	while(1)
	{

	}

}


void uart0_init(void)
{
	 /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));

    return ch;
}


