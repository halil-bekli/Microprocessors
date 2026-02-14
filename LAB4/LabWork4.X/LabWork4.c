/* 
 * File:   LabWork4.c
 * Author: Halil Ibrahim Bekli
 *
 * Device: PIC16F1518
 * Description: Simple I2C EEPROM Write/Read (Simplified)
 */

#include <xc.h>

#define _XTAL_FREQ 16000000

// Configuration Bits (Simplified)
#pragma config FOSC=INTOSC, WDTE=OFF, PWRTE=ON, MCLRE=ON, CP=OFF, BOREN=OFF
#pragma config CLKOUTEN=OFF, WRT=OFF, STVREN=ON, BORV=LO, LPBOR=OFF, LVP=OFF

// --- Simple I2C Functions ---

void I2C_Wait() {
    while ((SSP1CON2 & 0x1F) || (SSP1STATbits.R_nW)); // Wait for idle
}

void I2C_Init() {
    OSCCONbits.IRCF = 0b1111; // Set clock to 16MHz
    TRISC3 = 1; TRISC4 = 1;   // Set SCL and SDA as inputs
    SSP1CON1 = 0b00101000;    // Enable I2C Master mode
    SSP1ADD = 39;             // Set speed to 100kHz
}

void I2C_Start() { I2C_Wait(); SSP1CON2bits.SEN = 1; }
void I2C_Stop()  { I2C_Wait(); SSP1CON2bits.PEN = 1; }
void I2C_RepStart() { I2C_Wait(); SSP1CON2bits.RSEN = 1; }

void I2C_Write(unsigned char data) {
    I2C_Wait();
    SSP1BUF = data;
}

unsigned char I2C_Read(unsigned char ack) {
    unsigned char data;
    I2C_Wait();
    SSP1CON2bits.RCEN = 1;       // Enable receive
    I2C_Wait();
    data = SSP1BUF;              // Read buffer
    I2C_Wait();
    SSP1CON2bits.ACKDT = !ack;   // Set ACK/NACK
    SSP1CON2bits.ACKEN = 1;      // Send ACK/NACK
    return data;
}

// --- Main Program ---
void main() {
    unsigned char myData = 0;
    
    I2C_Init(); // Setup I2C

    // 1. WRITE Operation (Write 0x55 to Address 0x00)
    I2C_Start();
    I2C_Write(0xA0);    // Device Address (Write Mode)
    I2C_Write(0x00);    // Memory Address
    I2C_Write(0x55);    // Data to write
    I2C_Stop();
    
    __delay_ms(10);     // Wait for write to finish

    // 2. READ Operation (Read from Address 0x00)
    I2C_Start();
    I2C_Write(0xA0);    // Device Address (Write Mode)
    I2C_Write(0x00);    // Memory Address to read from
    I2C_RepStart();     // Restart
    I2C_Write(0xA1);    // Device Address (Read Mode)
    myData = I2C_Read(0); // Read byte with NACK
    I2C_Stop();

    while(1); // Done
}