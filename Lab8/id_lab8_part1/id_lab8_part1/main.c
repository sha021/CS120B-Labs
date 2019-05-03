/*
 * id_lab8_part1.c
 *
 * Created: 5/2/2019 4:33:16 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: setting this bit enables analog-to-digital conversion.
    // ADSC: setting this bit starts the first conversion.
    // ADATE: setting this bit enables auto-triggering. Since we are
    //        in Free Running Mode, a new conversion will trigger whenever
    //        the previous conversion completes.
}

int main(void)
{
    /* Replace with your application code */
    DDRA = 0x00; PORTA = 0x01;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0x03; PORTD = 0x00;
    ADC_init();
     // Value of ADC register now stored in variable x.

    while (1) 
    {
        PORTB = (char)ADC;
        PORTD = (char)((0x03) & (ADC >> 8)); 
    }
}

