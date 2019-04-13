/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 4 Part 4
 * Description: Changing code from the previous "Combinational Lock" program to more complex lock code;
 *              also removing states and changing to Looping State Machine
 *
 * Created: 4/12/2019 11:10:05 PM
 * Author : SIENA
 */ 


#include <avr/io.h>

enum Lock_State { Start, Init, Locked, Unlocked, Code } Lock_State;
    
void combinationalLock() {

    const unsigned char THE_CODE[3] = { /*#,*/ 0x01, 0x02, 0x01 }; 
    unsigned char numPad, deadBolt, correctCode, i;
    numPad = PINA;
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
            if (numPad == 0x04) Lock_State = Code;
            else Lock_State = Locked;
            break;
        case Unlocked:
            deadBolt = 0x01;            
            if (numPad == 0x80) Lock_State = Locked;
            else if (numPad == 0x04) Lock_State = Code;
            else Lock_State = Unlocked;
            break;
        case Code:
            deadBolt = deadBolt? 0x01 : 0x00;
            correctCode = 1;
            // While # key is pressed
            while (numPad == 0x04) numPad = PINA;
            // After releasing # key
            for (i = 0; i < 3; i++) {
                numPad = PINA;
                // While previous button is being held
                if (i > 0) {
                    while (numPad == THE_CODE[i - 1]) numPad = PINA;
                }
                // While no button pressed
                while (!numPad) numPad = PINA;
                // Wrong lock code combo
                if (numPad != THE_CODE[i]) {
                    correctCode = 0;
                    break;
                }
            }
            if (correctCode) Lock_State = deadBolt ? Locked : Unlocked;
            else Lock_State = deadBolt ? Unlocked : Locked;
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