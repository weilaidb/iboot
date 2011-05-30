/*
 * lcd_bl.c
 *
 *  Created on: 2011-5-29
 *      Author: sangwei
 */
#include "s3c6410.h"


#define TIMER3_PENDING_CLEAR 	(1U<<8)
#define TIMER3_INTERRUPT_ENABLE	(1<<3)
#define SYS_TIMER_PRESCALER		2
#define SYS_TIMER_DIVIDER		1
#define S3C6410_PCLK			(66*1000*1000)

#define SAMPLE_BPS				9600
#define REQ_INFO				0x60U


static inline void set_pin_as_input(void)
{
	unsigned int regv;

	regv = s3c_readl(GPFCON);
	regv &= ~(3 << 30);
	s3c_writel(regv, GPFCON);
}

static inline void set_pin_as_output(void)
{
	unsigned int regv;

	regv = s3c_readl(GPFCON);
	regv &= ~(3 << 30);
	regv |= (1 << 30);
	s3c_writel(regv, GPFCON);
}

static inline void set_pin_value(int v)
{
	unsigned int regv;

	regv = s3c_readl(GPFDAT);
	if (v)
		regv |= (1 << 15);
	else
		regv &= ~(1 << 15);
	s3c_writel(regv, GPFDAT);
}

static inline int get_pin_value(void)
{
	unsigned int regv;

	regv = s3c_readl(GPFDAT);
	return !!(regv & (1 << 15));
}

static void init_timer(void)
{
	unsigned int regv;

	regv = (S3C6410_PCLK / (SYS_TIMER_PRESCALER * SYS_TIMER_DIVIDER * SAMPLE_BPS) - 1);
	s3c_writel(regv, TCNTB3);

	regv = s3c_readl(TINT_CSTAT) & 0x1F;
	regv |= TIMER3_PENDING_CLEAR;
	s3c_writel(regv, TINT_CSTAT);
	regv = s3c_readl(TINT_CSTAT) & 0x1F;
	regv |= TIMER3_INTERRUPT_ENABLE;
	s3c_writel(regv, TINT_CSTAT);
}

static void start_timer(void)
{
	unsigned int regv;

	regv = s3c_readl(TCON);
	regv &= ~(0xf << 16);				// Timer3 Stop
	s3c_writel(regv, TCON);
	regv = s3c_readl(TCON);
	regv |= (1 << 17);					// update TCNTB3
	s3c_writel(regv, TCON);
	regv = s3c_readl(TCON);
	regv &= ~(1 << 17);
	s3c_writel(regv, TCON);
	regv = s3c_readl(TCON);
	regv |= ((1 << 19) | (1 << 16));	// AutoReload mode, Timer3 Start
	s3c_writel(regv, TCON);
}

static inline void stop_timer(void)
{
	unsigned int regv;

	regv = s3c_readl(TCON);
	regv &= ~(1 << 16);
	s3c_writel(regv, TCON);
}

static void wait_timer_tick(void)
{
	unsigned int regv;

	while ((s3c_readl(TINT_CSTAT) & (1U << 8)) == 0);

	regv = s3c_readl(TINT_CSTAT) & 0x1F;
	regv |= TIMER3_PENDING_CLEAR;
	s3c_writel(regv, TINT_CSTAT);
}

static unsigned char crc8(unsigned v, unsigned len);

static int one_wire_session(unsigned char req, unsigned char res[])
{
	unsigned int i, Req, *Res;

	Req = (req << 24) | (crc8(req << 24, 8) << 16);
	Res = (unsigned *)res;

	set_pin_value(1);
	set_pin_as_output();
	start_timer();
	for (i = 0; i < 60; i++)
		wait_timer_tick();

	set_pin_value(0);
	for (i = 0; i < 2; i++)
		wait_timer_tick();

	for (i = 0; i < 16; i++)
	{
		int v = !!(Req & (1U << 31));
		Req <<= 1;
		set_pin_value(v);
		wait_timer_tick();
	}
	wait_timer_tick();
	set_pin_as_input();
	wait_timer_tick();
	for (i = 0; i < 32; i++)
	{
		(*Res) <<= 1;
		(*Res) |= get_pin_value();
		wait_timer_tick();
	}
	stop_timer();
	set_pin_value(1);
	set_pin_as_output();

	return crc8(*Res, 24) == res[0];
}

static int try_one_wire_session(unsigned char req, unsigned char res[])
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (one_wire_session(req, res))
			return 1;
	}
	return 0;
}

static void init_one_wire(void)
{
	init_timer();
}

static int get_info(unsigned char *lcd, unsigned short *firmware_ver)
{
	unsigned char res[4];

	if (!try_one_wire_session(REQ_INFO, res))
		return 0;

	*lcd = res[3];
	*firmware_ver = res[2] * 100 + res[1];
	return 1;
}

int rt_hw_set_lcd_backlight(unsigned brightness)
{
	unsigned char res[4];

	if (brightness > 127)
		brightness = 127;

	return try_one_wire_session(brightness | 0x80U, res);
}

static unsigned char crc8(unsigned v, unsigned len)
{
	unsigned char crc = 0xACU;

	while (len--)
	{
		if ((crc & 0x80U) != 0)
		{
			crc <<= 1;
			crc ^= 0x7U;
		}
		else
		{
			crc <<= 1;
		}

		if ((v & (1U << 31)) != 0)
			crc ^= 0x7U;

		v <<= 1;
	}
	return crc;
}

void rt_hw_lcd_bl_init(void)
{
	unsigned char lcd = 0;
	unsigned short lcd_fw_ver = 0;

	init_one_wire();
	if (get_info(&lcd, &lcd_fw_ver))
	{
		rt_hw_set_lcd_backlight(0);
		//rt_kprintf("lcd: %02x, firmware version: %04d\n", lcd, lcd_fw_ver);
	}
}
