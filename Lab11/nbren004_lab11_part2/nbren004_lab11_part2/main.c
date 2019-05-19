 /* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 11 Part 2
 * Description: Display "How I Met Your Mother" reference on LCD
 *
 * Created: 5/19/2019 1:25:54 AM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#define TASKS_SIZE 1
#define TASKS_PERIOD 1000
// #include "../../../Includes/bit.h"
#include "../../../Includes/scheduler.h"
#include "../../../Includes/timer.h"
#include "../../../Includes/io.h"

char* message;

enum LCD_States { Start, Display };
int LCD_tick(int state) {
    // Transitions
    switch (state) {
        case Start:
            state = Display;
            message = "";
            break;
        case Display:
            state = Display;
            break;
        default:
            state = Start;
            break;
    }
    // Actions
    switch (state) {
        case Display:
            // Starting at position 1, scroll string
            for (int i = 0; i < 16; i++) {
                message++;
                if (strlen(message) <= 0) {
                    message = "CS120B is Legend... wait for it                 DARY!";
                    break;
                }
            }
            LCD_DisplayString(1, message);
            break;
        default:
            break;
    }
    return state;
}


int main(void)
{
    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
    
    unsigned char i = 0;
    tasks[i].state = Start;
    tasks[i].period = TASKS_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &LCD_tick;
    i++;
    
    TimerSet(TASKS_PERIOD);
    TimerOn();
    // Initializes the LCD display
    LCD_init();
    
    while (1) {}
}