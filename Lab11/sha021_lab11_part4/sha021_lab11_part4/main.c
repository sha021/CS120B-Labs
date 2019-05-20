 /* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 11 Part 4
 * Description: Edit display on KeyPad through LCD output
 *
 * Created: 5/19/2019 8:30:00 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#define TASKS_SIZE 2
#define TASKS_PERIOD 100
#include "bit.h"
#include "scheduler.h"
#include "timer.h"
#include "keypad.h"
#include "io.h"

char* message;
unsigned char position;


/* ----------------------- STATE MACHINES ----------------------- */
enum keyPadStates{KP_Start, Button};
int keyPad_tick(int state) {
    static char input;
    unsigned char mutate;
    // Transitions
    switch (state) {
        case KP_Start :
            state = Button;
            message = "Type over me!                   ";
            position = 0;
            mutate = 0;
            break;
        case Button :
            state = Button;
            break;
        default:
            break;
    }
    // Actions
    switch (state) {
        case Button :
            input = GetKeypadKey();
            if (input) mutate = 1;
            else mutate = 0;
            
            if (mutate) {
                message[position] = input;
                if (position >= 31) position = 0;
                else position++;
            }            
            break;
        default : 
            break;
    }
    return state;
}

enum LCD_States { LCD_Start, Display };
int LCD_tick(int state) {
    // Transitions
    switch (state) {
        case LCD_Start:
            state = Display;
            break;
        case Display:
            state = Display;
            break;
        default:
            state = LCD_Start;
            break;
    }
    // Actions
    switch (state) {
        case Display:
            LCD_DisplayString(1, message);
            LCD_Cursor(position + 1);

            break;
        default:
            break;
    }
    return state;
}


int main(void)
{
    DDRA = 0xF0; PORTA = 0x0F;  // Keypad input
    DDRC = 0xFF; PORTC = 0x00;  // LCD data lines
    DDRD = 0xFF; PORTD = 0x00;  // LCD control lines
    
    unsigned char i = 0;
    tasks[i].state = KP_Start;
    tasks[i].period = TASKS_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &keyPad_tick;
    i++;
    tasks[i].state = LCD_Start;
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