/* 
 * File:   LabWork5.c
 * Author: Halil Ibrahim Bekli
 *
 * Target: PIC16F1518
 * Description: Lab 0 Project - Adapted for the schematic with CORRECTED PINOUT.
 *
 */

#include <xc.h>
#include <string.h>

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = ON
#pragma config CP = OFF, BOREN = OFF, CLKOUTEN = OFF
#pragma config IESO = OFF, FCMEN = OFF, WRT = OFF
#pragma config STVREN = ON, LVP = OFF

#define _XTAL_FREQ 16000000

/* ---- VARIANT 1 ---- */
#define SECRET_CODE "123"
#define CODE_LEN    3
#define CONFIRM_KEY '#'
#define LATCH_TIME  300
#define LATCH_PIN   LATAbits.LATA0

/* ---- LCD (4-bit, PORTB) ---- */
#define RS LATBbits.LATB6
#define EN LATBbits.LATB7
#define D4 LATBbits.LATB2
#define D5 LATBbits.LATB3
#define D6 LATBbits.LATB4
#define D7 LATBbits.LATB5

/* ---- GLOBALS ---- */
char entered_code[4];
unsigned char index = 0;

/* ---- LCD FUNCTIONS ---- */
void lcd_pulse(void){
    EN = 1; __delay_us(1); EN = 0;
}

void lcd_send4(unsigned char d){
    D4 = d & 1;
    D5 = (d >> 1) & 1;
    D6 = (d >> 2) & 1;
    D7 = (d >> 3) & 1;
    lcd_pulse();
}

void lcd_cmd(unsigned char cmd){
    RS = 0;
    lcd_send4(cmd >> 4);
    lcd_send4(cmd);
    __delay_ms(2);
}

void lcd_putch(char c){
    RS = 1;
    lcd_send4(c >> 4);
    lcd_send4(c);
}

void lcd_puts(const char *s){
    while(*s) lcd_putch(*s++);
}

void lcd_goto(unsigned char p){
    lcd_cmd(0x80 | p);
}

void lcd_init(void){
    RS = EN = 0;
    __delay_ms(20);
    lcd_send4(0x03); __delay_ms(5);
    lcd_send4(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
}

/* ---- KEYPAD (4x4, PORTC) ---- */
char read_kpad(void){
    const char map[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    for(unsigned char r=0;r<4;r++){
        LATC = 0x0F;
        LATC &= ~(1<<r);
        __delay_us(50);

        for(unsigned char c=0;c<4;c++){
            if(!(PORTC & (1<<(c+4)))){
                while(!(PORTC & (1<<(c+4))));
                return map[r][c];
            }
        }
    }
    return 0;
}

/* ---- MAIN ---- */
void main(void){
    OSCCON = 0b01111000;

    ANSELA = ANSELB = ANSELC = 0;
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0xF0;

    LATCH_PIN = 0;

    lcd_init();

    while(1){
        index = 0;
        memset(entered_code,0,sizeof(entered_code));

        lcd_cmd(0x01);
        lcd_goto(0x00);
        lcd_puts("Laukiu kodo");
        lcd_goto(0x40);

        while(1){
            char key = read_kpad();
            if(key){
                __delay_ms(150);
                if(key == CONFIRM_KEY) break;

                if(key>='0' && key<='9' && index<CODE_LEN){
                    entered_code[index++] = key;
                    lcd_putch('*');
                }
            }
        }

        if(strcmp(entered_code, SECRET_CODE) == 0){
            lcd_cmd(0x01);
            lcd_puts("Atrakinta");

            LATCH_PIN = 1;
            __delay_ms(LATCH_TIME);
            LATCH_PIN = 0;

            __delay_ms(2000);
        } else {
            lcd_cmd(0x01);
            lcd_puts("Klaida");
            __delay_ms(1000);
        }
    }
}