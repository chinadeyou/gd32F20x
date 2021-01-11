/*
    \file  i2c2-slave.h
    \brief the header file of i2c0-slave.c
*/

#ifndef __I2C2_SLAVE_H
#define __I2C2_SLAVE_H

#include <stdio.h>
#include <gd32f20x.h>

#define I2C2_OWN_ADDRESS7      0x82

extern uint8_t i2c_reg[32];
extern uint8_t i2c2_data_transmit;
extern volatile uint8_t i2c2_offset;

void I2C2_EventIRQ_Handler(void);
void I2C2_ErrorIRQ_Handler(void);

void I2C2_Slave_Config(void);
void i2c2_set_own_addr(uint8_t addr);

extern uint8_t i2c2_recv_count;
extern uint8_t i2c2_recv_offset[];

#endif /* __I2C2_SLAVE_H */
