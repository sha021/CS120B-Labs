 /* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 11 Part 1
 * Description: KeyPad output on Light Array
 *
 * Created: 5/14/2019 5:50:17 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define TASKS_SIZE 1
#define TASKS_PERIOD 10
#include "../../../Includes/bit.h"
#include "../../../Includes/scheduler.h"
#include "../../../Includes/timer.h"
#include "../../../Includes/keypad.h"


/* ----------------------- STATE MACHINES ----------------------- */
enum keyPadStates{Start, Button};
int keyPad(int state) {
    unsigned char x;
    // Transitions
    switch (state) {
        case Start :
            state = Button;
            break;
        case Button :
            state = Button;
            break;
        default :
            break;
    }
    // Actions
    switch (state) {
        case Button : 
            x = GetKeypadKey();
            switch (x) {
                case '\0': PORTB = 0x1F; break; // All 5 LEDs on
                case '1': PORTB = 0x01; break; // hex equivalent
                case '2': PORTB = 0x02; break;

                case '3': PORTB = 0x03; break;
                case '4': PORTB = 0x04; break;
                case '5': PORTB = 0x05; break;
                case '6': PORTB = 0x06; break;
                
                case '7': PORTB = 0x07; break;
                case '8': PORTB = 0x08; break;
                case '9': PORTB = 0x09; break;
                case 'A': PORTB = 0x0A; break;
                
                case 'B': PORTB = 0x0B; break;
                case 'C': PORTB = 0x0C; break;
                
                case 'D': PORTB = 0x0D; break;
                case '*': PORTB = 0x0E; break;
                case '0': PORTB = 0x00; break;
                case '#': PORTB = 0x0F; break;
                default: PORTB = 0x1B;  break; // Should never occur. Middle LED off.
            }
        default : 
            break;                            
    }
    return state;
}



/* ----------------------- MAIN ----------------------- */

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; 
	DDRA = 0xF0; PORTA = 0x0F; 
    
    tasks[0].state = Start;
    tasks[0].period = 10;
    tasks[0].elapsedTime = tasks[0].period;
    tasks[0].TickFct = &keyPad;
    
    TimerSet(TASKS_PERIOD);
    TimerOn();
	while(1) {}
}