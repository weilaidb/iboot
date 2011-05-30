/*
 * serial.h
 *
 *  Created on: 2011-5-21
 *      Author: sangwei
 */
#ifndef __SERIAL_H__
#define __SERIAL_H__


int serial_init(int baudrate);
void serial_putc(const char ch);
void serial_puts(const char *str);

void print_dword(unsigned int val);

#endif /* end of __SERIAL_H__ */
