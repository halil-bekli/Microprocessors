/* 
 * File:   LabWork5.c
 * Author: Halil Ibrahim Bekli
 *
 * Target: PIC16F1518
 * Description: Lab 0 Project - Adapted for the schematic with CORRECTED PINOUT.
 *
 * FINAL PINOUT (Verified from Schematic):
 * LCD E   -> RB7 (Pin 28) -> JP1-1
 * LCD RS  -> RB6 (Pin 27) -> JP1-2
 * LCD D7  -> RB5 (Pin 26)
 * LCD D6  -> RB4 (Pin 25)
 * LCD D5  -> RB3 (Pin 24)
 * LCD D4  -> RB2 (Pin 23)
 *
 * Note: Data nibble needs to be shifted left by 2 to align with RB2-RB5.
 */

// --- CONFIGURATION BITS ---
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

#include <xc.h>

#define _XTAL_FREQ 16000000

// --- PIN DEFINITIONS ---
// Control Pins are on RB6 and RB7
#define LCD_RS_PIN  LATBbits.LATB6
#define LCD_E_PIN   LATBbits.LATB7

// --- LCD HELPER FUNCTIONS ---

void LCD_Pulse_Enable(void) {
    LCD_E_PIN = 1;
    __delay_us(5);
    LCD_E_PIN = 0;
    __delay_us(50);
}

void LCD_Send_Nibble(unsigned char nibble) {
    // Input 'nibble' is in lower 4 bits (0000 DCBA)
    // We need to send it to RB5, RB4, RB3, RB2.
    // So we shift left by 2: (00DC BA00)
    
    unsigned char port_val = LATB;
    
    // Clear data bits RB2-RB5 (Mask: 1100 0011 -> 0xC3)
    // We keep RB6(RS), RB7(E) and RB0, RB1 untouched.
    port_val &= 0xC3; 
    
    // Position the data
    port_val |= ((nibble & 0x0F) << 2);
    
    // Write to Port
    LATB = port_val;
    
    LCD_Pulse_Enable();
}

void LCD_Send_Byte(unsigned char byte, unsigned char is_data) {
    LCD_RS_PIN = is_data; // 0 = Command, 1 = Data
    
    // Send High Nibble
    LCD_Send_Nibble(byte >> 4);
    
    // Send Low Nibble
    LCD_Send_Nibble(byte);
}

void LCD_Command(unsigned char cmd) {
    LCD_Send_Byte(cmd, 0);
    __delay_ms(2);
}

void LCD_Char(char c) {
    LCD_Send_Byte(c, 1);
}

void LCD_String(const char* str) {
    while(*str) {
        LCD_Char(*str++);
    }
}

void LCD_Init(void) {
    LCD_RS_PIN = 0;
    LCD_E_PIN = 0;
    LATB = 0; // Clear all
    
    __delay_ms(50);
    
    // Reset Sequence
    LCD_Send_Nibble(0x03); __delay_ms(5);
    LCD_Send_Nibble(0x03); __delay_us(150);
    LCD_Send_Nibble(0x03);
    
    LCD_Send_Nibble(0x02); // 4-bit mode
    
    // Config
    LCD_Command(0x28); // 4-bit, 2-line, 5x8
    LCD_Command(0x0C); // Display ON
    LCD_Command(0x06); // Increment Cursor
    LCD_Command(0x01); // Clear Display
}

// --- MAIN ---
int main(void) {
    // Oscillator 16MHz
    OSCCONbits.IRCF = 0b1111; 
    OSCCONbits.SCS = 0b00;
    
    // Ports
    ANSELB = 0; 
    ANSELC = 0; 
    
    // Directions
    // RB2-RB7 are Outputs for LCD. 
    // RB0-RB1 are unused by LCD logic but let's set them output too to be safe/clean.
    TRISB = 0b00000000; // All Output
    TRISC = 0xFF;       // Keypad Inputs
    
    LATB = 0;
    
    LCD_Init();
    
    LCD_Command(0x80); // Line 1
    LCD_String("LAB 0 - START");
    
    LCD_Command(0xC0); // Line 2
    LCD_String("PINOUT: OK!");
    
    while(1) {
        // Infinite Loop
    }
    return 0;
}