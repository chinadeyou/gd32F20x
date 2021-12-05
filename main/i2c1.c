/*!
    \file  i2c1.c
    \brief GPIO 模拟 I2C1
*/

#include "i2c1.h"
#include "systick.h"

/* I2C1 GPIO PORT GPIOB */
#define RCU_I2C_PORT	RCU_GPIOB
#define I2C1_GPIO_PORT	GPIOB
#define I2C1_SCL_PIN	GPIO_PIN_10
#define I2C1_SDA_PIN 	GPIO_PIN_11

#define I2C1_SCL_SET()     gpio_bit_set			(I2C1_GPIO_PORT, I2C1_SCL_PIN)
#define I2C1_SCL_RESET()   gpio_bit_reset		(I2C1_GPIO_PORT, I2C1_SCL_PIN)
#define I2C1_SDA_SET()     gpio_bit_set			(I2C1_GPIO_PORT, I2C1_SDA_PIN)
#define I2C1_SDA_RESET()   gpio_bit_reset		(I2C1_GPIO_PORT, I2C1_SDA_PIN)
#define READ_I2C1_SDA()    gpio_input_bit_get	(I2C1_GPIO_PORT, I2C1_SDA_PIN)

#define I2C1_SDA_OUT()     gpio_init(I2C1_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, I2C1_SDA_PIN)
#define I2C1_SDA_IN()      gpio_init(I2C1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, I2C1_SDA_PIN)

#define BASIC_DELAY_US     (5)

/*!
    \brief      模拟I2c初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c1_init(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_I2C_PORT);

    /* connect PB10 to I2C1_SCL */
    /* connect PB11 to I2C1_SDA */
    gpio_init(I2C1_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, I2C1_SCL_PIN | I2C1_SDA_PIN);

} /* i2c1_init() */

/*!
    \brief      I2C1 start
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c1_start(void)
{
    I2C1_SDA_OUT();     //设置SDA为输出
    I2C1_SDA_SET();
    I2C1_SCL_SET();
    delay_1us(BASIC_DELAY_US);
    
 	I2C1_SDA_RESET();	//发送Start信号
    delay_1us(BASIC_DELAY_US);
    
    I2C1_SCL_RESET();	//设置SCL为低电平,准备发送或接收数据
} /* i2c1_start() */

/*!
    \brief      I2C1 stop
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c1_stop(void)
{
    I2C1_SDA_OUT();		//设置SDA为输出
    I2C1_SDA_RESET();
 	delay_1us(BASIC_DELAY_US);
    
    I2C1_SCL_SET();
    delay_1us(BASIC_DELAY_US);
    
    I2C1_SDA_SET();		//发送Stop信号
    delay_1us(BASIC_DELAY_US);
} /* i2c1_stop() */

/*!
    \brief      主机等待从机发送Ack信号
    \param[in]  none
    \param[out] none
    \retval     1, 接收到应答
    			0, 未接收到应答
*/
uint8_t i2c1_wait_ack(void)
{
    uint8_t ucErrTime = 0;
    
    I2C1_SDA_IN();      //设置SDA为输入
    
    I2C1_SDA_SET();
    delay_1us(1);
    
    I2C1_SCL_SET();
    delay_1us(BASIC_DELAY_US);
    
    while (READ_I2C1_SDA())		//读取从机应答信号
    {
    	ucErrTime++;
    	if (ucErrTime > 250)
    	{
    		return 1;
    	} /* if (ucErrTime ... ) */
    } /* while() */
    
    I2C1_SCL_RESET();	//设置SCL为低电平
    
    return 0;
} /* i2c1_wait_ack() */

/*!
    \brief      主机发送Ack信号
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c1_ack(void)
{
    I2C1_SCL_RESET();
    I2C1_SDA_OUT();
    I2C1_SDA_RESET();		//主机拉低SDA,发送Ack信号
    delay_1us(BASIC_DELAY_US);
    
    I2C1_SCL_SET();
    delay_1us(BASIC_DELAY_US);
    
    I2C1_SCL_RESET();
} /* i2c1_ack() */

