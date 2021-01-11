/*
    \file  i2c1-slave.h
    \brief the header file of i2c1-slave.c
*/

#ifndef __I2C1_SLAVE_H
#define __I2C1_SLAVE_H

#include <stdio.h>
#include <gd32f20x.h>

#define I2C1_OWN_ADDRESS7      0x82

extern uint8_t i2c_reg[32];
extern uint8_t i2c1_data_transmit;
extern volatile uint8_t i2c1_offset;

void I2C1_EventIRQ_Handler(void);
void I2C1_ErrorIRQ_Handler(void);

void I2C1_Slave_Config(void);
void i2c1_set_own_addr(uint8_t addr);

#endif /* __I2C1_SLAVE_H */
