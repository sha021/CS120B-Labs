/*
 * updatePortC.c
 *
 * Created: 4/16/2019 8:18:30 PM
 * Author : SIENA
 */ 

#include <avr/io.h>


int main(void)
{
    DDRC = 0xFF;    PORTC = 0x00;
    DDRA = 0x00;    PORTA = 0x01;
    unsigned char led = 0x00, button = 0x00;
    
    /* Replace with your application code */
    while (1) 
    {
        button = ~PINA & 0x01;
        if(button) led = 0xFF;
        else led = 0x00;
        PORTC = led;
    }
}

