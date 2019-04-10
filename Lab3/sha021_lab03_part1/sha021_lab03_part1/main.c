/*
 * sha021_lab03_part1.c
 *
 * Created: 4/9/2019 5:15:37 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
    return (b ? x |(0x01 << k) : x & !(0x01 <<k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}    
 
int main(void)
{
    DDRA = 0x00;    PORTA = 0xFF;
    DDRB = 0x00;    PORTB = 0xFF;
    DDRC = 0xFF;    PORTC = 0x00;
    
    unsigned char i;
    unsigned char tempC = 0x00;
    
    /* Replace with your application code */
    while (1) 
    {
        for (i = 0; i < 8; ++i){
            if (GetBit(PINA, i)>>i) {
                tempC += 0x01;
            }
        }
        
        for (i = 0; i < 8; ++i){
            if (GetBit(PINB, i)>>i) {
                tempC += 0x01;
            }
        }
        
        PORTC = tempC;
        
    }
}

