/*
 * nbren004_lab2_part3.c
 *
 * Created: 4/4/2019 11:32:33 PM
 * Author : Nathan
 */ 

#include <avr/io.h>


int main(void)
{
     DDRA = 0x00; PORTA = 0xFF;        // Input initialized to 1
     DDRC = 0xFF; PORTC = 0x00;        // Output initialized to 0
     unsigned char tempA = 0x00;
     
    while (1) 
    {
        unsigned char spacesOpen = 0x00;
        tempA = PINA & 0x0F;
        
        spacesOpen = ~tempA & 0x0F;
        if (spacesOpen == 0x00) spacesOpen |= 0x80;
        
        PORTC = spacesOpen;
    }
}