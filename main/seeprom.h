/*!
    \file  seeprom.h
    \brief the header of file seeprom.c
*/

#ifndef __SEEPROM_H
#define __SEEPROM_H

#include <stdio.h>
#include <gd32f20x.h>

#define SEEPROM_ADDR		0xA0
#define SEEPROM_INFO_SIZE	16384

void seeprom_update(void);

#endif /* __SEEPROM_H */
