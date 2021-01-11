/*
    \file  LTC4280.h
    \brief the header file of LTC4280.c
*/

#ifndef __LTC4280_H
#define __LTC4280_H

#include <stdio.h>
#include <gd32f20x.h>

#define LTC4280_ADDR	0xAA

#define CONTROL_REG		0x00
#define ALERT_REG		0x01
#define STATUS_REG		0x02
#define	FAULT_REG		0x03
#define	SENSE_REG		0x04
#define	SOURCE_REG		0x05
#define	ADIN_REG		0x06

void ltc4280_read_control(uint8_t *value);

void ltc4280_read_status(uint8_t *value);

void ltc4280_read_sense(uint8_t *value);

void ltc4280_read_source(uint8_t *value);


void ltc4280_byte_write(uint8_t slave_addr, uint8_t write_address, uint8_t* p_buffer);
void ltc4280_byte_read(uint8_t slave_addr, uint8_t read_address, uint8_t* p_buffer);


#endif  /* __LTC4280_H */


