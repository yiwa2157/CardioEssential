//*****************************************************************************
//
// MSP432 main.c
//
//****************************************************************************

#include "msp.h"
#include "stdio.h"
#include "stdlib.h"
#include "ADC.h"
#include "UART.h"
#include "Timer16.h"
#include "interface.h"
//#include "spi.h"

extern volatile uint16_t num0;
extern volatile uint16_t num2;
extern uint8_t transmit_state;
extern uint8_t type;
volatile uint8_t rxd_command;
//#include <float.h>

void main(void)
{
	P2DIR=BIT5|BIT4;
	P1DIR=BIT0;
	P5DIR=BIT6;
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer
    configure_serial_port(BAUD_9600);
    __enable_interrupt();
    while(1){
    	if(!transmit_state){
    		//char get = getchar();
    		char get = rxd_command;
    		//printf("%d\n",get);
    		while(!(UCA3IFG & UCTXIFG));
    				UCA3TXBUF =0x5A;
    		switch (get){
    		case 'A'://channel 1 sampling & transmitting
    			ADC_channel1_start();
    			break;
    		case 'B'://channel 2 sampling & transmitting
    			ADC_channel2_start();
    			break;
    		case 'C'://sampling & transmitting stop
    			ADC_channel_stop();
    			break;
    		}
    	}
    	else{
    		switch(type){
    		case 0x01:
    			ADC_channel1_transmit();
    		break;
    		case 0x02:
    		    ADC_channel2_transmit();
    		break;
    		}
    	}
    }
}
