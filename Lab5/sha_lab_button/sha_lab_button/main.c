/*
 * sha_lab_button.c
 *
 * Created: 4/16/2019 6:47:31 PM
 * Author : SIENA
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00;    PORTA = 0x01;
    DDRB = 0xFF;    PORTB = 0x00;
    unsigned char led = 0x00;
    unsigned char button = 0x00;
    /* Replace with your application code */
    while (1) 
    {
        button = ~PINA & 0x01;
        if (button) led = 0x02;
        else led = 0x01;
        PORTB = led;
    }
}

