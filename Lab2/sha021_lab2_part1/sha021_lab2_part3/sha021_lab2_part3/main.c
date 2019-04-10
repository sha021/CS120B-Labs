
/* Partner(s) Name & E-mail: Nathan Brennan nbren004@ucr.edu
* Lab Section: 027
* Assignment: Lab 2 Exercise 3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00;    PORTA = 0xFF;
    DDRC = 0xFF;    PORTC = 0x00;
    
    unsigned char tmpA0 = 0x00;
    unsigned char tmpA1 = 0x00;
    unsigned char tmpA2 = 0x00;
    unsigned char tmpA3 = 0x00;
    
    unsigned char tmpC = 0x00;

        
    /* Replace with your application code */
    while (1) 
    {
        tmpA0 = PINA & 0x01;
        tmpA1 = PINA & 0x02;
        tmpA2 = PINA & 0x04;
        tmpA3 = PINA & 0x08;
        
        tmpC = tmpC & 0x00;

        if (tmpA0 && tmpA1 && tmpA2 && tmpA3) { tmpC = 0x80; }
        else{   
            if (!tmpA0) {tmpC += 0x01;}
            if (!tmpA1) {tmpC += 0x02;}
            if (!tmpA2) {tmpC += 0x04;}
            if (!tmpA3) {tmpC += 0x08;}
        }        
        PORTC = tmpC;
        
    }
}

