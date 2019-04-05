/*
 * sha021_lab2_part1.c
 *
 * Created: 4/4/2019 5:32:31 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpPA0 = 0x00; // Temporary variable to hold the value of A0 Door
	unsigned char tmpPA1 = 0x00; // Temporary variable to hold the value of A1 Light
	unsigned char tmpPB0 = 0x00;
	while(1)
	{
		tmpPA0 = PINA & 0x01; // Door 
		tmpPA1 = PINA & 0x10; // Light
		
		//Light should be on when Door is on while Light is off:  0x01
		tmpPB0 = (tmpPA1 == 0 && tmpPA0 == 1) ? 1 : 0; 

		PORTB = tmpPB0;
	}
	return 0;	
}



/*
		// 1) Read input
		tmpA = PINA & 0x01;
		// 2) Perform computation
		// if PA0 is 1, set PB1PB0 = 01, else = 10
		if (tmpA == 0x01) { // True if PA0 is 1
			tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbb01
			// (clear rightmost 2 bits, then set to 01)
		}
		else {
			tmpB = (tmpB & 0xFC) | 0x02; // Sets tmpB to bbbbbb10
			// (clear rightmost 2 bits, then set to 10)
		}
		// 3) Write output
		PORTB = tmpB;
	}
	return 0;
	
*/