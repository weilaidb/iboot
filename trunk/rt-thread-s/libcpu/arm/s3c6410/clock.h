/*
 * clock.h
 *
 *  Created on: 2011-5-8
 *      Author: sangwei
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
