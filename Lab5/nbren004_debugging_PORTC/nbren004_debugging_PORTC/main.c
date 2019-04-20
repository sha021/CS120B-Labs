/*
 * nbren004_debugging_PORTC.c
 *
 * Created: 4/16/2019 8:17:30 PM
 * Author : Nathan
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0x01;
    DDRC = 0xFF; PORTC = 0x00;
    
    unsigned char button, light = 0x00;
    
    while (1) 
    {
        button = ~PINA & 0x01;
        if (button) {
            if (PINC) light = 0x00;
            else light = 0xFF;
        }
        PORTC = light;
    }
}

