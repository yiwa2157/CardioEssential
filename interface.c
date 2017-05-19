/*
 * interface.c
 *
 *  Created on: Oct 6, 2016
 *      Author: Yiming Wang
 */
#include "msp.h"
#include "ADC.h"
#include "UART.h"
#include "Timer16.h"
#include "interface.h"
#include <stdio.h>

extern volatile uint16_t num0;
extern volatile uint16_t num2;
extern volatile uint8_t rxd_command;

uint8_t num0_high=0;
uint8_t num0_low=0;

uint8_t type;
uint8_t transmit_state;
float temp;
void ADC_channel1_start(){
	/*while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
	REFCTL0 |= REFVSEL_0 | REFON;             // Enable internal 1.2V reference
	ADC14MCTL0 = ADC14INCH_14 | ADC14VRSEL_0;
	ADC14IER0 = ADC14IE0;	// Enable MCTL0,MCTL1,MCTL2 Interrupts
	while(!(REFCTL0 & REFGENRDY));      		// Wait for reference generator to settle
	ADC14CTL0 |= ADC14ENC;           			// Enable Conversions
	NVIC_ISER0 = 1 << ((INT_ADC14 -16) & 31);  	// Enable ADC intin NVIC module
	ADC14CTL0 |= ADC14ENC|ADC14SC;           			// Start Conversion*/

	configure_ADC(1);

	type= 0x01;
	transmit_state = 1;
}

void ADC_channel2_start(){
	/*while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
	REFCTL0 |= REFVSEL_0 | REFON;             // Enable internal 1.2V reference
	ADC14MCTL0 = ADC14INCH_18 | ADC14VRSEL_0;
	ADC14IER0 = ADC14IE0;	// Enable MCTL0,MCTL1,MCTL2 Interrupts
	while(!(REFCTL0 & REFGENRDY));      		// Wait for reference generator to settle
	ADC14CTL0 |= ADC14ENC;           			// Enable Conversions
	NVIC_ISER0 = 1 << ((INT_ADC14 -16) & 31);  	// Enable ADC intin NVIC module
	ADC14CTL0 |= ADC14ENC|ADC14SC;           			// Start Conversion*/

	configure_ADC(2);

	type= 0x02;
	transmit_state = 1;
}

void ADC_channel_stop(){
	ADC14CTL0 &= !(ADC14ENC|ADC14SC);
	transmit_state = 0;
}

void ADC_channel1_transmit(){
	uint16_t buffer_1[400];
	//uint16_t *p;
	uint16_t i;
	uint16_t idx_1;
	uint16_t idx_2;
	uint8_t peak = 1;
	uint8_t heart_rate;
	for (i = 0 ; i <= 399; i++){
		buffer_1[i] = num0;
		while(!(REFCTL0 & REFGENRDY));      		// Wait for reference generator to settle
		ADC14CTL0 |= ADC14ENC|ADC14SC;           	// Start Conversion
		__delay_cycles(15000); 						//ADC Sampling rate =200hz
	}

	for (i = 1 ; i <= 398; i++){
		if(peak == 1){
			if ((buffer_1[i] >= 7000)&(buffer_1[i] > buffer_1[i-1])&(buffer_1[i] > buffer_1[i+1])){ //find peak
				idx_1 = i;
				peak ++;
			}
		}
		else if(peak == 2){
			if ((buffer_1[i] >= 7000)&(buffer_1[i] > buffer_1[i-1])&(buffer_1[i] > buffer_1[i+1])){ //find peak
				idx_2 = i;
				peak ++;
			}
		}
	}
	heart_rate = 12000/(idx_2-idx_1);

	//uint8_t batt_bar = battery_level_detectation();
	//char name[]="start";			//for ecg2mcu test only
	//printf("%s\n",name);			//for ecg2mcu test only
	for (i=0; i <= 399; i ++){
		num0_high = ((buffer_1[i])&(0x3F00))>>8;
		num0_low  = (buffer_1[i])&0xFF;

		if(num0_low == 0x00){num0_low = 0x01;}

		char package []= {0x5A,num0_high,num0_low};
		uart_putchar_n((uint8_t*)package,sizeof(package));
		if(rxd_command == 0x43){
			ADC_channel_stop();
			return;
		}
		//printf("%d\n",buffer_1[i]);		//for ecg2mcu test only
	}

	char package []= {0x5A,0x00,heart_rate};
	uart_putchar_n((uint8_t*)package,sizeof(package));
	//printf("heart_rate:%d\n",heart_rate);
}

void ADC_channel2_transmit(){
	while(!(REFCTL0 & REFGENRDY));      		// Wait for reference generator to settle
			ADC14CTL0 |= ADC14ENC|ADC14SC;           	// Start Conversion
			 //temp = 0.465*(((float)num0)/(16384-(float)num0));
			 //temp = temp*-23.83+71.34;		//celceius
			 //temp = temp*1.8+32;				//faranheit
			 //printf("%d\n",buffer[i]);		//for ecg2mcu test only
			 //printf("%f\n",temp);		//for ecg2mcu test only
			num0_high = ((num0)&(0x3F00))>>8;
			num0_low  = (num0)&0xFF;
			char package []= {0x5A,num0_high,num0_low};
			uart_putchar_n((uint8_t*)package,sizeof(package));
			 if(rxd_command == 0x43){
			 ADC_channel_stop();
			 	return;
			 }
}

uint8_t battery_level_detectation(){
	uint8_t battery_bar;
	if(num2 >= 12320){
		battery_bar = 4;
	}
	else if(num2 >= 12164){
		battery_bar = 3;
	}
	else if(num2 >= 12008){
		battery_bar = 2;
	}
	else if(num2 >= 11852){
		battery_bar = 1;
	}
	else {
		battery_bar = 0;
	}
	//if (battery_bar <= 1){
	//	char batt_low [] = "batt_warning";
	//	uart_putchar_n((uint8_t*)batt_low,sizeof(batt_low));
	//}
	return battery_bar;
}