/*!
    \brief      send NAck
    \param[in]  none
    \param[out] none
    \retval     none
*/		    
void i2c1_nack(void)
{
    I2C1_SCL_RESET();
    I2C1_SDA_OUT();
    I2C1_SDA_SET();		//主机拉高SDA,发送NAck信号
    delay_1us(BASIC_DELAY_US);
    
    I2C1_SCL_SET();
    delay_1us(BASIC_DELAY_US);
    
    I2C1_SCL_RESET();
} /* i2c1_nack() */

/*!
    \brief      发送一个字节的数据
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c1_send_byte(uint8_t txd)
{
    uint8_t t;
    
    I2C1_SDA_OUT();
    I2C1_SCL_RESET();	//拉低SCL,开始数据传输
    
    for (t = 0; t < 8; t++)
    {   
    	if ((txd & 0x80) >> 7)
    	{
    		I2C1_SDA_SET();
    	} /* if txd ... */
    	else
    	{
    		I2C1_SDA_RESET();
    	} /* if txd ... else */
        txd <<= 1;
    	delay_1us(4);   //对TEA5767这三个延时都是必须的
    	
    	I2C1_SCL_SET();
    	delay_1us(BASIC_DELAY_US);
    	
    	I2C1_SCL_RESET();
    	delay_1us(2);
    } /* for t ... */	 
} /* i2c1_send_byte() */

/*!
    \brief      接收一个字节
    \param[in]  ack
    			ack=1时,发送ACK
    			ack=0时,发送nACK
    \param[out] none
    \retval     接收到的数据
*/ 
uint8_t i2c1_read_byte(unsigned char ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    
    I2C1_SDA_IN();//SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        I2C1_SCL_RESET(); 
        delay_1us(BASIC_DELAY_US);
    	
    	I2C1_SCL_SET();
        receive <<= 1;
    	
        if (READ_I2C1_SDA())
    	{
    		receive++;   
    	} /* if (READ_I2C1_SDA()) */
    	
    	delay_1us(BASIC_DELAY_US);
    } /* for (i ...) */
    
    if (!ack)
    {
        i2c1_nack();//发送NAck
    } /* if (!ack) */
    else
    {
        i2c1_ack(); //发送Ack
    } /* if (!ack) else */
    
    return receive;
} /* i2c1_read_byte() */

/* i2c read、write function example */
/*!
    \brief      write one byte to i2c slave device
    \param[in]  slave_addr: i2c slave device address
    \param[in]  write_address: salve's internal address to write to
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the slave device
    \param[out] none
    \retval     none
*/
void i2c1_byte_write(uint8_t slave_addr, uint8_t write_address, uint8_t* p_buffer)
{
    i2c1_start();
    i2c1_send_byte(slave_addr);		//发送从机地址 + Write
    i2c1_wait_ack();
    i2c1_send_byte(write_address);	//发送片内地址
    i2c1_wait_ack();
    i2c1_send_byte(*p_buffer);		//发送数据
    i2c1_wait_ack();
    i2c1_stop();					//产生一个停止条件
}

/*!
    \brief      read one byte from i2c slave device
    \param[in]  slave_addr: i2c slave device address
    \param[in]  read_address:  slave device's internal address to start reading from
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the slave device
    \param[out] none
    \retval     none
*/
void i2c1_byte_read(uint8_t slave_addr, uint8_t read_address, uint8_t* p_buffer)
{		  	    																 
    i2c1_start();
    i2c1_send_byte(slave_addr);		//发送从机地址 + Write
    i2c1_wait_ack();
    i2c1_send_byte(read_address);	//发送片内地址
    i2c1_wait_ack();
    i2c1_start();
    i2c1_send_byte(slave_addr+1);	//发送从机地址 + Read
    i2c1_wait_ack();
    *p_buffer=i2c1_read_byte(0);
    i2c1_stop();					//产生一个停止条件
}




