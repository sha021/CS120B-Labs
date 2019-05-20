 /* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 11 Part 2
 * Description: ???
 *
 * Created: 5/14/2019 5:50:17 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 


#include <avr/io.h>
#include "io.h"
#include <string.h>
#include <stdio.h>


#define TASKS_SIZE 1
#define TASKS_PERIOD 1000
#include "../../../Includes/scheduler.h"
#include "../../../Includes/timer.h"

const unsigned char* WORD = "CS120B is Legend... wait for it DARY!";

unsigned char count = 0;
/* ----------------------- STATE MACHINES ----------------------- */
enum DisplayStates{Start, LCDON};
int display(int state) {
    // Transitions
    switch (state) {
        case Start :
            state = LCDON;
            break;
        case LCDON :
            state = LCDON;
            break;
    }
    // Actions
    switch (state) {
        case LCDON : 
            //count = 0;
            LCD_DisplayString(1, WORD);
            
            if (strlen(WORD) > 15 ) WORD = WORD + 16;
         //   else if (strlen(WORD) > 0) WORD = "";
            else WORD = "CS120B is Legend... wait for it DARY!";
            
           break; 
        default : break;                            
    }    
    return state;
}




/* ----------------------- MAIN ----------------------- */

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; 
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines

    tasks[0].state = Start;
    tasks[0].period = 1000;
    tasks[0].elapsedTime = tasks[0].period;
    tasks[0].TickFct = &display;
        
    TimerSet(1000);
    TimerOn();
    LCD_init();
                         
	while(1) {}
	
}
