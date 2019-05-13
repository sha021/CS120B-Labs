/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 10 Part 3
 * Description: Concurrent Light Display + Sound
 *
 * Created: 5/7/2019 5:01:22 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


//#define TASKS_SIZE 3


typedef struct Task_struct {
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct)(int);
} Task;

unsigned char displayTL = 0, displayBL = 0, sound;
const unsigned char TASKS_SIZE = 4;
const unsigned long TASKS_PERIOD = 1;
Task tasks[4];


/* ----------------------- TIMER ----------------------- */

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
    unsigned char i;
    for (i = 0; i < TASKS_SIZE; ++i) {
        if (tasks[i].elapsedTime >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += 1;
    }
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

/*----------------------------- PWM------------------------------ */

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



/* ----------------------- STATE MACHINES ----------------------- */

enum ThreeLedStates { TL_Start, TL0, TL1, TL2 } ;    
int TickFct_threeLeds(int state) {
    // Transitions
    switch (state) {
        case TL_Start:
            state = TL0;
            break;
        
        case TL0:
            state = TL1;
            break;
        
        case TL1:
            state = TL2;
            break;
        
        case TL2:
            state = TL0;
            break;
        
        default: 
            state = TL_Start;
            break;
    }
    // Actions
    switch(state){        
        case TL0:
            displayTL = 0x01;
            break;
            
        case TL1:
            displayTL = 0x02;
            break;
            
        case TL2:
            displayTL = 0x04;
            break;
            
        default: 
            break;
    }
    //PORTB = displayTL;
    return state;
}

enum blinkLedStates { BL_Start, Off, On } ;
int TickFct_blinkLed(int state) {    
    // Transitions
    switch (state) {
        case BL_Start:
            state = Off;
            break;
        
        case Off:
            state = On;
            break;
        
        case On:
            state = Off;
            break;
        
        default: 
            state = BL_Start;
            break;        
    }
    // Actions
    switch(state){
        case Off:
            displayBL = 0x00;
            break;
        
        case On:
            displayBL = 0x08;
            break;
            
        default:
            break;
    }
    return state;
}

enum CombineLedStates { C_Start, Combine } ;
int TickFct_combineLeds(int state) {
    // Transitions
    switch (state) {
        case C_Start:
            state = Combine;
            break;
        case Combine:
            state = Combine;
            break;
        default:
            state = C_Start;
            break;
    }    
    // Actions
    switch (state) {
        case Combine:
            if (sound) set_PWM(262.63);
            else set_PWM(0);
            PORTB = displayTL | displayBL;
            break;
        default:
            break;
    }
    return state;
}

enum OscillateSound{O_Start, S_Off, S_Toggle};
 int TickFct_oscillate(int state){   
     switch (state) {
         case O_Start:
            sound = 0;
            state = S_Off;            
            break;
         
         case S_Toggle:
            if (!(~PINA & 0x04)) {
                state = S_Off;
            }                
            else {
                state = S_Toggle;
            }                
            break;
         
         case S_Off:
            if (~PINA & 0x04) {
                state = S_Toggle;
            }                
            else {
                state = S_Off;
            }                
            break;
            
        default: break;
     }
     
     switch (state) {
         case S_Off:
            sound = 0;
            break; 
         
         case S_Toggle: 
            sound = !sound;
            break;
            
        default: sound = 0;
            break;
     }            
  
     return state;
 }


/* ----------------------- MAIN ----------------------- */

int main() {
    
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    
    unsigned char i = 0;
    
    tasks[i].state = TL_Start;
    tasks[i].period = 1000;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_threeLeds;
    i++;
    
    tasks[i].state = BL_Start;
    tasks[i].period = 1000;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_blinkLed;
    i++;
    
    tasks[i].state = O_Start;
    tasks[i].period = 2;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_oscillate; 
    i++;

    tasks[i].state = C_Start;
    tasks[i].period = 1;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_combineLeds;
   
    TimerSet(1);
    TimerOn();
    PWM_on();

    while (1) {}
    
    return 0;
        
}