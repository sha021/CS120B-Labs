/*
 * nbren004_lab2_part2.c
 *
 * Created: 4/4/2019 10:35:33 PM
 * Author : Nathan
 */ 

#include <avr/io.h>


int main(void)
{
     DDRA = 0x00; PORTA = 0xFF;        // Input initialized to 1
     DDRC = 0xFF; PORTC = 0x00;        // Output initialized to 0
     unsigned char tempA0 = 0x00;
     unsigned char tempA1 = 0x00;
     unsigned char tempA2 = 0x00;
     unsigned char tempA3 = 0x00;
     
    while (1) 
    {
        unsigned char numSpacesOpen = 4;
        tempA0 = PINA & 0x01;
        tempA1 = PINA & 0x02;
        tempA2 = PINA & 0x04;
        tempA3 = PINA & 0x08;
        
        if (tempA0) numSpacesOpen--;
        if (tempA1) numSpacesOpen--;
        if (tempA2) numSpacesOpen--;
        if (tempA3) numSpacesOpen--;
        
        PORTC = numSpacesOpen;
    }
}

