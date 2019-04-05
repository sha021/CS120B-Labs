/*
 * sha021_lab2_part2.c
 *
 * Created: 4/4/2019 10:31:43 PM
 * Author : SIENA
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00;    PORTA = 0xFF;
	DDRC = 0XFF;	PORTC = 0x00;

	unsigned char tmpA3 = 0x00;
	unsigned char tmpA2 = 0x00;
	unsigned char tmpA1 = 0x00;
	unsigned char tmpA0 = 0x00;

	unsigned char tmpC = 0x00;

    /* Replace with your application code */
    while (1) 
    {
        tmpA0 = PINA & 0x01;
        tmpA1 = PINA & 0x02;
        tmpA2 = PINA & 0x04;
        tmpA3 = PINA & 0x08;
	    tmpC = 0x04;        		
                        
        if (tmpA0 == 0x01) { tmpC--; }
        if (tmpA1 == 0x02) { tmpC--; }
        if (tmpA2 == 0x04) { tmpC--; }
        if (tmpA3 == 0x08) { tmpC--; }
            
        PORTC = tmpC;
    }
	return 0;
}

