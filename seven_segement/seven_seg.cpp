#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "l74hc595b.h"

#include "seven_seg.h"

seven_seg::seven_seg(uint8_t NUM_DIG, volatile uint8_t *TRAN_DDR, volatile uint8_t *TRAN_PORT, uint8_t TRAN_PIN_ST, l74hc595b *shi){
    shifti      = shi;
    num_digits  = NUM_DIG;
    DDR         = TRAN_DDR;
    PORT        = TRAN_PORT;
    PIN_ST      = TRAN_PIN_ST;
    digits          = (uint8_t*) malloc(sizeof(uint8_t)*num_digits);
    segment_code    = (uint8_t*) malloc(sizeof(uint8_t)*11);
    segment_code[0 ] = 0b11111100;
    segment_code[1 ] = 0b01100000;
    segment_code[2 ] = 0b11011010;
    segment_code[3 ] = 0b11110010;
    segment_code[4 ] = 0b01100110;
    segment_code[5 ] = 0b10110110;
    segment_code[6 ] = 0b10111110;
    segment_code[7 ] = 0b11100000;
    segment_code[8 ] = 0b11111110;
    segment_code[9 ] = 0b11110110;
    segment_code[10] = 0b00000001;
    now_segment = 0;
    ison = false;
    dot_pos = -1;

    for(uint8_t p=PIN_ST; p<PIN_ST+num_digits;p++){
        *DDR    |= (1<<p);
        *PORT   &= ~(1<<p);
        digits[p-PIN_ST] = 0;
    }

}
seven_seg::~seven_seg(){
    for(uint8_t p=PIN_ST; p<PIN_ST+num_digits;p++){
        *PORT   &= ~(1<<p);
        *DDR    &= ~(1<<p);
    }
    free(digits);
    free(segment_code);
}

void seven_seg::set_number(uint8_t pos, uint8_t number){
    digits[pos] = number;
}

void seven_seg::show(){
    uint8_t byte;
    for(uint8_t i=0; i<num_digits; i++){
        shifti->setregalloff();
        //shifti->setchipbyte(0,pgm_read_byte(&segment_code[digits[i]]));
        if(digits[i]<0 || digits[i]>9){byte=0;}
        else{byte = segment_code[digits[i]];}
        if(i==dot_pos){byte|=segment_code[10];}
        shifti->setchipbyte(0,byte);
        activate_tran(i);
        shifti->shiftout();
        _delay_ms(5);
        deactivate_tran(i);
    }
}

void seven_seg::activate_tran(uint8_t pos){*PORT &= ~(1<<(PIN_ST+pos));}
void seven_seg::deactivate_tran(uint8_t pos){*PORT |= (1<<(PIN_ST+pos));}

void seven_seg::next_segement(){
    uint8_t byte;
    deactivate_tran(now_segment);
    shifti->setregalloff();
    now_segment++;
    if(now_segment>=num_digits){now_segment=0;}
    if(digits[now_segment]<0 || digits[now_segment]>9){byte=0;}
    else{byte = segment_code[digits[now_segment]];}
    if(now_segment==dot_pos){byte|=segment_code[10];}
    shifti->setchipbyte(0,byte);
    shifti->shiftout();
    activate_tran(now_segment);
}
void seven_seg::on(){ison = true;}
void seven_seg::off(){ison = false;}

