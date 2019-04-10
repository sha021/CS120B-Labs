/*
 * sha021_lab3_lab5.c
 *
 * Created: 4/9/2019 10:27:22 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}


int main(void)
{
    DDRD = 0x00;    PORTD = 0xFF;
    DDRB = 0x06;    PORTB = 0x01;
    
    unsigned short weight;
    unsigned char tmpB, i;
    
    while (1) 
    {
        weight = 0;
        tmpB = 0;
        
        weight = (PIND * 2) + (GetBit(PINB, 0));
        
        if (weight >= 70) tmpB = SetBit(tmpB, 1, 1);
        else if (weight > 5) tmpB = SetBit(tmpB, 2, 1);
        
        PORTB = tmpB;
    }
}

