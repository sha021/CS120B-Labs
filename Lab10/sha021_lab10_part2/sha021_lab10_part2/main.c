/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 10 Part 2
 * Description: Concurrent Light Display
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

unsigned char displayTL = 0, displayBL = 0;
const unsigned char TASKS_SIZE = 3;
const unsigned long TASKS_PERIOD = 1;
Task tasks[3];


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
    unsigned char output = 0;
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
            output = displayTL | displayBL;
            PORTB = output;
            break;
        default:
            break;
    }
    return state;
}


/* ----------------------- MAIN ----------------------- */

int main() {
    
    DDRB = 0xFF; PORTB = 0x00;
    
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
    tasks[i].state = C_Start;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &TickFct_combineLeds;
    
    TimerSet(TASKS_PERIOD);
    TimerOn();
    
    while (1) { }
    return 0;
}