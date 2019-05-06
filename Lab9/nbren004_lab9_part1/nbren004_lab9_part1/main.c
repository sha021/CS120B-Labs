/* Nathan Brennan & nbren004@ucr.edu
 * Seung Eun Ha & sha021@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 9 Exercise 1
 * Description: Three-Note Sound Keyboard
 *
 * Created: 5/5/2019 5:22:56 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 

#include <avr/io.h>

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
    static double current_frequency; // Keeps track of the currently set frequency
    // Will only update the registers when the frequency changes, otherwise allows
    // music to play uninterrupted.
    if (frequency != current_frequency) {
        if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
        else { TCCR3B |= 0x03; } // resumes/continues timer/counter
        
        // prevents OCR3A from overflowing, using prescaler 64
        // 0.954 is smallest frequency that will not result in overflow
        if (frequency < 0.954) { OCR3A = (int)0xFFFF; }
        
        // prevents OCR0A from underflowing, using prescaler 64                    // 31250 is largest frequency that will not result in underflow
        else if (frequency > 31250) { OCR3A = 0x0000; }
        
        // set OCR3A based on desired frequency
        else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

        TCNT3 = 0; // resets counter
        current_frequency = frequency; // Updates the current frequency
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    // COM3A0: Toggle PB3 on compare match between counter and OCR0A
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    // WGM02: When counter (TCNT0) matches OCR0A, reset counter
    // CS01 & CS30: Set a prescaler of 64
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}



int main(void){
    
    DDRB = 0xFF;    PORTB = 0x00;
    DDRA = 0x00;    PORTA = 0x07;
    PWM_on();
    
    while (1) 
    {
        unsigned char c = ~PINA & 0x01, d = ~PINA & 0X02, e = ~PINA & 0x04;
        
        if (c && !d && !e) set_PWM(261.63);
        else if (!c && d && !e) set_PWM(293.66);
        else if (!c && !d && e) set_PWM(329.63);
        else set_PWM(0);
    }
}