/*
 * File      : sdcard.h
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
#ifndef __SDCARD_H__
#define __SDCARD_H__


#define SDCLK			20000000
#define MMCCLK			15000000


typedef struct _rt_crad
{
	int present;
} rt_card_t;


#endif /* end of __SDCARD_H__ */
