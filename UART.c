/*
 * UART.c
 *
 *  Created on: Oct 2, 2016
 *      Author: Yiming Wang
 */
#include "msp.h"
#include "interface.h"
extern volatile uint8_t rxd_command;

void uart_putchar(uint8_t tx_data){
	while(!(UCA3IFG & UCTXIFG));
	UCA3TXBUF = tx_data;          // Load data onto buffer
}

void uart_putchar_n(uint8_t *BufferPtr,uint32_t length){
	while(length != 0){
		while(!(UCA3IFG & UCTXIFG));
		//while(rxd_command!=0x3F);	//check the confirm information sent from Simblee
		UCA3TXBUF = *BufferPtr;
		BufferPtr++;
		length--;
		 //__delay_cycles(36000);		//delay 12ms to sync with Simblee
		 //__delay_cycles(45000);		//delay 15ms to sync with Simblee
		 __delay_cycles(39000);		//delay 13ms to sync with Simblee
	}
}

uint8_t getchar(){
	while(!(UCA3IFG & UCRXIFG));
		return UCA3RXBUF;          // Load data onto buffer
}
void configure_serial_port(int baud){
	// Configure UART pins, set 2-UART pin as primaryfunction
	P9SEL0 |= (BIT6 | BIT7);
	P9SEL1|= 0;
	// Configure UART
	UCA3CTLW0 |= UCSWRST;       // Put eUSCI in reset
	UCA3CTLW0 |= UCSSEL_2;		// Select Frame parameters and clock source
	if(baud == 0){				//baudrate=9600
		UCA3BRW = 312;
		UCA3MCTLW = 8;
	}
	else if(baud == 1){
		UCA3BRW = 26;				// So we set the same Baud rate here.
		UCA3MCTLW = 0x01<<8;         		// Set first stage modulator bits
	}
	UCA3CTLW0 &= ~UCSWRST;      // Initialize eUSCI
	UCA3IE |= UCRXIE;          	// Enable USCI_A0 RX interrupts
	NVIC_ISER0 = 1 << ((INT_EUSCIA3 -16) & 31); // Enable eUSCIA0 interrupt in NVIC module
}

void eUSCIA3IsrHandler(void){
	if(UCA3IFG & UCRXIFG) {
		// code to handle RX interrupts
		UCA3IFG &= ~UCRXIFG;
		rxd_command = UCA3RXBUF;
		//if(rxd_command == 'C'){
		//    ADC_channel_stop();
		//}
	}
	if(UCA3IFG & UCTXIFG) {
		// code to handle TX interrupts
		UCA0IFG &= ~UCTXIFG;
	}
}

