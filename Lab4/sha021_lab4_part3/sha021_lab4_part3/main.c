/*
 * sha021_lab4_part3.c
 *
 * Created: 4/12/2019 8:51:50 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

enum Lock_State { Locked_0, PressP_1, ReleaseP_2, UnlockedY_3, Start, Init } Lock_State;
    
void combinationalLock() {
    
    unsigned char x, y, pound, lock, deadBolt, state;
    x = PINA & 0x01;
    y = PINA & 0x02;
    pound = PINA & 0x04;
    lock = PINA & 0x80;
    state = PINC;
    
    switch(Lock_State) {
        case Start:
            Lock_State = Init;
            break;
        case Init:
            deadBolt = 0x00;
            Lock_State = Locked_0;
        case Locked_0:
            deadBolt = 0x00;
            state = 0;
            if (pound && !x && !y && !lock) Lock_State = PressP_1;
            else Lock_State = Locked_0;
            break;
        case PressP_1:
            deadBolt = 0x00;
            state = 1;
            if (!pound && !x && !y && !lock) Lock_State = ReleaseP_2;
            else if (pound && !x && !y && !lock) Lock_State = PressP_1;
            else Lock_State = Locked_0;
            break;
        case ReleaseP_2:
            deadBolt = 0x00;
            state = 2;
            if (!pound && !x && y && !lock) Lock_State = UnlockedY_3;
            else if (!pound && !x && !y && !lock) Lock_State = ReleaseP_2;
            else Lock_State = Locked_0;
            break;
        case UnlockedY_3:
            deadBolt = 0x01;            
            state = 3;
            if (!pound && !x && !y && lock) Lock_State = Locked_0;
            else Lock_State = UnlockedY_3;
            break;
        default:
            Lock_State = Init;
            break;
    }
    PORTB = deadBolt;
    PORTC = state;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0x87;
    DDRB = 0x01; PORTB = 0x00;
    DDRC = 0x03; PORTC = 0x00;
    
    while (1) 
    {
        combinationalLock();
    }
}

