/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 5 Part 2
 * Description: Improved wacky counter with counter-intuitive specs (Hardware version)
 *
 * Created: 4/19/2019 9:56:11 PM 
 * Author : Nathan
 */ 

#include <avr/io.h>

enum Counter_State { Start, Init, Wait, Increment_Press, Decrement_Press, Held } Counter_State;

void counter() {
    
    unsigned char increment = ~PINA & 0x01, decrement = ~PINA & 0X02, display = PINB & 0x0F;
    
    switch(Counter_State) {
        
        case Start:
            Counter_State = Init;
        break;
        
        case Init:
            display = 0x07;
            Counter_State = Wait;
            break;
        
        case Wait:
            if (increment && !decrement) Counter_State = Increment_Press;
            else if (!increment && decrement) Counter_State = Decrement_Press;
            else if (increment && decrement) Counter_State = Held;
            else Counter_State = Wait;
            break;
        
        case Increment_Press:
            if (display < 9) display++;
            Counter_State = Held;
            break;
        
        case Decrement_Press:
            if (display > 0) display--;
            Counter_State = Held;
            break;
        
        case Held:
            if (!(increment || decrement)) Counter_State = Wait;
            else {
                // If both pressed, reset to counter to 0 
                if (increment && decrement) display = 0x00;
                Counter_State = Held;
            }                
            break;
        
        default:
            Counter_State = Start;
            break;
    }
    PORTB = display;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0x03;
    DDRB = 0x0F; PORTB = 0x00;
    
    while (1) 
    {
        counter();
    }
}