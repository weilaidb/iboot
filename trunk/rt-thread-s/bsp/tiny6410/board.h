/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-05-11     swkyer       for tiny6410
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <s3c6410.h>
#include <serial.h>

void rt_hw_board_init(void);
void rt_hw_sdcard_init(void);

#endif