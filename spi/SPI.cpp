/*
 * Created by Kevin Luedemann
 * 09.09.2017 12:27
 */

#include "SPI.h"

SPI::SPI(){
    SPCR = (1<<SPE) | (1<<MSTR);
    SPSR = (1<<SPI2X);
}

SPI::~SPI(){
    SPCR = 0;
    SPSR = 0;

}

inline static uint8_t SPI::transfer(uint8_t d){
    SPDR = d;
    asm volatile("nop");
    while (!(SPSR & (1<<SPIF)));
    return SPDR;
}

inline static void SPI::transfer(void *buf, size_t count) {
if (count == 0) return;
uint8_t *p = (uint8_t *)buf;
SPDR = *p;
while (--count > 0) {
  uint8_t out = *(p + 1);
  while (!(SPSR & (1<<SPIF))) ;
  uint8_t in = SPDR;
  SPDR = out;
  *p++ = in;
}
while (!(SPSR & _BV(SPIF))) ;
*p = SPDR;
}
