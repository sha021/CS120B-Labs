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
    
    unsigned char next, display;
    next = ~PINA & 0x01;
    
    switch(Sequence_State) {
        
        case Start:
            Sequence_State = Init;
            break;
        
        case Init:
            Sequence_State = Release;
            display = 0x00;
            break;
        
        case One:
            Sequence_State = next? One : Release;
            display = 0x00;
            break;
        
        case Two:
            Sequence_State = next? Two : Release;
            display = 0x0C;
            break;
        
        case Three:
            Sequence_State = next? Three : Release;
            display = 0x1E;
            break;
        
        case Four:
            Sequence_State = next? Four : Release;
            display = 0x3F;
            break;

        case Five:
            Sequence_State = next? Five : Release;
            display = 0x33;
            break;

        case Six:
            Sequence_State = next? Six : Release;
            display = 0x21;
            break;
            
       case Release:
            if (next) {
                switch (PINB) {
                    case 0x00: // One/Init
                        Sequence_State = Two;
                        break;
                    case 0x0C: // Two
                        Sequence_State = Three;
                        break;
                    case 0x1E: // Three
                        Sequence_State = Four;
                        break;
                    case 0x3F: // Four
                        Sequence_State = Five;
                        break;
                    case 0x33: // Five
                        Sequence_State = Six;
                        break;
                    case 0x21: // Six
                        Sequence_State = One;
                        break;
                    default:
                        Sequence_State = Start;
                        break;
                }
            } else {
                Sequence_State = Release;
            }
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