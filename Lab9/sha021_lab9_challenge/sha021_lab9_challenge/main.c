/* Nathan Brennan & nbren004@ucr.edu
 * Seung Eun Ha & sha021@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 9 Exercise 2
 * Description: Play "Song of Storms" by Legend of Zelda Zeldatrack
 *
 * Created: 5/5/2019 9:42:52 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 

#include <avr/io.h>
#include "../../../Includes_PWM/frequencies.h"
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
    // AVR timer/counter controller register TCCR1
    TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
    // bit2bit1bit0=011: pre-scaler /64
    // 00001011: 0x0B
    // SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
    // Thus, TCNT1 register will count at 125,000 ticks/s

    // AVR output compare register OCR1A.
    OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
    // We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
    // So when TCNT1 register equals 125,
    // 1 ms has passed. Thus, we compare to 125.
    // AVR timer interrupt mask register
    TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

    //Initialize avr counter
    TCNT1=0;

    _avr_timer_cntcurr = _avr_timer_M;
    // TimerISR will be called every _avr_timer_cntcurr milliseconds

    //Enable global interrupts
    SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
    TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
    TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
    // CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
    _avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
    if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
        TimerISR(); // Call the ISR that the user uses
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}



// 0.954 hz is lowest musicNote possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the musicNote will stop the speaker from generating Zelda
void set_PWM(double musicNote) {
    static double current_musicNote; // Keeps track of the currently set musicNote
    // Will only update the registers when the musicNote changes, otherwise allows
    // music to play uninterrupted.
    if (musicNote != current_musicNote) {
        if (!musicNote) { TCCR3B &= 0x08; } //stops timer/counter
        else { TCCR3B |= 0x03; } // resumes/continues timer/counter
        
        // prevents OCR3A from overflowing, using prescaler 64
        // 0.954 is smallest musicNote that will not result in overflow
        if (musicNote < 0.954) { OCR3A = (int)0xFFFF; }
        
        // prevents OCR0A from underflowing, using prescaler 64                    // 31250 is largest musicNote that will not result in underflow
        else if (musicNote > 31250) { OCR3A = 0x0000; }
        
        // set OCR3A based on desired musicNote
        else { OCR3A = (short)(8000000 / (128 * musicNote)) - 1; }

        TCNT3 = 0; // resets counter
        current_musicNote = musicNote; // Updates the current musicNote
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


enum ZeldaStates { Init, Wait, Play } ZeldaState;
unsigned char playSong;
unsigned short beat; 
double musicNote;

void tick() {
    
    playSong = ~PINA & 0x01;
        
    switch (ZeldaState) {   // State Transitions
        case Init: 
            ZeldaState = Wait;
            break;
            
        case Wait:
            if (playSong) {
                ZeldaState = Play;
                PWM_on();
                beat = 0;
            }                
            else ZeldaState = Wait;
            break;
        
        case Play:
            if (beat > 362) {
                ZeldaState = Wait;
                PWM_off();
            }                
            else ZeldaState = Play;
            break;
            
        default:
            ZeldaState = Init;
            break;
    }
    
    switch (ZeldaState) {   // State Actions
        case Init:
            PORTC = 0x03;
            break;
            
        case Wait:
            PORTC = 0x0C;
            break;
        
        case Play:
            PORTC = 0XF0;
            
            if (beat < 4) musicNote = D3;
            else if (beat < 8) musicNote = A3;
            else if (beat < 24) musicNote = D4;
         
            else if (beat < 28) musicNote = D3;
            else if (beat < 32) musicNote = A3;
            else if (beat < 48) musicNote = D4;
            
            else if (beat < 60) musicNote = E4;
            else if (beat < 64) musicNote = F4;
            else if (beat < 68) musicNote = E4;
            else if (beat < 72) musicNote = F4;
            else if (beat < 76) musicNote = E4;
            else if (beat < 80) musicNote = C4;
            else if (beat < 95) musicNote = A3;
            
            else if (beat < 96) musicNote = REST;
            
            else if (beat < 104) musicNote = A3;
            else if (beat < 112) musicNote = D3;
            else if (beat < 116) musicNote = F3;
            else if (beat < 120) musicNote = G3;
            
            else if (beat < 143) musicNote = A3;
            
            else if (beat < 144) musicNote = REST;
             
            else if (beat < 152) musicNote = A3;
            else if (beat < 160) musicNote = D3;
            else if (beat < 164) musicNote = F3;
            else if (beat < 168) musicNote = G3;

            else if (beat < 191) musicNote = E3;
            else if (beat < 192) musicNote = REST;
            
            // Second half
            else if (beat < 196) musicNote = D3;
            else if (beat < 200) musicNote = A3;
            else if (beat < 216) musicNote = D4;
         
            else if (beat < 220) musicNote = D3;
            else if (beat < 224) musicNote = A3;
            else if (beat < 240) musicNote = D4;
            
            else if (beat < 252) musicNote = E4;
            else if (beat < 256) musicNote = F4;
            else if (beat < 260) musicNote = E4;
            else if (beat < 264) musicNote = F4;
            else if (beat < 268) musicNote = E4;
            else if (beat < 272) musicNote = C4;
            else if (beat < 287) musicNote = A3;
            
            else if (beat < 288) musicNote = REST;
            
            else if (beat < 296) musicNote = A3;
            else if (beat < 304) musicNote = D3;
            else if (beat < 308) musicNote = F3;
            else if (beat < 312) musicNote = G3;
            
            else if (beat < 329) musicNote = A3;
            else if (beat < 330) musicNote = REST;
            
            else if (beat < 338) musicNote = A3;
            else if (beat < 362) musicNote = D3;
            
                        
            set_PWM(musicNote);
            beat++;
            break;
    }
}



int main(void){
    
    DDRB = 0x40;    PORTB = 0x00;
    DDRA = 0x00;    PORTA = 0x01;
    DDRC = 0xFF;    PORTC = 0x00;
    ZeldaState = Init;
    
    TimerSet(50);
    TimerOn();
      
    while (1) 
    {
        tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}