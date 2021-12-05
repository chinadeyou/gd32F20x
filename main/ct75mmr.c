/*!
    \file  ct75mmr.c
    \brief CT75MMR接口函数
    \desc 	CT75MMR 默认使用12位表示温度，低温阈值75度，高温阈值80度。
    		ALERT告警信号工作在比较模式，极性(POL = 0)，即：温度高于80度时，ALERT输出低，直到温度低于75度；
    		温度低于80度时，ALERT输出高。
*/

#include "ct75mmr.h"
#include "systick.h"
#include "i2c0.h"

/*!
    \brief	读取温度
    \desc 	温度为无符号整数。
*/
int8_t ct75U16_read_temp(void)
{
    int16_t p_buffer = 0;
    ct75mmr_byte_read(CT75MMR_ADDR1, 0, &p_buffer);

    return (p_buffer >> 4) * 0.0625;
}

int8_t ct75U17_read_temp(void)
{
    int16_t p_buffer = 0;
    ct75mmr_byte_read(CT75MMR_ADDR2, 0, &p_buffer);
    return (p_buffer >> 4) * 0.0625;
}

int8_t ct75U18_read_temp(void)
{
    int16_t p_buffer = 0;
    ct75mmr_byte_read(CT75MMR_ADDR3, 0, &p_buffer);
    return (p_buffer >> 4) * 0.0625;
}

int8_t ct75U19_read_temp(void)
{
    int16_t p_buffer = 0;
    ct75mmr_byte_read(CT75MMR_ADDR4, 0, &p_buffer);
    return (p_buffer >> 4) * 0.0625;
}

/*!
    \brief      write one byte to CT755MMR
    \param[in]  slave_addr: CT755MMR address
    \param[in]  write_address: CT755MMR internal address to write to
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to CT755MMR
    \param[out] none
    \retval     none
*/
void ct75mmr_byte_write(uint8_t slave_addr, uint8_t write_address, int16_t* p_buffer)
{
    i2c0_start();
    i2c0_send_byte(slave_addr);		//发送从机地址 + Write
    i2c0_wait_ack();
    i2c0_send_byte(write_address);	//发送片内地址
    i2c0_wait_ack();
    i2c0_send_byte(*p_buffer >> 8);	//发送first byte 
    i2c0_wait_ack();
    i2c0_send_byte(*p_buffer);		//发送second byte
    i2c0_wait_ack();
    i2c0_stop();					//产生一个停止条件
}

/*!
    \brief      read one byte from CT755MMR
    \param[in]  slave_addr: CT755MMR address
    \param[in]  read_address: CT755MMR internal address to start reading from
    \param[in]  p_buffer: pointer to the buffer that receives the data read from CT755MMR
    \param[out] none
    \retval     none
*/
void ct75mmr_byte_read(uint8_t slave_addr, uint8_t read_address, int16_t* p_buffer)
{		  	 
    uint8_t second = 0;
    i2c0_start();
    i2c0_send_byte(slave_addr);		//发送从机地址 + Write
    i2c0_wait_ack();
    i2c0_send_byte(read_address);	//发送片内地址
    i2c0_wait_ack();
    i2c0_start();
    i2c0_send_byte(slave_addr+1);	//发送从机地址 + Read
    i2c0_wait_ack();
    *p_buffer=i2c0_read_byte(1);	//接收ct75 温度 first byte并发送ack
    second = i2c0_read_byte(1);		//接收ct75 温度 second byte并发送ack
    i2c0_stop();					//产生一个停止条件
    
    *p_buffer = (*p_buffer << 8) | second;
}


