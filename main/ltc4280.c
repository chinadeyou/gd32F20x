/*
    \file  LTC4280.c
    \brief LTC4280接口函数
*/

#include "ltc4280.h"
#include "i2c0.h"

extern uint8_t i2c_reg[32];

void ltc4280_read_control(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, CONTROL_REG, value);
}

void ltc4280_read_alert(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, ALERT_REG, value);
}

void ltc4280_read_status(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, STATUS_REG, value);
}

void ltc4280_read_fault(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, FAULT_REG, value);
}

void ltc4280_read_sense(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, SENSE_REG, value);
}

void ltc4280_read_source(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, SOURCE_REG, value);
}

void ltc4280_read_adin(uint8_t *value)
{
	ltc4280_byte_read(LTC4280_ADDR, ADIN_REG, value);
}


/*!
    \brief      write one byte to LTC4280
	\param[in]  slave_addr: LTC4280 address
    \param[in]  write_address: LTC4280 internal address to write to
	\param[in]  p_buffer: pointer to the buffer containing the data to be written to LTC4280
    \param[out] none
    \retval     none
*/
void ltc4280_byte_write(uint8_t slave_addr, uint8_t write_address, uint8_t* p_buffer)
{
	i2c0_start();
	i2c0_send_byte(slave_addr);		//发送从机地址 + Write
	i2c0_wait_ack();
	i2c0_send_byte(write_address);	//发送片内地址
	i2c0_wait_ack();
	i2c0_send_byte(*p_buffer);		//发送数据
	i2c0_wait_ack();
    i2c0_stop();					//产生一个停止条件
}


/*!
    \brief      read one byte from LTC4280
	\param[in]  slave_addr: LTC4280 address
    \param[in]  read_address: LTC4280 internal address to start reading from
    \param[in]  p_buffer: pointer to the buffer that receives the data read from LTC4280
    \param[out] none
    \retval     none
*/
void ltc4280_byte_read(uint8_t slave_addr, uint8_t read_address, uint8_t* p_buffer)
{		  	    																 
	i2c0_start();
	i2c0_send_byte(slave_addr);		//发送从机地址 + Write
	i2c0_wait_ack();
	i2c0_send_byte(read_address);	//发送片内地址
	i2c0_wait_ack();
	i2c0_start();
	i2c0_send_byte(slave_addr+1);	//发送从机地址 + Read
	i2c0_wait_ack();
	*p_buffer=i2c0_read_byte(0);
	i2c0_stop();					//产生一个停止条件
}

/*!
	\brief 	读取LTC4280电流，并保存在i2c_reg中
	\desc 	数据格式，前4位位整数，后四位为小数
*/
void ltc4280_current(void)
{
	float current = 0;
	uint8_t value = 0;
	
	float decimal;
	uint8_t decimal_hex;
	int8_t integer;
	
	/* 读压差 */
	ltc4280_read_sense(&value);
	/* 计算电流值大小 */
	current = value*151/1000.0;	/* value*151uV LSB/1000/1000/0.001欧 */
	
	integer = current;
	decimal = current - integer;
	decimal_hex = decimal/0.0625 + 0.5;
	if(decimal_hex > 15)
		decimal_hex = 15;
	
	i2c_reg[0x0a] = integer << 4 | decimal_hex;

	printf("ltc4280 current = %fA\r\n", current);
}

/*!
	\brief 	读取LTC4280电压，并保存在i2c_reg中
	\desc 	数据格式，前4位位整数，后四位为小数
*/
void ltc4280_voltage(void)
{
	float voltage = 0;
	uint8_t value = 0;
	
	float decimal;
	uint8_t decimal_hex;
	int8_t integer;
	
	/* 读取SOURCE脚电压 */
	ltc4280_read_source(&value);
	
	voltage = value*60.5/1000.0; 	/* 60.5mV LSB*/
	
	integer = voltage;
	decimal = voltage - integer;
	decimal_hex = decimal/0.0625 + 0.5;
	if(decimal_hex > 15)
		decimal_hex = 15;
	
	i2c_reg[5] = integer << 4 | decimal_hex;

	printf("ltc4280 voltage = %fV\r\n", voltage);
}

