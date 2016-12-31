/* 
 * File:   main.c
 * Author: Josefe
 *
 * Created on 20 de diciembre de 2015, 20:32
 */

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF       // MCLR Pin Function Select (MCLR/VPP pin function is RA5)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


#define _XTAL_FREQ 500000       

#define PLAY    1
#define SILENCE 2
#define END     3

#define T2_TRESHOLD 122

#include <xc.h>
#include <pic16f1826.h>
//#include <config.h>

/*Variables*/
unsigned char *note_index; //Pointer to data address
int note_period; //Period ROM index
unsigned char status; // 1 Play, 2 Silence, 3 end

/*Constants*/
//Musical sequence for Cumpleanos feliz
const unsigned char notes_values_C[] = { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 60, 60, 60, 60, 60, 60, 128, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 128, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 60, 60, 60, 60, 60, 60, 128, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 128, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 128, 60, 60, 60, 60, 60, 60, 128, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 128, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 128, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 128, 70, 70, 70, 70, 70, 70, 128, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 255 };
//Musical sequence for Feliz en tu dia
const unsigned char notes_values_F[]={ 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 128, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 128, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 128, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 128, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 128, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 128, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 128, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 128, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 128, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 128, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 74, 128, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 128, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 128, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 255 };
//Freq to period ROM
const unsigned char notes_periodsH[]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const unsigned char notes_periodsL[]={ 239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119, 113, 106, 100, 95 };;

void t1_init(void);
void t2_init(void);

void main (void){

    /*Oscillator configurations*/
    OSCCONbits.SPLLEN=0; // PLL disabled
    OSCCONbits.IRCF=0b0111; //500 Khz internal oscillator
    OSCCONbits.SCS=0b100;   //Internal oscillator source
    

    /*Port configuration*/
    TRISBbits.TRISB3=0; //Port B3 as Speaker output
    LATBbits.LATB3=0;
    TRISBbits.TRISB1=1;  //Port B1 as song select input
    TRISBbits.TRISB2=1;  //Port B2 as loop mode input
    ANSELBbits.ANSB1=0; 
    ANSELBbits.ANSB2=0;
    OPTION_REGbits.nWPUEN=0;    //Weak pull-up enabled
    WPUBbits.WPUB1=1;
    WPUBbits.WPUB2=1;

    /*Variables configuration*/    
    note_period=0;
    status=SILENCE;
    if(!PORTBbits.RB1){  //If mode song sw ON ('0' logic) play Feliz en tu dia
        note_index=notes_values_F;
    }
    else{               //Else play Cumpleanos feliz
        note_index=notes_values_C;
    }

    /*Timer1 (freq generator) configuration*/
    t1_init();

    /*timer2 (beat source) configuration*/
    t2_init();

    /*CCP1 configuration*/
    CCP1CONbits.CCP1M=11; //Compare mode and event trigger
                          //Timer1 resets on match  

    /*Interrupts configuration*/
    PIE1bits.CCP1IE=1;  //CCP interrupt
    PIE1bits.TMR2IE=1;  //Timer 2 interrupt
    PIR1bits.CCP1IF=0;  //Clear Flags
    PIR1bits.TMR2IF=0;
    INTCONbits.PEIE=1;  //Peripheral interrupts enabled
    INTCONbits.GIE=1;   //Global interrupts enabled

    /*Start timers*/
    T2CONbits.TMR2ON=1;
    T1CONbits.TMR1ON=1;

    while(1){
        if(status==END){ //If sequence has ended
            if(!PORTBbits.RB2){ //If loop mode sw ON ('0' logic)             
                    asm("RESET");  //Reset and play again              
            }else{
                   INTCONbits.GIE=0;   //Global interrupt disabled                   
                   asm("SLEEP");       //Sleep

            }

        }
    }

}


void t1_init (void){
    T1GCON=0;           //No gate trigger
    T1CONbits.TMR1CS=0; //System clock/4
    T1CONbits.T1CKPS=0; //No prescale =>Fint=Fosc/(4*notes_periods_L[i])=2*Fnote
    T1CONbits.T1OSCEN=0; //Dedicated oscillator disabled
    TMR1H=0;
    TMR1L=0;
}

void t2_init(void){
    T2CONbits.T2OUTPS=1; //1:2 postscaler output
    T2CONbits.T2CKPS=2;  //Prescaler input 1:16 =>Fcount=Fosc/256
    TMR2=0;             //Reset counter
    PR2=T2_TRESHOLD;    //Set compare treshold =>Fint=32Hz => BPM=120

}

void interrupt ISR(void){

    if(PIR1bits.CCP1IF){    //If frequency generator comparator matches
       PIR1bits.CCP1IF=0;   //Reset flag
       //FSM
       if(status==PLAY){    //If it is playing a note
           LATBbits.LATB3=!LATBbits.LATB3; //Toggle output
       }
       if(status==SILENCE){ //If note is silence
           LATBbits.LATB3=0; //Output off
       }
       CCPR1H=notes_periodsH[note_period]; //Latch periods    
       CCPR1L=notes_periodsL[note_period];        
       
    }

    if(PIR1bits.TMR2IF){
        PIR1bits.TMR2IF=0; //Clear flag
        if (*note_index<128){ //If valid note
            note_period=(*note_index-60); //Set freq
            status=PLAY;
           }
        if (*note_index==128){ //If silence
            note_period=0; // Interrupts each DO period pooling if silence has ended
            status=SILENCE;         
        }
        if (*note_index>128){
            status=END;
            T1CONbits.TMR1ON=0; //Stop t1
            T2CONbits.TMR2ON=0; //Stop t2
            LATBbits.LATB3=0; //Switch off output
            CCP1CONbits.CCP1M=0; //Switch off CCP

        }
        TMR2=0; //Reset t2 counter
        note_index++; //Increment data pointer
    }

}