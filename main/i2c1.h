/*!
    \file  i2c1.h
    \brief the header file of i2c1.c
*/

#ifndef __I2C1_H
#define __I2C1_H

#include <stdio.h>
#include "gd32f20x.h"

/* configure the I2C interfaces */
void i2c1_init(void);
/* i2c1 开始信号 */
void i2c1_start(void);
/* i2c1 停止信号 */
void i2c1_stop(void);
/* i2c1 发送应答 */
void i2c1_ack(void);
/* i2c1 发送不应答 */
void i2c1_nack(void);
/* i2c1 等待从机应答 */
uint8_t i2c1_wait_ack(void);
/* i2c1 传输一个字节的数据 */
void i2c1_send_byte(uint8_t txd);
/* I2C1 接收一个字节的数据 */
uint8_t i2c1_read_byte(unsigned char ack);

void i2c1_byte_write(uint8_t slave_addr, uint8_t write_address, uint8_t* p_buffer);
void i2c1_byte_read(uint8_t slave_addr, uint8_t read_address, uint8_t* p_buffer);

#endif  /* __I2C1_H */
