 /* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 11 Part 3
 * Description: Display KeyPad inputs on LCD output
 *
 * Created: 5/19/2019 3:49:51 PM
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

/* ----------------------- STATE MACHINES ----------------------- */
enum keyPadStates{KP_Start, Button};
int keyPad_tick(int state) {
    unsigned char x;
    // Transitions
    switch (state) {
        case KP_Start :
            state = Button;
            message = "";
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
            x = GetKeypadKey();
            switch (x) {
                case '\0': break;
                case '1': message = "1"; break;
                case '2': message = "2"; break;

                case '3': message = "3"; break;
                case '4': message = "4"; break;
                case '5': message = "5"; break;
                case '6': message = "6"; break;
            
                case '7': message = "7"; break;
                case '8': message = "8"; break;
                case '9': message = "9"; break;
                case 'A': message = "A"; break;
            
                case 'B': message = "B"; break;
                case 'C': message = "C"; break;
            
                case 'D': message = "D"; break;
                case '*': message = "*"; break;
                case '0': message = "0"; break;
                case '#': message = "#"; break;
                default: message = "<Error>";  break; // Should never occur.
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