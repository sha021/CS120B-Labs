/*
 * sha021_lab_chip.c
 *
 * Created: 4/16/2019 5:22:00 PM
 * Author : SIENA
 */ 

#include <avr/io.h>


int main(void)
{
    DDRB = 0xFF;    PORTB = 0x00;
    /* Replace with your application code */
    while (1) 
    {
        PORTB = 0x0F;
    }
}

