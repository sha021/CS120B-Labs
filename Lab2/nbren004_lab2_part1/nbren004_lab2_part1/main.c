/*
 * nbren004_lab2_part1.c
 *
 * Created: 4/4/2019 5:45:25 PM
 * Author : Nathan
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of PB
	unsigned char tmpA0 = 0x00; // Temporary variable to hold the value of PA0
	unsigned char tmpA1 = 0x00; // Temporary variable to hold the value of PA1

	while(1)
	{
		tmpA0 = PINA & 0x01;		// A[0]
		tmpA1 = PINA & 0x02;		// A[1]
		
		tmpB = (!tmpA1 && tmpA0) ? 1 : 0;
/*
		if (!tmpA0 && tmpA1)
			tmpB = 1;
		else
			tmpB = 0;
*/		PORTB = tmpB;
	}
	return 0;
}

/* Sample Code from Pre-Lab
 
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
		*/

