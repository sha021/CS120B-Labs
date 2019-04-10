/*
 * sha021_lab3_part2.c
 *
 * Created: 4/9/2019 6:06:10 PM
 * Author : SIENA
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    DDRA = 0x00;    PORTA = 0xFF;
    DDRC = 0xFF;    PORTC = 0x00;
    
    unsigned char tmpA, tmpC, keyInIgnition, driverSeated, seatBeltFastened;
  
    /* Replace with your application code */
    while (1) 
    {   
        tmpA = PINA & 0x0F;
        
        keyInIgnition = GetBit(PINA, 4);
        driverSeated = GetBit(PINA, 5);
        seatBeltFastened = GetBit(PINA, 6);
        
        tmpC = 0;
        if (tmpA < 1) {
            tmpC = SetBit(tmpC, 6, 1);
        }
        else if (tmpA < 3){
            tmpC = SetBit(tmpC, 5, 1) 
                    + SetBit(tmpC, 6, 1);
        }
        else if (tmpA <5){
            tmpC = SetBit(tmpC, 5, 1) 
                    + SetBit(tmpC, 4, 1) 
                    + SetBit(tmpC, 6, 1);
        }
        else if (tmpA < 7){
            tmpC = SetBit(tmpC, 5, 1) 
                    + SetBit(tmpC, 4, 1) 
                    + SetBit(tmpC, 3, 1);            
        }
        else if (tmpA < 10){
            tmpC = SetBit(tmpC, 5, 1) 
                    + SetBit(tmpC, 4, 1) 
                    + SetBit(tmpC, 3, 1) 
                    + SetBit(tmpC, 2, 1);
        }  
        else if (tmpA < 13){
            tmpC = SetBit(tmpC, 5, 1) 
                    + SetBit(tmpC, 4, 1) 
                    + SetBit(tmpC, 3, 1) 
                    + SetBit(tmpC, 2, 1) 
                    + SetBit(tmpC, 1, 1);
        }      
        else if (tmpA < 16){
            tmpC = SetBit(tmpC, 5, 1) 
                    + SetBit(tmpC, 4, 1) 
                    + SetBit(tmpC, 3, 1) 
                    + SetBit(tmpC, 2, 1) 
                    + SetBit(tmpC, 1, 1) 
                    + SetBit(tmpC, 0, 1);            
        }
         
        if (keyInIgnition && driverSeated && !seatBeltFastened){
            tmpC = SetBit(tmpC, 7, 1);
        }    
        PORTC = tmpC;
    }
}

