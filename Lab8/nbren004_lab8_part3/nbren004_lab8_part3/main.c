/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 8 Part 3
 * Description: Display input from photo sensor
 *
 * Created: 5/2/2019 6:07:15 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 


#include <avr/io.h>

const unsigned short MAX = 75;

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
    DDRB = 0x01; PORTB = 0x00;
    ADC_init();
    // Value of ADC register now stored in variable x.

    while (1)
    {
        if (ADC < (unsigned char)(MAX/2)) PORTB = 0x00;
        else PORTB = 0x01;
    }
}