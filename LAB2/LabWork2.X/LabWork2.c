/* 
 * File:   LabWork2.c
 * Author: Halil Ibrahim Bekli
 *
 * Target: PIC16F1518
 * IDE:    MPLAB X IDE
 * Compiler: XC8
 *
 * Description:
 * This code drives two 7-segment displays based on the provided schematic.
 * - LEFT Display (LED2) is connected to PORTC and is COMMON CATHODE (GND connected).
 * - RIGHT Display (LED1) is connected to PORTB and is COMMON ANODE (+5V connected).
 *
 * Since there are no buttons in the schematic, the code implements a 00-99 counter.
 */

#include <xc.h>

// ===== CONFIGURATION BITS =====
#pragma config FOSC = INTOSC    // Oscillator Selection (Internal Oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable (Disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (Enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (Disabled)
#pragma config IESO = OFF       // Internal/External Switchover (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Disabled)

#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (Disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Enabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Low trip point)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (Disabled)

// Define Oscillator Frequency for __delay_ms()
#define _XTAL_FREQ 4000000      // 4 MHz

// ===== SEGMENT TABLES =====

// Table for COMMON CATHODE (Left Display, PORTC)
// Logic 1 turns the segment ON.
// Format: .gfedcba
const unsigned char SEG_CC[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Table for COMMON ANODE (Right Display, PORTB)
// Logic 0 turns the segment ON.
// This is effectively the bitwise INVERSE of the Common Cathode table.
const unsigned char SEG_CA[10] = {
    0b11000000, // 0 (Inverted 0x3F)
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};

void main(void) {
    // 1. Oscillator Setup
    // SCS = 1x (Internal), IRCF = 1101 (4 MHz)
    OSCCON = 0b01101010;

    // 2. Port Configuration
    // Disable Analog functionality on PORTB (ANSELB)
    // Note: PORTC on this chip usually doesn't have ANSEL, but checked datasheet just in case.
    ANSELB = 0x00;
    
    // Set Data Direction (TRIS)
    // 0 = Output, 1 = Input
    TRISB = 0x00; // All PORTB pins are OUTPUTS (Driving LED1)
    TRISC = 0x00; // All PORTC pins are OUTPUTS (Driving LED2)

    // 3. Variables
    unsigned char count = 0;
    unsigned char leftDigit = 0;
    unsigned char rightDigit = 0;

    // 4. Main Loop
    while(1) {
        // Extract digits (e.g., if count is 42, left=4, right=2)
        leftDigit = count / 10;
        rightDigit = count % 10;

        // Drive Displays
        // PORTC drives the Left Display (Common Cathode) -> Use CC table
        PORTC = SEG_CC[leftDigit];

        // PORTB drives the Right Display (Common Anode) -> Use CA table
        PORTB = SEG_CA[rightDigit];

        // Increment counter
        count++;
        if (count > 99) {
            count = 0;
        }

        // Delay for visibility (500ms)
        __delay_ms(500);
    }
}