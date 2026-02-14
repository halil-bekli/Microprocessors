/* 
 * File:   LabWork0.c
 * Author: Halil Ibrahim Bekli
 *
 * Created on November 5, 2025, 8:40 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// CONFIG
#pragma config FOSC = INTOSC // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

#pragma config IESO = OFF // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
// CONFIG2
#pragma config WRT = OFF // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF // Voltage Regulator Capacitor Enable bit (VCAP pin function disabled)
#pragma config STVREN = ON // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

int main() {
    ANSELB = 0; // All PORTB pins are digital
    TRISB = 0b11000000; // PORTB.6 and PORTB.7 are inputs, other pins are outputs
    TRISA = 0; // All PORTA pins are outputs
    //PORTA = 0xFF; // Set all PORTA pins to high level (+5V)
    
    while (1) { // While 1 = true (and 1 is always true)...
        if (RB0 == 1) { // If the input level on PORTB.0 is high, then...
        PORTB = 0; // All PORTB pins are connected to GND (logic low)
    } else { // Otherwise
        PORTB = 255; // Set all PORTA pins to high level (+5V)
    }
        
    }
}
