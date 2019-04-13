/*
 * sha021_lab4_part1.c
 *
 * Created: 4/12/2019 6:04:24 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

enum LED_State {Start, Init, Off_Release, On_Press, On_Release, Off_Press} LED_State;
    
void Led_Alternate() {      // Transitions
    
    unsigned char light, button = PINA & 0x01;
    
    switch(LED_State) {
        case Start:
            LED_State = Init;
        case Init: 
            LED_State = Off_Release;
            break;
        case Off_Release:
            if (button) LED_State = On_Press;
            else LED_State = Off_Release;
            break;
        case On_Press:
            if (!button) LED_State = On_Release;
            else LED_State = On_Press;
            break;
        case On_Release:
            if (button) LED_State = Off_Press;
            else LED_State = On_Release;
            break;
        case Off_Press:
            if (!button) LED_State = Off_Release;
            else LED_State = Off_Press;
            break;
        default:
            LED_State = Start;
            break;
    }
    
    switch(LED_State) {     // Actions
        case Init:
        case Off_Release:
        case Off_Press:
            light = 0x01;
            break;
        case On_Press:
        case On_Release:
            light = 0x02;   
            break;
        default:
            break;
    }
    PORTB = light;
}        
    

int main(void)
{
    DDRA = 0x00;    PORTA = 0xFF;
    DDRB = 0x03;    PORTB = 0x00;

    while (1) 
    {
        Led_Alternate();
    }
}

