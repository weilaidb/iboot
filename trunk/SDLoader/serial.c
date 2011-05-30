/*
 * serial.c
 *
 *  Created on: 2011-5-21
 *      Author: sangwei
 */
#include "s3c6410.h"
#include "tiny6410.h"
#include "serial.h"


int serial_init(int baudrate)
{
	unsigned int regv;

    // UART I/O port initialize (RXD0 : GPA0, TXD0: GPA1)
    regv = s3c_readl(GPACON);
    regv = (regv & ~(0xff<<0)) | (0x22<<0);	// GPA0->RXD0, GPA1->TXD0
    s3c_writel(regv, GPACON);
    regv = s3c_readl(GPAPUD);
    regv = (regv & ~(0xf<<0)) | (0x1<<0);	// RXD0: Pull-down, TXD0: pull up/down disable
    s3c_writel(regv, GPAPUD);

    /// Normal Mode, No Parity, 1 Stop Bit, 8 Bit Data
    s3c_writel(0x0003, ULCON0);
    // PCLK divide, Polling Mode
    regv = (1<<9) | (1<<8) | (1<<2) | (1<<0);
    s3c_writel(regv, UCON0);
    // Disable FIFO
    s3c_writel(0x0000, UFCON0);
    // Disable Auto Flow Control
    s3c_writel(0x0000, UMCON0);

    // Baud rate, DIV=PCLK/(BPS*16)-1; 66000000/(115200*16)-1 = 34;
    s3c_writel(34, UBRDIV0);
    for (regv=0; regv<0x100; regv++);
    //aSlotTable[DivSlot];
    s3c_writel(0x80, UDIVSLOT0);

	return 0;
}

void serial_putc(const char ch)
{
	/* wait for room in the tx FIFO */
	while (!(s3c_readl(UTRSTAT0) & 0x2));
	s3c_writel(ch, UTXH0);

	/* If \n, also do \r */
	if (ch == '\n')
		serial_putc('\r');
}

void serial_puts(const char *str)
{
	while (*str)
		serial_putc(*str++);
}

static char get_hex(unsigned char v)
{
	if (v >= 0 && v <= 9)
		return v + '0';
	else
		return v - 10 + 'A';
}

void print_dword(unsigned int val)
{
	char str[12];
	unsigned char i, chv, *p;

	p = (unsigned char *)&val;
	for (i=0; i<4; i++)
	{
		chv = *p++;
		str[7-i*2-1] = get_hex((chv & 0xF0) >> 4);
		str[7-i*2] = get_hex(chv & 0xF);
	}
	str[8] = '\n';
	str[9] = 0;

	serial_puts(str);
}
