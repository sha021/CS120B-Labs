/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 11 Part 5
 * Description: Interactive LCD Game
 *
 * Created: 5/19/2019 10:32:55 PM
 * Author : Nathan Brennan & Seung Eun Ha
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#define TASKS_SIZE 3
#define TASKS_PERIOD 100
#include "bit.h"
#include "scheduler.h"
#include "timer.h"
#include "io.h"

char* arena;
unsigned char playing, gameOver, cursor;


/* ----------------------- STATE MACHINES ----------------------- */

enum OnPauseStates { O_Start, Play, Pause };
int onPause_tick(int state){
    unsigned playButton = ~PINA & 0x01;
    // Transitions
    switch (state) {
        case O_Start:
            state = Pause;
            break;
        case Pause:
            if (playButton) state = Play;
            else state = Pause;
            break;
        case Play:
            if (playButton || gameOver) state = Pause;
            else state = Play;
            break;
        default: 
            state = O_Start;
            break;
    }
    // Actions
    switch (state) {
        case Pause:
            playing = 0;
            break;
        case Play:
            playing = 1;
            break;
        default:
            break;
    }
    return state;
}

enum ControlStates { C_Start, Stay, Up, Down };
int controlPosition_tick(int state) {
    unsigned char upBtn = ~PINA & 0x04, downBtn = ~PINA & 0x02;
    // Transitions
    switch (state) {
        case C_Start:
            state = Stay;
            cursor = 2;
            break;
        case Stay:
            state = Stay;
            if (playing) {
                if (upBtn && !downBtn) state = Up;
                if (!upBtn && downBtn) state = Down;
            }
            if (gameOver) cursor = 2;
            break;
        case Up:
            if (!upBtn && downBtn) state = Down;
            else state = Stay;
            break;
        case Down:
            if (upBtn && !downBtn) state = Up;
            else state = Stay;
            break;
        default:
            state = C_Start;
            break;
    }
    // Actions
    switch (state) {
        case Up:
            cursor = 2;
            break;
        case Down:
            cursor = 18;
            break;
        default:
            break;
    }
    return state;
}

enum Game_States { StartGame, Run, Freeze, EndGame };
int game_tick(int state) {
    static signed char obstacle1, obstacle2;
    // Transitions
    switch (state) {
        case StartGame:
            // Initialize values
            state = Freeze;
            arena = "                                ";
            obstacle1 = 15;
            obstacle2 = 24;
            arena[obstacle1] = '*';
            arena[obstacle2] = '*';
            gameOver = 0;
            LCD_DisplayString(1, arena);
            LCD_Cursor(cursor);
            break;
        case Freeze:
        case Run:
            if (playing && !gameOver) state = Run;
            else if (gameOver) state = EndGame;
            else state = Freeze;
            break;
        case EndGame:
            if (playing) { 
                // Re-initialize values
                state = Run;
                // Clear arena
                for (int i = 0; i < 32; i++) {
                    arena[i] = ' ';
                }
                obstacle1 = 15;
                obstacle2 = 24;
                arena[obstacle1] = '*';
                arena[obstacle2] = '*';
                gameOver = 0;
            }                
            else state = EndGame;
            break;
        default:
            state = StartGame;
            break;
    }
    // Actions
    switch (state) {
        case Run:
            // Clear arena
            for (int i = 0; i < 32; i++) {
                arena[i] = ' ';
            }
            arena[32] = '\0';
            // Cycle obstacles
            if (obstacle1 < 0) {
                arena[0] = ' ';
                obstacle1 = 15;
            } else {
                arena[obstacle1 + 1] = ' ';
            }
            if (obstacle2 < 16) {
                arena[16] = ' ';
                obstacle2 = 31;
            } else {
                arena[obstacle2 + 1] = ' ';
            }
            // Position obstacles, clear trailing obstacles
            arena[obstacle1] = '*'; 
            arena[obstacle2] = '*'; 
            // Contact made
            if ((cursor == obstacle1) || (cursor == obstacle2)) {
                gameOver = 1;
                LCD_DisplayString(1, "Nobody can beat this game!");
                LCD_Cursor(33);
                break;
            }
            obstacle1--;
            obstacle2--;
            LCD_DisplayString(1, arena);
            LCD_Cursor(cursor);
            break;
        default:
            break;
    }
    return state;
}


/* ----------------------- MAIN ----------------------- */

int main(void)
{
    DDRA = 0x00; PORTA = 0x07;  // Button input
    DDRC = 0xFF; PORTC = 0x00;  // LCD data lines
    DDRD = 0xFF; PORTD = 0x00;  // LCD control lines
    
    unsigned char i = 0;

    tasks[i].state = O_Start;
    tasks[i].period = TASKS_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &onPause_tick;
    i++;
    tasks[i].state = C_Start;
    tasks[i].period = TASKS_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &controlPosition_tick;
    i++;
    tasks[i].state = StartGame;
    tasks[i].period = TASKS_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &game_tick;
    i++;

    TimerSet(TASKS_PERIOD);
    TimerOn();
    LCD_init();
    
    while (1) {}
}