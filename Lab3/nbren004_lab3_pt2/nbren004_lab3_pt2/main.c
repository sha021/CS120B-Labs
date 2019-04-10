/* Nathan Brennan & nbren004@ucr.edu
 * Seung Eun Ha & sha021@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 3 Exercise 1
 * Description: Simulate gas meter for car
 * nbren004_lab3_pt2.c
 *
 * Created: 4/9/2019 6:31:25 PM
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
    DDRC = 0xFF; PORTC = 0x00;
    
    unsigned char rawFuel, graphFuel;
     
    while (1) 
    {
        rawFuel = PINA & 0x0F;
        graphFuel = 0x00;
        
        switch(rawFuel) {
            
            case 0x00:
                graphFuel = SetBit(graphFuel, 6, 1);
            break;
            
            case 0x01:
            case 0x02:
                graphFuel = SetBit(graphFuel, 5, 1);
            break;
            
            case 0x03:
            case 0x04:
                graphFuel = SetBit(graphFuel, 4, 1) + SetBit(graphFuel, 5, 1);
            break;
            
            case 0x05:
            case 0x06:
                graphFuel = SetBit(graphFuel, 3, 1)
                          + SetBit(graphFuel, 4, 1)
                          + SetBit(graphFuel, 5, 1);
            break;
            
            case 0x07:
            case 0x08:
            case 0x09:
                graphFuel = SetBit(graphFuel, 2, 1)
                          + SetBit(graphFuel, 3, 1)
                          + SetBit(graphFuel, 4, 1)
                          + SetBit(graphFuel, 5, 1);
            break;
            
            case 0x0A:
            case 0x0B:
            case 0x0C:
                graphFuel = SetBit(graphFuel, 1, 1)
                          + SetBit(graphFuel, 2, 1)
                          + SetBit(graphFuel, 3, 1)
                          + SetBit(graphFuel, 4, 1)
                          + SetBit(graphFuel, 5, 1);
            break;
            
            case 0x0D:
            case 0x0E:
            case 0x0F:
                graphFuel = SetBit(graphFuel, 0, 1)
                          + SetBit(graphFuel, 1, 1)
                          + SetBit(graphFuel, 2, 1)
                          + SetBit(graphFuel, 3, 1)
                          + SetBit(graphFuel, 4, 1)
                          + SetBit(graphFuel, 5, 1);
            break;
            
            default:
                graphFuel = SetBit(graphFuel, 6, 1) + SetBit(graphFuel, 7, 1);
        }
        PORTC = graphFuel;
    }
}

