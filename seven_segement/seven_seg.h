#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "l74hc595b.h"

#ifndef SEVEN_SEG_H_
#define SEVEN_SEG_H_

/*
const uint8_t segment_code[11] PROGMEM = {
	0b11111100,
	0b01100000,
	0b11011010,
	0b11110010,
	0b01100110,
	0b10110110,
	0b10111110,
	0b11100000,
	0b11111110,
	0b11110110,
	0b00000001
};*/

class seven_seg
{
private:
	l74hc595b *shifti;//(&DDRB,&PORTB,1,2,3,1);
	uint8_t num_digits;
	uint8_t *digits;
	uint8_t PIN_ST;
	volatile uint8_t *DDR;
	volatile uint8_t *PORT;
	uint8_t *segment_code;
	void activate_tran(uint8_t pos);
	void deactivate_tran(uint8_t pos);
	uint8_t now_segment;

public:
	seven_seg(uint8_t NUM_DIG, volatile uint8_t *TRAN_DDR, volatile uint8_t *TRAN_PORT, uint8_t TRAN_PIN_ST, l74hc595b *shi);
	~seven_seg();
	uint8_t ison;
	int8_t dot_pos;
	void set_number(uint8_t pos, uint8_t number);
	void show();
	void next_segement();
	void on();
	void off();
};

#endif
