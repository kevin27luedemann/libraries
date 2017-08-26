/*
l74hc595 lib 0x01

copyright (c) Davide Gironi, 2011

References: bildr 74hc595 library for arduino
	http://bildr.org/2011/08/74hc595-breakout-arduino/

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include "l74hc595b.h"


l74hc595b::l74hc595b(volatile uint8_t *DDRREG, volatile uint8_t *PORTREG, uint8_t SER, uint8_t RCLK, uint8_t SRCLK, uint8_t REGS){
   DDR      = DDRREG; 
   PORT     = PORTREG;
   SERPIN   = SER;
   RCLKPIN  = RCLK;
   SRCLKPIN = SRCLK;
   REGNUMBER= REGS;
   icarray  = (uint8_t *) malloc(sizeof(uint8_t)*REGNUMBER);

	//output
	*DDR |= (1 << SERPIN);
	*DDR |= (1 << RCLKPIN);
	*DDR |= (1 << SRCLKPIN);
	//low
	*PORT &= ~(1 << SERPIN);
	*PORT &= ~(1 << RCLKPIN);
	*PORT &= ~(1 << SRCLKPIN);
	//init registers
	setregalloff();
}

l74hc595b::~l74hc595b(){
	//output
	*DDR &= ~(1 << SERPIN);
	*DDR &= ~(1 << RCLKPIN);
	*DDR &= ~(1 << SRCLKPIN);
    free(icarray);
}

/*
 * set all registers off
 */
void l74hc595b::setregalloff() {
	for(uint16_t i = 0; i < REGNUMBER*8; i++){setreg(i, 0);}
}

/*
 * set all registers on
 */
void l74hc595b::setregallon() {
	for(uint16_t i = 0; i < REGNUMBER*8; i++){setreg(i, 1);}
}

/*
 * set a register on or off
 */
void l74hc595b::setreg(uint16_t regindex, uint8_t val) {
	//get valid byteindex, first byte is for first ic attached
	uint16_t byteindex = (REGNUMBER-1)-regindex/8;
	//bit address 0 is Qa
	uint8_t bitindex = (8-1) - regindex % 8;

	uint8_t current = icarray[byteindex];

	current &= ~(1 << bitindex); //clear the bit
	current |= val << bitindex; //set the bit

	icarray[byteindex] = current; //set the new value
}

/*
 * set all ports of a chip
 */
void l74hc595b::setchipbyte(uint8_t chipindex, uint8_t val) {
	icarray[REGNUMBER-1-chipindex] = val; //set the new value
}

void l74hc595b::setregon(uint8_t regindex){setreg(regindex,1);}
void l74hc595b::setregoff(uint8_t regindex){setreg(regindex,0);}

/*
 * shift out data
 */
void l74hc595b::shiftout() {
	*PORT &= ~(1 << RCLKPIN); //set the register-clock pin low
	for(uint8_t i = 0; i < REGNUMBER; i++){
		//iterate through the bits in each registers
		for(uint8_t j = 0; j < 8; j++){
			*PORT &= ~(1 << SRCLKPIN); //set the serial-clock pin low
			uint8_t val = (icarray[i] & (1 << j))>>j;
			*PORT |= (val << SERPIN);
			*PORT |= (1 << SRCLKPIN); //set the serial-clock pin high
			*PORT &= ~(val << SERPIN); //set the datapin low again
		}
	}
	*PORT |= (1 << RCLKPIN); //set the register-clock pin high to update the output of the shift-register
}
