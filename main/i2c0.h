/*!
    \file  i2c0.h
    \brief the header file of i2c0.c
*/

#ifndef __I2C0_H
#define __I2C0_H

#include <stdio.h>
#include <gd32f20x.h>

/* configure the I2C interfaces */
void i2c0_init(void);
/* i2c0 开始信号 */
void i2c0_start(void);
/* i2c0 停止信号 */
void i2c0_stop(void);
/* i2c0 发送应答 */
void i2c0_ack(void);
/* i2c0 发送不应答 */
void i2c0_nack(void);
/* i2c0 等待从机应答 */
uint8_t i2c0_wait_ack(void);
/* i2c0 传输一个字节的数据 */
void i2c0_send_byte(uint8_t txd);
/* I2C1 接收一个字节的数据 */
uint8_t i2c0_read_byte(unsigned char ack);




#endif  /* __I2C0_H */


