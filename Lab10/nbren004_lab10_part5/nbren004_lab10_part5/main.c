/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 10 Part 5
 * Description: Synchronous Counter
 *
 * Created: 5/12/2019 8:50:37 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define TASKS_SIZE 1

typedef struct Task {
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFunc)(int);    
} Task;

const unsigned long TASKS_PERIOD = 10;
unsigned char display;
unsigned short count;

Task tasks[TASKS_SIZE];


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
    for (i = 0; i < TASKS_SIZE; i++) {
        if (tasks[i].elapsedTime >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFunc(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += TASKS_PERIOD;
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


/* ----------------------- STATE MACHINE ----------------------- */

enum CounterStates { B_Start, Stay, Increment, Decrement };
int counterTick(int state) {
    unsigned char up = ~PINA & 0x01, down = ~PINA & 0x02;
    // Transitions
    switch (state) {
        case B_Start:
            state = Stay;
            display = 0x07;
            break;
        case Stay:
            if ((up || down) && !(up && down)) {
                if (up && !down) state = Increment;
                if (!up && down) state = Decrement;
                count = 0;
            }            
            else state = Stay;
            break;
        case Increment:
        case Decrement:
            if (up && !down) state = Increment;
            else if (!up && down) state = Decrement;
            else state = Stay;
            break;
        default:
            state = B_Start;
            break;
    }
    // Actions
    switch (state) {
        case Stay:
            if (up && down) display = 0x00;                 // Reset
            PORTD = 0x00;
            break;
        case Increment:
            if (up && down) display = 0x00;                 // Reset
            else if (display < 9) {
                if (count <= 300) {
                    if (!((count) % 100)) display++;        // Up Slow
                    PORTD = 0x01;            
                }                
                else if (!((count - 300) % 40)) {
                    display++;                              // Up Fast
                    PORTD = 0x02;
                }                    
            }            
            count++;
            break;
        case Decrement:
            if (up && down) display = 0x00;                 // Reset
            else if (display > 0) {
                if (count <= 300) {
                    if (!((count) % 100)) display--;        // Down Slow
                    PORTD = 0x01;
                }
                else if (!((count - 300) % 40)) { 
                    display--;                              // Down Fast
                    PORTD = 0x02;
                }                    
            }            
            count++;
            break;
        default:
            break;
    }
    PORTB = display;
    return state;
}


/* ----------------------- MAIN ----------------------- */

int main(void) {
    
    DDRA = 0x07; PORTA = 0x03;
    DDRB = 0x0F; PORTB = 0x00;
    DDRD = 0x03; PORTD = 0x00;      // <---- Used for debugging
    
    unsigned char i = 0;
    tasks[i].state = B_Start;
    tasks[i].period = TASKS_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFunc = &counterTick;
    
    TimerSet(TASKS_PERIOD);
    TimerOn();
    
    while (1) {}
}