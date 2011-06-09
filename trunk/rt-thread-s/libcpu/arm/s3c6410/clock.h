/*
 * File      : cpu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-05-29     swkyer       first version
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <rtdef.h>


rt_uint32_t s3c_get_arm_clk(void);
rt_uint32_t s3c_get_fclk(void);
rt_uint32_t s3c_get_hclk(void);
rt_uint32_t s3c_get_pclk(void);
rt_uint32_t s3c_get_uclk(void);

void rt_print_cpuinfo(void);
void rt_hw_clock_init(void);

#endif /* end of __CLOCK_H__ */
