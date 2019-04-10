/* Nathan Brennan & nbren004@ucr.edu
 * Seung Eun Ha & sha021@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 3 Exercise 1
 * Description: Count '1' bits on portA and port B, store on C
 * 
 * Lab_3_pt_1.c
 *
 * Created: 4/9/2019 5:58:42 PM
 * Author : Nathan
 */ 

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}    
unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    
    unsigned char a, b, i, numOnes;
     
    while (1) 
    {
        a = PINA;
        b = PINB;
        numOnes = 0;
        
        for (i = 0; i < 8; i++) {
            if (GetBit(a, i)) numOnes++;
            if (GetBit(b, i)) numOnes++;
        }
        PORTC = numOnes;
    }
}