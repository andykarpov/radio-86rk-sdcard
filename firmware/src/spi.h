// SD Controller for Computer "Radio 86RK" / "Apogee BK01"
// (c) 10-05-2014 vinxru (aleksey.f.morozov@gmail.com)

// Based on sources CC Dharmani, Chennai (India)
// 1 May 2013 vinxru

#ifndef _SPI_ROUTINES_H_
#define _SPI_ROUTINES_H_

#include "common.h"
#include <stdint.h>

void spi_init(void);
uint8_t spi_transmit(uint8_t);
uint8_t spi_receive(void);
void spi_highSpeed(void);

#endif
