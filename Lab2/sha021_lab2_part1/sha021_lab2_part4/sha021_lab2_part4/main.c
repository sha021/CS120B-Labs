/*
 * sha021_lab2_part4.c
 *
 * Created: 4/5/2019 3:00:16 AM
 * Author : SIENA
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00;    PORTA = 0xFF;
    DDRB = 0x00;    PORTB = 0xFF;
    DDRC = 0x00;    PORTC = 0xFF;
    DDRD = 0xFF;    PORTD = 0x00;
    
    unsigned char tmpA ;
    unsigned char tmpB ;
    unsigned char tmpC ;
    unsigned char tmpD0;
    unsigned char tmpD1;
    unsigned char averageWeight = 0x00;
    
    const unsigned char MAX_WEIGHT = 0x8C;
    const unsigned char MAX_DIFFERENCE = 0x50;
    
    while (1) 
    {
        tmpA = PINA & 0xFF;
        tmpB = PINB & 0xFF;
        tmpC = PINC & 0xFF;
        
        tmpD0 = tmpD0 & 0x00;       // over MAX_WEIGHT alert 
        tmpD1 = tmpD1 & 0x00;       // over MAX_DIFFERENCE alert
        
        averageWeight = (tmpA + tmpB + tmpC) / 3;
        
        tmpD0 = ((tmpA + tmpB + tmpC) > MAX_WEIGHT) ? 1 : 0;        
        tmpD1 = (abs(tmpA - tmpC) > MAX_DIFFERENCE) ? 2 : 0;
        
        PORTD = (tmpD0 + tmpD1) | averageWeight; 
    
    }
    
    return 0;
    
}

