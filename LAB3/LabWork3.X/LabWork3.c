/* 
 * File:   LabWork3.c
 * Author: Halil Ibrahim Bekli
 *
 * Target: PIC16F1518
 * Description: ADC Reading from Thermistor (RA1) and Button Input (RB4)
 * Based on the provided schematic and Lab 0 configuration bits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// ==========================================
// CONFIGURATION BITS
// ==========================================

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable (Disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (Enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR enabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (Disabled)
#pragma config IESO = OFF       // Internal/External Switchover (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Disabled)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (Disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Enabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Low trip point)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (Disabled)

#define _XTAL_FREQ 4000000      // Define frequency as 4MHz for delay macros

// ==========================================
// MAIN PROGRAM
// ==========================================
int main() {
    
    // --------------------------------------
    // 1. Oscillator Setup
    // --------------------------------------
    // Set Internal Oscillator to 4MHz
    OSCCONbits.IRCF = 0b1101;   
    OSCCONbits.SCS = 0b10;      

    // --------------------------------------
    // 2. Port Configuration (Tristate & Analog)
    // --------------------------------------
    
    // --- PORT A Setup (Thermistor & Outputs) ---
    // RA1 is Input (Thermistor), others are Outputs (LEDs)
    TRISA = 0b00000010;         
    
    // RA1 must be Analog for ADC, others Digital
    ANSELA = 0b00000010;        
    
    // --- PORT B Setup (Button) ---
    // RB4 is Input (Button on schematic), others Outputs
    TRISBbits.TRISB4 = 1;       
    
    // IMPORTANT: Make sure RB4 is DIGITAL. 
    // By default, RB4 might be analog (AN11). We must clear ANSELB bit 4.
    ANSELBbits.ANSB4 = 0;       

    // --------------------------------------
    // 3. ADC Configuration (Analog to Digital Converter)
    // --------------------------------------
    
    // ADCON1: Result Right Justified, Fosc/16, Vref = VDD
    ADCON1bits.ADFM = 1;        // Right justified
    ADCON1bits.ADCS = 0b101;    // Fosc/16
    ADCON1bits.ADPREF = 0b00;   // Vref+ is VDD
    
    // ADCON0: Select Channel AN1 (RA1) and Enable ADC
    ADCON0bits.CHS = 0b00001;   // Select Channel AN1 (RA1)
    ADCON0bits.ADON = 1;        // Turn ADC On

    // Variables
    unsigned int adc_result;    // To store 10-bit ADC value (0-1023)
    
    // --------------------------------------
    // 4. Main Loop
    // --------------------------------------
    while(1) {
        
        // --- Step A: Read Thermistor (ADC) ---
        __delay_us(20);             // Acquisition delay
        ADCON0bits.GO = 1;          // Start conversion
        while(ADCON0bits.GO);       // Wait for conversion to finish
        
        // Read the result (Combine High and Low bytes)
        adc_result = ((unsigned int)ADRESH << 8) + ADRESL;

        // --- Step B: Logic Control ---
        
        // Example Logic: If Thermistor value is high (or low depending on NTC/PTC),
        // Turn on PORTA LEDs.
        // Assuming NTC: Heat -> Resistance Down -> Voltage Down -> ADC Low.
        // Let's say if ADC < 512 (approx 2.5V), turn on LEDs.
        
        if (adc_result < 512) {     
            PORTA = 0xFF; // Turn ON all LEDs on PORTA (Visual indicator)
        } else {
            PORTA = 0x00; // Turn OFF all LEDs
        }

        // --- Step C: Read Button (RB4) ---
        // Schematic shows Pull-up resistor on RB4.
        // Default (Not Pressed) = 1 (+5V)
        // Pressed = 0 (GND)
        
        if (RB4 == 0) {
            // If button is pressed, force LEDs ON regardless of temperature
            PORTA = 0xFF; 
        }
    }
    
    return 0;
}