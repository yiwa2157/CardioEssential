/*
 * UART.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Yiming Wang
 */

#ifndef UART_H_
#define UART_H_

#define BAUD_9600 0
#define BAUD_115200 1

void configure_serial_port(int baud);
void uart_putchar(uint8_t tx_data);
void uart_putchar_n(uint8_t *BufferPtr,uint32_t length);
uint8_t uart_getchar();
void eUSCIA3IsrHandler(void);


#endif /* UART_H_ */
