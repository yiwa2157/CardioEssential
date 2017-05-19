/*
 * ADC.c
 *
 *  Created on: Oct 2, 2016
 *      Author: Yiming Wang
 */
#include "msp.h"
volatile uint16_t num0;
volatile uint16_t num1;
volatile uint16_t num2;

void configure_ADC(uint8_t channel){
		P6SEL0 |= BIT1;
		P6SEL1 |= 0;
		//P4SEL0 |= (BIT0 | BIT2);    //P4.0:ECG
		//P4SEL1 |= BIT2;				//P4.2:battery level detectation
		P8SEL0 |= BIT7;
		//Initialize the shared reference module
		//By default, REFMSTR=1 => REFCTL is used to configure the internal reference
		while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
		REFCTL0 |= REFVSEL_0 | REFON;             // Enable internal 1.2V reference
		REFCTL0 |= REFTCOFF;                     // Turn off Temperature Sensor
		//Configure ADC -Pulse sample mode; ADC14SC trigger
		//ADC ON
		ADC14CTL0 |= ADC14SHT0_0 | ADC14ON | ADC14SHP | ADC14PDIV_0 |ADC14SSEL__SYSCLK;//| ADC14CONSEQ_1
		ADC14CTL1 |= ADC14RES_0;       //  Set resolution

		if (channel==1){
			ADC14MCTL0 = ADC14INCH_14 | ADC14VRSEL_0;	// ECG
		}
		else if(channel==2){
			ADC14MCTL0 = ADC14INCH_18 | ADC14VRSEL_0;	// temperature
		}
		//ADC14MCTL2 = ADC14INCH_11 | ADC14VRSEL_0 | ADC14EOS;	// Map z-axis Analog channel to MEM2/MCTL2, set 3.3v ref

		ADC14IER0 = ADC14IE0;	// Enable MCTL0,MCTL1,MCTL2 Interrupts
		while(!(REFCTL0 & REFGENRDY));      		// Wait for reference generator to settle
		ADC14CTL0 |= ADC14ENC;           			// Enable Conversions
		NVIC_ISER0 = 1 << ((INT_ADC14 -16) & 31);  	// Enable ADC intin NVIC module

}

void ADC14IsrHandler(void){
	if(ADC14IFGR0 & ADC14IFG0){
		num0 = ADC14MEM0;
		//printf("num0 is %ld\n",ADC14MEM0);
	}
	if(ADC14IFGR0 & ADC14IFG1){
		num1 = ADC14MEM1;
		//printf("num1 is %ld\n",ADC14MEM1);
	}
	if(ADC14IFGR0 & ADC14IFG2){
			num2 = ADC14MEM2;
			//printf("num2 is %ld\n",ADC14MEM2);
		}

}

