/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 5 Part 3
 * Description: Festive/Decorative lighting display sequence
 *
 * Created: 4/19/2019 10:40:20 PM
 * Author : Nathan
 */ 

#include <avr/io.h>

enum Sequence_States { Start, Init, One, Two, Three, Four, Five, Six, Release} Sequence_State;

void lightSequence() {
    
    unsigned char next, display, nextState;
    next = ~PINA & 0x01;
    
    switch(Sequence_State) {
        
        case Start:
            Sequence_State = Init;
            nextState = Init;
            break;
        
        case Init:
            Sequence_State = One;
            display = 0;
            break;
        
        case One:
            Sequence_State = next? Release : One;
            display = 0x00;
            nextState = Two;
            break;
        
        case Two:
            Sequence_State = next? Release : Two; 
            display = 0x0C;
            nextState = Three;
            break;
        
        case Three:
            Sequence_State = next? Release : Three;
            display = 0x1E;
            nextState = Four;
            break;
        
        case Four:
            Sequence_State = next? Release : Four;
            display = 0x3F;
            nextState = Five;
            break;

        case Five:
            Sequence_State = next? Release : Five;
            display = 0x33;
            nextState = Six;
           break;

        case Six:
            Sequence_State = next? Release : Six;
            display = 0x21;
            nextState = One;
            break;
            
       case Release:
            Sequence_State = next? Release : nextState;
            display = PINB;
            break;
        
        default:
            Sequence_State = Start;
            break;
    }
    PORTB = display;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    Sequence_State = Start;
        
    while (1) 
    {
        lightSequence();
    }
}

