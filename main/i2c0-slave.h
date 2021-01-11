/*
    \file  i2c0-slave.h
    \brief the header file of i2c0-slave.c
*/

#ifndef __I2C0_SLAVE_H
#define __I2C0_SLAVE_H

#include <stdio.h>
#include <gd32f20x.h>

#define I2C0_OWN_ADDRESS7      0x82

extern uint8_t i2c_reg[32];
extern uint8_t i2c0_data_transmit;
extern volatile uint8_t i2c0_offset;

void I2C0_EventIRQ_Handler(void);
void I2C0_ErrorIRQ_Handler(void);

void I2C0_Slave_Config(void);
void i2c0_set_own_addr(uint8_t addr);

#endif /* __I2C1_SLAVE_H */
