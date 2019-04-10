/*
 * sha021_lab3_part4.c
 *
 * Created: 4/9/2019 9:45:46 PM
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
    DDRA = 0x00;    PORTA = 0xFF;
    DDRB = 0xFF;    PORTB = 0x00;
    DDRC = 0xFF;    PORTC = 0x00;
    
    unsigned char tmpB, tmpC;
    /* Replace with your application code */
    while (1) 
    {
        tmpB = 0;
        tmpC = 0;
        
        if (GetBit(PINA, 7)) tmpB = SetBit(tmpB, 3, 1);
        if (GetBit(PINA, 6)) tmpB = SetBit(tmpB, 2, 1);
        if (GetBit(PINA, 5)) tmpB = SetBit(tmpB, 1, 1);
        if (GetBit(PINA, 4)) tmpB = SetBit(tmpB, 0, 1);
        if (GetBit(PINA, 3)) tmpC = SetBit(tmpC, 7, 1);
        if (GetBit(PINA, 2)) tmpC = SetBit(tmpC, 6, 1);
        if (GetBit(PINA, 1)) tmpC = SetBit(tmpC, 5, 1);
        if (GetBit(PINA, 0)) tmpC = SetBit(tmpC, 4, 1);
        
        PORTB = tmpB;
        PORTC = tmpC;
    }

}

