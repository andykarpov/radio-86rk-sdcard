// SD Controller for Computer "Radio 86RK" / "Apogee BK01"
// (c) 10-05-2014 vinxru (aleksey.f.morozov@gmail.com)

#include <avr/io.h>
#include "proto.h"

void wait(void) {
    // Ждем перепад 0->1
    while( !(PINC && (1 << PC5)) );
    while( PINC && (1 << PC5) ); 
//    while((PINC&0x3F)==0); 
//    while((PINC&0x3F)==0x20);
    if((PINC&0x3F)==0) return;
    asm volatile("RJMP 0");
}

void sendStart(uint8_t c) {
    wait();
    DATA_OUT
    PORTD = c;
}

void recvStart(void) {
    wait();
    DATA_IN
    PORTD = 0xFF;
} 

uint8_t wrecv(void) {
    wait();
    return PIND;
}

void send(uint8_t c) {
    wait();
    PORTD = c;
}
