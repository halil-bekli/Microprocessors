/* 
 * File:   LabWork1.c
 * Author: Halil Ibrahim Bekli
 *
 * Created on November 5, 2025, 9:41 AM
 */

/* PIC16F1518 - LAB1 variant template
   Task: Variant 1 -> LED: RB4 (anode), Button: RB0 (to GND), internal pull-up
   XC8 compiler (MPLAB X)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config BOREN = OFF
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
// CONFIG2
#pragma config WRT = OFF
#pragma config VCAPEN = OFF
#pragma config STVREN = ON
#pragma config BORV = LO
#pragma config LPBOR = OFF
#pragma config LVP = OFF

// ----- Variant-specific mapping (Variant 1) -----
#define LED_LAT LATBbits.LATB4
#define LED_TRIS TRISBbits.TRISB4

#define BTN_PORT PORTBbits.RB0
#define BTN_TRIS TRISBbits.TRISB0
#define BTN_WPU WPUBbits.WPUB0

// Other definements
#define DELAY_TIME 20
#define _XTAL_FREQ 16000000UL   // osilatör frekans?n 16 MHz ise

// -------------------- Functions --------------------
// Introduce methods
void mainLoop();
void init_io();
void delay_ms_simple(uint16_t ms);

int main(void) {
    // Optionally set internal oscillator to 16 MHz (device specific)
    // OSCCON = 0b01011010; // example value; ensure matches _XTAL_FREQ if you change

    init_io();

    // Start with LED ON
    LED_LAT = 1;

    mainLoop();

    return 0;
}

void mainLoop() {
    while (1) {
        // Button is pulled-up -> unpressed: logic 1. pressed -> 0 (to GND)
        if (BTN_PORT == 0) {
            // Debounce: small delay then check again
            delay_ms_simple(DELAY_TIME);
            if (BTN_PORT == 0) {
                // Button confirmed pressed
                LED_LAT = 0; // turn LED OFF while button pressed

                // Wait until button released (and simple debounce on release)
                while (BTN_PORT == 0) {
                    // optionally implement a tiny sleep
                    
                }
                // released, wait debounce
                delay_ms_simple(DELAY_TIME);

                // After release, turn LED back ON
                LED_LAT = 1;
            }
        }
        // small idle delay to reduce CPU churn
        __delay_ms(5);
    }
}

void init_io() {
    // Make all ports digital
    ANSELB = 0; // PORTB is digital
    ANSELC = 0;
    ANSELA = 0;

    // TRIS: 0 -> output, 1 -> input
    LED_TRIS = 0; // RB4 as output
    BTN_TRIS = 1; // RB0 as input

    // Clear outputs
    LATB = 0x00;

    // Enable weak pull-ups globally and per-pin
    nWPUEN = 0; // enable weak pull-ups (0 = enabled)
    BTN_WPU = 1; // enable pull-up on RB0

    // OPTION_REG bits might control WPU on some PICs; using nWPUEN and WPUBx as example
    // If needed, set IOC or other registers here.

    // Other useful settings
    // Disable comparators etc if present (device-specific). For PIC16F1518 comparators are not on PORTB.
}

void delay_ms_simple(uint16_t ms) {
    while (ms--) __delay_ms(1);
}
