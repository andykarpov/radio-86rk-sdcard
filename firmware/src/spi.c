// SD Controller for Computer "Radio 86RK" / "Apogee BK01"
// (c) 10-05-2014 vinxru (aleksey.f.morozov@gmail.com)

// Based on sources CC Dharmani, Chennai (India)
// 1 May 2013 vinxru

#include "spi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#ifndef SPI2X
#define SPI2X 0
#endif
         
void spi_init(void) {
  SPCR = 0x52; // setup SPI: Master mode, MSB first, SCK phase low, SCK idle low
  SPSR = 0x00;    
}
                    
uint8_t spi_transmit(uint8_t data) {
  SPDR = data;  
  while(!(SPSR & 0x80)); // Wait for transmission complete
  return SPDR;
}

uint8_t spi_receive(void) {
  SPDR = 0xFF;
  while(!(SPSR & 0x80)); // Wait for reception complete
  return SPDR;
}

void spi_highSpeed(void) {
  SPCR = 0x50;
  SPSR |= (1<<SPI2X);
  _delay_ms(1);
}
