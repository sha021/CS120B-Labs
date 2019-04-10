/*
 * nbren004_lab3_pt5.c
 *
 * Created: 4/9/2019 10:23:47 PM
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
    DDRB = 0x06; PORTB = 0x01;
    DDRD = 0x00; PORTD = 0xFF;

    unsigned short weight;
    unsigned char signal;
    
    while (1) 
    {
        signal = 0x00;
        weight = (PINB & 0x01) + (PIND << 1);
        
        if (weight >= 70) {
            signal = SetBit(signal, 2, 1);
        } else if (weight < 70 && weight > 5) {
            signal = SetBit(signal, 1, 1);
        }
        // Add one to keep PORTB0 AT 1
        PORTB = signal + 1;
    }
}