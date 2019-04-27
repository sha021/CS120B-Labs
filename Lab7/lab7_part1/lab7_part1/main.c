/* Nathan Brennan - nbren004@ucr.edu
 * Seung Eun Ha - sha021@ucr.edu
 * Lab Section: 27
 * Assignment: Lab 7 Part 1
 * Description: Improved wacky counter with counter-intuitive specs (Hardware version)
 *
 * Created: 4/25/2019 9:56:11 PM 
 * Author : Nathan & Seung Eun
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"


unsigned char cnt;
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
			// bit2bit1bit0=011: pre-scaler /64
			// 00001011: 0x0B
			// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
			// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
			// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
			// So when TCNT1 register equals 125,
			// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

    unsigned char increment, decrement ;


enum Counter_State { Start, Init, Wait, Increment_Press, Decrement_Press, Held } Counter_State;

void counter() {
   // increment = ~PINA & 0x01, decrement = ~PINA & 0X02;
    
    switch(Counter_State) {
        
        case Start:
            Counter_State = Init;
        break;
        
        case Init:
            Counter_State = Wait;
            cnt = 0;
            break;
        
        case Wait:
            if (increment && !decrement) Counter_State = Increment_Press;
            else if (!increment && decrement) Counter_State = Decrement_Press;
            else if (increment && decrement) Counter_State = Held;
            else Counter_State = Wait;
            break;
        
        case Increment_Press:
            if (cnt < 9) cnt++;
            Counter_State = Held;
            break;
        
        case Decrement_Press:
            if (cnt > 0) cnt--;
            Counter_State = Held;
            break;
        
        case Held:
        
         if (increment && !decrement) Counter_State = Increment_Press;
         else if (!increment && decrement) Counter_State = Decrement_Press;
         else if (increment && decrement)
            {   cnt = 0;
                Counter_State = Held;
            }   
         else Counter_State = Wait;             
            break;
        
        default:
            Counter_State = Start;
            break;
    }
        LCD_WriteData(cnt + '0');

}

int main(void)
{
    DDRA = 0x00; PORTA = 0x03;
    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines

    unsigned char flag1 = 0, flag2 = 0;
    
    
    TimerSet(1000);
	TimerOn();

    LCD_init();   				 
    

    while (1) 
    {
        increment = flag1;
        decrement = flag2;
        
        //LCDCount_Tick();
        LCD_Cursor(1);
        
        flag1 = 0;
        flag2 = 0;
        
        while (!TimerFlag){
            if(~PINA & 0x01) flag1 = 1;
            if(~PINA & 0x02) flag2 = 1;
            }	// Wait 1 sec
		TimerFlag = 0;
        counter();
    }
}