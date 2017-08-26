/*
l74hc595 lib 0x01

copyright (c) Davide Gironi, 2011

References: bildr 74hc595 library for arduino
	http://bildr.org/2011/08/74hc595-breakout-arduino/

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <avr/io.h>

#ifndef L74HC595_H_
#define L74HC595_H_

class l74hc595b
{
private:
    volatile uint8_t *DDR;
    volatile uint8_t *PORT;
    uint8_t SERPIN;
    uint8_t RCLKPIN;
    uint8_t SRCLKPIN;
    uint8_t *icarray;

public: 
    uint8_t REGNUMBER;
    l74hc595b(volatile uint8_t *DDRREG, volatile uint8_t *PORTREG, uint8_t SER, uint8_t RCLK, uint8_t SRCLK, uint8_t REGS);
    ~l74hc595b();
    void setregalloff();
    void setreg(uint16_t regindex, uint8_t val);
    void setregallon();
    void setregon(uint8_t regindex);
    void setregoff(uint8_t regindex);
    void setchipbyte(uint8_t chipindex, uint8_t val);
    void shiftout();
};

#endif
