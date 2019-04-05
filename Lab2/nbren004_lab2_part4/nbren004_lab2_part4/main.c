/*
 * nbren004_lab2_part4.c
 *
 * Created: 4/5/2019 3:10:11 AM
 * Author : Nathan
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00;
    
    unsigned char seatA = 0x00;
    unsigned char seatB = 0x00;
    unsigned char seatC = 0x00;
    unsigned char outputD = 0x00;
    const unsigned char WEIGHT_LIMIT = 140;     // 0x8C
    const unsigned char BALANCE_LIMIT = 80;     // 0x50
    
    while (1) 
    {
        unsigned char weightExceeded, unbalanced, approxAvgWeight;
        unsigned short totalWeight = seatA + seatB + seatC;
       
        seatA = PINA;
        seatB = PINB;
        seatC = PINC;
        
        weightExceeded = totalWeight > WEIGHT_LIMIT ? 0x01 : 0x00;
        if (seatA > seatC) {
            unbalanced = seatA - seatC > BALANCE_LIMIT ? 0x02 : 0x00;
        } else {
            unbalanced = seatC - seatA > BALANCE_LIMIT ? 0x02 : 0x00;
        }            
        approxAvgWeight = totalWeight / 3;
        
        outputD = weightExceeded;
        outputD += unbalanced;
        outputD += approxAvgWeight & 0xFC;
        
        PORTD = outputD;
    }
}