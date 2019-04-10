/*
 * nbren004_lab3_pt4.c
 *
 * Created: 4/9/2019 9:57:51 PM
 * Author : Nathan
 */ 

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    
    unsigned char lowerA, upperA;
    
    while (1) 
    {
        lowerA = PINA & 0x0F;
        upperA = PINA & 0xF0;
        
        PORTB = upperA >> 4;
        PORTC = lowerA << 4;
    }
}