/*!
    \file  gt24c256.h
    \brief the header of file gt24c256.c
*/

#ifndef __GT24C256_H
#define __GT24C256_H

#include <stdio.h>
#include <gd32f20x.h>

#define GT24C256_ADDR 		0xA0

#define I2C_OK              0
#define I2C_FAIL            1
#define I2C2_SPEED          100000
#define I2C2_OWN_ADDR7		0x82

#define EEP_FIRST_PAGE      0x00
#define GT24C256_PAGE_SIZE	64
#define GT24C256_SIZE		32768

void i2c2_config(void);

void gt24c256_init(uint8_t slave_addr);

void gt24_eeprom_buffer_write(uint8_t* p_buffer, uint16_t write_address, uint16_t number_of_byte);
void gt24_eeprom_byte_write(uint8_t* p_buffer, uint16_t write_address);
void gt24_eeprom_page_write(uint8_t* p_buffer, uint16_t write_address, uint8_t number_of_byte);
void gt24_eeprom_buffer_read(uint8_t* p_buffer, uint16_t read_address, uint16_t number_of_byte);
void gt24_eeprom_wait_standby_state(void);
       
void gt24c256_byte_write(uint16_t write_address, uint8_t* p_buffer);
void gt24c256_byte_read(uint16_t read_address, uint8_t* p_buffer);

#endif /* __GT24C256_H */
