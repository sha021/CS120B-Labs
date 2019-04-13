/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 4 Part 2
 * Description: Adding a lock from the outside feature to the previous "Combinational Lock" program
 *
 * Created: 4/12/2019 9:55:00 PM
 * Author : SIENA
 */ 


#include <avr/io.h>

enum Counter_State { Start, Init, Reset, Increment_Press, Decrement_Press, Release } Counter_State;

void counter() {
    
    unsigned char increment = PINA & 0x01, decrement = PINA & 0X02, display = PINC & 0x0F;
    
    switch(Counter_State) {
        case Start:
            Counter_State = Init;
        break;
        
        case Init:
            display = 0x07;
            if (increment && !decrement) Counter_State = Increment_Press;
            else if (!increment && decrement) Counter_State = Decrement_Press;
            else Counter_State = Reset;
        break;
        
        case Reset:
            display = 0x00;
            if (increment && !decrement) Counter_State = Increment_Press;
            else if (!increment && decrement) Counter_State = Decrement_Press;
            else Counter_State = Reset;
        break;
        
        case Increment_Press:
            if (display < 9) display++;
            if (!increment) Counter_State = Release;
            else Counter_State = Increment_Press;
        break;
        
        case Decrement_Press:
            if (display > 0) display--;
            if (!decrement) Counter_State = Release;
            else Counter_State = Decrement_Press;
        break;
        
        case Release:
            if (increment && !decrement) Counter_State = Increment_Press;
            else if (!increment && decrement) Counter_State = Decrement_Press;
            else Counter_State = Reset;
        break;
        
        default:
            Counter_State = Start;
        break;
    }
    PORTC = display;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0x03;
    DDRC = 0x0F; PORTC = 0x00;
    
    while (1) 
    {
        counter();
    }
}