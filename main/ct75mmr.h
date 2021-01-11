/*!
    \file  ct75mmr.h
    \brief the header file of ct75mmr.c
*/

#ifndef __CT75MMR_H
#define __CT75MMR_H

#include <stdio.h>
#include <gd32f20x.h>

#define CT75MMR_ADDR1 	0x90
#define CT75MMR_ADDR2 	0x92
#define CT75MMR_ADDR3 	0x94
#define CT75MMR_ADDR4 	0x96

int8_t ct75U16_read_temp(void);
int8_t ct75U17_read_temp(void);
int8_t ct75U18_read_temp(void);
int8_t ct75U19_read_temp(void);

void ct75mmr_byte_write(uint8_t slave_addr, uint8_t write_address, int16_t* p_buffer);
void ct75mmr_byte_read(uint8_t slave_addr, uint8_t read_address, int16_t* p_buffer);

#endif  /* __CT75MMR_H */


