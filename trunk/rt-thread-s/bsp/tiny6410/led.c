/*
 * File      : led.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-05-11     swkyer       tiny6410
 */

/**
 * @addtogroup tiny6410
 */
/*@{*/
#include <rtthread.h>
#include <s3c6410.h>
#include "led.h"

void rt_hw_led_init(void)
{
	register rt_ubase_t regv;

	/* GPK4,GPK5,GPK6,GPK7 for LED */
	regv = s3c_readl(GPKCON0);
	regv = (regv & ~(0xffffU<<16))|(0x1111U<<16);
	s3c_writel(regv, GPKCON0);

	regv = s3c_readl(GPKPUD);
	regv  = (regv & ~(0xffU << 8))|(0x00U<<8);
	s3c_writel(regv, GPKPUD);
}

void rt_hw_led_on(unsigned char value)
{
	register rt_ubase_t regv;

	regv = s3c_readl(GPKDAT);
	regv &= ~ ((value & 0x0f) << 4);
	s3c_writel(regv, GPKDAT);
}

void rt_hw_led_off(unsigned char value)
{
	register rt_ubase_t regv;

	regv = s3c_readl(GPKDAT);
	regv |= (value & 0x0f) << 4;
	s3c_writel(regv, GPKDAT);
}

/*@}*/

