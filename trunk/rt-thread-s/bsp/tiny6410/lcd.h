/*
 * File      : lcd.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-03-29     Yi.Qiu
 * 2011-05-29     swkyer       tiny6410
 */
#ifndef __LCD_H__
#define __LCD_H__

#include <rtthread.h>

/* LCD driver for N3'5 */
#define LCD_WIDTH				480
#define LCD_HEIGHT				272
#define LCD_PIXCLOCK			4
#define LCD_RIGHT_MARGIN		36
#define LCD_LEFT_MARGIN			19
#define LCD_HSYNC_LEN			5

#define LCD_UPPER_MARGIN		1
#define LCD_LOWER_MARGIN		5
#define LCD_VSYNC_LEN			1


#define RT_DEVICE_CTRL_LCD_GET_WIDTH			0
#define RT_DEVICE_CTRL_LCD_GET_HEIGHT			1
#define RT_DEVICE_CTRL_LCD_GET_BPP			 	2
#define RT_DEVICE_CTRL_LCD_GET_FRAMEBUFFER		3


void rt_hw_lcd_init();


#endif
