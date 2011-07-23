/*
 * File      : led.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-05-29     swkyer       tiny6410
 */
#include <rtthread.h>
#include "s3c6410.h"


#define TIMER3_PENDING_CLEAR 	(1U<<8)
#define TIMER3_INTERRUPT_ENABLE	(1<<3)
#define SYS_TIMER_PRESCALER		2
#define SYS_TIMER_DIVIDER		1

#define SAMPLE_BPS				9600
#define REQ_INFO				0x60U


static inline void set_pin_as_input(void)
{
	rt_ubase_t regv;

	regv = s3c_readl(GPFCON);
	regv &= ~(3 << 30);
	s3c_writel(regv, GPFCON);
}

static inline void set_pin_as_output(void)
{
	rt_ubase_t regv;

	regv = s3c_readl(GPFCON);
	regv &= ~(3 << 30);
	regv |= (1 << 30);
	s3c_writel(regv, GPFCON);
}

static inline void set_pin_value(int v)
{
	rt_ubase_t regv;

	regv = s3c_readl(GPFDAT);
	if (v)
		regv |= (1 << 15);
	else
		regv &= ~(1 << 15);
	s3c_writel(regv, GPFDAT);
}

static inline int get_pin_value(void)
{
	rt_ubase_t regv;

	regv = s3c_readl(GPFDAT);
	return (regv & (1 << 15)) ? 1 : 0;
}

static void init_timer(void)
{
	rt_ubase_t regv;
	rt_uint32_t pclk;

	pclk = s3c_get_pclk();
	regv = (pclk / (SYS_TIMER_PRESCALER * SYS_TIMER_DIVIDER * SAMPLE_BPS) - 1);
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
	rt_ubase_t regv;

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
	rt_ubase_t regv;

	regv = s3c_readl(TCON);
	regv &= ~(1 << 16);
	s3c_writel(regv, TCON);
}

static void wait_timer_tick(void)
{
	rt_ubase_t regv;

	while ((s3c_readl(TINT_CSTAT) & (1U << 8)) == 0);

	regv = s3c_readl(TINT_CSTAT) & 0x1F;
	regv |= TIMER3_PENDING_CLEAR;
	s3c_writel(regv, TINT_CSTAT);
}

static rt_uint8_t crc8(rt_uint32_t v, rt_uint32_t len);

static int one_wire_session(rt_uint8_t req, rt_uint8_t res[])
{
	rt_ubase_t i, Req, *Res;

	Req = (req << 24) | (crc8(req << 24, 8) << 16);
	Res = (rt_ubase_t *)res;

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

static int try_one_wire_session(rt_uint8_t req, rt_uint8_t res[])
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

static int get_info(rt_uint8_t *lcd, rt_uint16_t *firmware_ver)
{
	rt_uint8_t res[4];

	//rt_kprintf("~~%d~~%02x-%02x-%02x-%02x\n", __LINE__, res[0], res[1], res[2], res[3]);
	if (!try_one_wire_session(REQ_INFO, res))
	{
		//rt_kprintf("~~%d~~%02x-%02x-%02x-%02x\n", __LINE__, res[0], res[1], res[2], res[3]);
		return 0;
	}

	*lcd = res[3];
	*firmware_ver = res[2] * 100 + res[1];
	return 1;
}

int rt_hw_set_lcd_backlight(unsigned brightness)
{
	rt_uint8_t res[4];

	if (brightness > 127)
		brightness = 127;

	return try_one_wire_session(brightness | 0x80U, res);
}

static rt_uint8_t crc8(rt_uint32_t v, rt_uint32_t len)
{
	rt_uint8_t crc = 0xACU;

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
	rt_uint8_t lcd = 0;
	rt_uint16_t lcd_fw_ver = 0;

	init_one_wire();
	if (get_info(&lcd, &lcd_fw_ver))
	{
		rt_hw_set_lcd_backlight(0);
		rt_kprintf("lcd: %02x, firmware version: %04d\n", lcd, lcd_fw_ver);
	}
}

#ifdef RT_USING_FINSH
#include "finsh.h"

long bl_on(void)
{
	rt_uint8_t lcd = 0;
	rt_uint16_t lcd_fw_ver = 0;

	init_one_wire();
	if (get_info(&lcd, &lcd_fw_ver))
	{
		rt_hw_set_lcd_backlight(20);
		rt_kprintf("lcd: %02x, firmware version: %04d\n", lcd, lcd_fw_ver);
	}
	rt_kprintf("Lcd BackLight On!\n");

	return 0;
}
FINSH_FUNCTION_EXPORT(bl_on, lcd backlight on);

long bl_off(void)
{
	rt_uint8_t lcd = 0;
	rt_uint16_t lcd_fw_ver = 0;

	init_one_wire();
	if (get_info(&lcd, &lcd_fw_ver))
	{
		rt_hw_set_lcd_backlight(0);
		rt_kprintf("lcd: %02x, firmware version: %04d\n", lcd, lcd_fw_ver);
	}
	rt_kprintf("Lcd BackLight Off!\n");

	return 0;
}
FINSH_FUNCTION_EXPORT(bl_off, lcd backlight off);
#endif /* end of RT_USING_FINSH */
