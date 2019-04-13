/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 4 Part 4
 * Description: Changing code from the previous "Combinational Lock" program to more complex lock code
 * 
 *
 * Created: 4/12/2019 11:10:05 PM
 * Author : SIENA
 */ 


#include <avr/io.h>

enum Lock_State { Start, Init, Locked, Unlocked, Code } Lock_State;
    
void combinationalLock() {
    
    unsigned char x, y, pound, lock, deadBolt;
    x = PINA & 0x01;
    y = PINA & 0x02;
    pound = PINA & 0x04;
    lock = PINA & 0x80;
    deadBolt = PINB;
    
    switch(Lock_State) {
        case Start:
            Lock_State = Init;
            break;
        case Init:
            deadBolt = 0x00;
            Lock_State = Locked;
        case Locked:
            deadBolt = 0x00;
            if (pound && !x && !y && !lock) Lock_State = PressP_1;
            else Lock_State = Locked;
            break;
        case PressP_1:
            deadBolt = deadBolt? 0x01 : 0x00;
            state = 1;
            // Stay in this state
            if (pound && !x && !y && !lock) Lock_State = PressP_1;
            // Transition to new state
            else {
                // Correct sequence
                if (!pound && !x && !y && !lock) Lock_State = ReleaseP_2;
                // Code Wrong while attempting to Lock
                else if (deadBolt) {
                    Lock_State = Unlocked;
                }
                // Code Wrong while attempting to Unlock
                else {
                    Lock_State = Locked;
                } 
            }            
            break;
        case ReleaseP_2:
            deadBolt = deadBolt? 0x01 : 0x00;
            state = 2;
            // Stay in this state
            if (!pound && !x && !y && !lock) Lock_State = ReleaseP_2;
            // Transition to new state
            else {
                // Attempting to Lock
                if (deadBolt) {
                    if (!pound && !x && y && !lock) Lock_State = Locked;
                    else Lock_State = Unlocked;
                }
                // Attempting to Unlock
                else {
                    if (!pound && !x && y && !lock) Lock_State = Unlocked;
                    else Lock_State = Locked;
                }                
            }                
            break;
        case Unlocked:
            deadBolt = 0x01;            
            if (!pound && !x && !y && lock) Lock_State = Locked;
            else if (pound && !x && !y && !lock) Lock_State = PressP_1;
            else Lock_State = Unlocked;
            break;
        default:
            Lock_State = Init;
            break;
    }
    PORTB = deadBolt;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0x87;
    DDRB = 0x01; PORTB = 0x00;
    
    while (1) 
    {
        combinationalLock();
    }
}

