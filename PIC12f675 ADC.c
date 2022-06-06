/*
 
                      (ˇ_ˇ”) ƪ(˘⌣˘)┐ ƪ(˘⌣˘)ʃ ┌(˘⌣˘)ʃ
 
 *******************************************************************************
 * File                  :   main.c				  
 * Author                :   Cemal KOR										  
 * Compiler              :   MPLAB X IDE v6.00									  
 * Target                :   PIC12F675									  
 * Notes                 :   None											  
 *******************************************************************************
 */

#include <xc.h>

#define _XTAL_FREQ   4000000

// CONFIG  PIC12F675 Configuration Bit Settings 
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

const float voltage_rate = 1024.0 / 5.0;

/*
 * Function Name:   InitADC
 */
void InitADC() {
    ANSEL = 0x15; // Clear Pin selection bits
    TRISIO = 0x2f; // GP4 output, rest all input
    ADCON0 = 0x81; // Turn on the A/D Converter ADFM and ADON
    CMCON = 0x07; // Shut off the Comparator, so that pins are available for ADC
    VRCON = 0x00; // Shut off the Voltage Reference for Comparator
}

/*
 * Function Name: GetADCValue0
 * Used for analog readings for pin GP0.
 */
unsigned int GetADCValue0(void) {
    ADCON0 &= 0xf3; // Clear Channel selection bits 
    ADCON0 |= 0x00; // Select GP0 pin as ADC input

    __delay_ms(20); // Time for Acqusition capacitor to charge up and show correct value

    GO_nDONE = 1; // Enable Go/Done

    while (GO_nDONE); //wait for conversion completion

    return ((ADRESH << 8) + ADRESL); // Return 10 bit ADC value
}

/*
 * Function Name: GetADCValue1
 * Used for analog readings for pin GP2.
 */
unsigned int GetADCValue1(void) {
    ADCON0 &= 0xf3; // Clear Channel selection bits 

    ADCON0 |= 0x08; // Select GP2 pin as ADC input

    __delay_ms(20); // Time for Acqusition capacitor to charge up and show correct value

    GO_nDONE = 1; // Enable Go/Done

    while (GO_nDONE); //wait for conversion completion

    return ((ADRESH << 8) + ADRESL); // Return 10 bit ADC value
}

/* Main function*/
void main(void) {
    unsigned int ADC_value0 = 0, ADC_value2 = 0;
    InitADC(); // Initialize GP0, GP2 as ADC input pin

    while (1) {
        ADC_value0 = GetADCValue0(); // Read ADC value for GP0 pin
        ADC_value2 = GetADCValue1(); // Read ADC value for GP2 pin
        
        /* if(GP0 > 3V && GP2 > 3.3V && GP1 == 0 && GP5 == 0) */
        if (ADC_value0 > 3 * voltage_rate && ADC_value2 > 3.3 * voltage_rate && !GP1 && !GP5) {
            GP4 = 0;    //GP4 off

        } else {
            GP4 = 1;    //GP4 on

        }

    }

}
/*
          THE END
        ＼（＾▽＾）／
 */