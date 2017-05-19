/*
 * ADC.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Yiming Wang
 */

#ifndef ADC_H_
#define ADC_H_

#define num0_low_tag 	0x00
#define num0_high_tag 	0x40
#define num1_low_tag 	0x80
#define num1_high_tag 	0xC0
void configure_ADC(uint8_t channel);
void ADC14IsrHandler(void);


#endif /* ADC_H_ */
