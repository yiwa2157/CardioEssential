/*
 * interface.h
 *
 *  Created on: Oct 6, 2016
 *      Author: Yiming Wang
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

void ADC_channel1_start();
void ADC_channel2_start();
void ADC_channel_stop();
void ADC_channel1_transmit();
void ADC_channel2_transmit();
uint8_t battery_level_detectation();

#endif /* INTERFACE_H_ */
