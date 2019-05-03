/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 8 Part 4
 * Description: Display input from photo sensor, in a clean manner
 *
 * Created: 5/2/2019 6:57:53 PM
 * Author : Nathan Brenan & Seung Eun Ha
 */ 


#include <avr/io.h>

const unsigned char BASE = 11.5;

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
    
    unsigned short lightDisplay, lightLevel;
    ADC_init();

    while (1)
    {
        lightLevel = ADC;
        
        if (lightLevel < BASE) lightDisplay = 0x0000;
        else if (lightLevel < (BASE * 2)) lightDisplay = 0x0001;
        else if (lightLevel < (BASE * 3)) lightDisplay = 0x0003;
        else if (lightLevel < (BASE * 4)) lightDisplay = 0x0007;
        else if (lightLevel < (BASE * 5)) lightDisplay = 0x000F;
        else if (lightLevel < (BASE * 6)) lightDisplay = 0x001F;
        else if (lightLevel < (BASE * 7)) lightDisplay = 0x003F;
        else if (lightLevel < (BASE * 8)) lightDisplay = 0x007F;
        else if (lightLevel < (BASE * 9)) lightDisplay = 0x00FF;
        else if (lightLevel < (BASE * 10)) lightDisplay = 0x01FF;
        else lightDisplay = 0x03FF;
        
        PORTB = (char)lightDisplay;
        PORTD = (char)((0x03) & (lightDisplay >> 8));
    }
}

