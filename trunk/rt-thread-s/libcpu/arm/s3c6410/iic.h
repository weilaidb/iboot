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
#ifndef __IIC_H__
#define __IIC_H__


typedef struct _rt_iic
{
	int index;

	rt_uint32_t regbase;
	rt_uint32_t clock;
} rt_iic_t;



void rt_hw_i2c_init(rt_iic_t *pi2c, rt_uint32_t speed, rt_uint32_t slaveadd);
int rt_hw_i2c_probe(rt_iic_t *pi2c, rt_uint8_t chip);
int rt_hw_i2c_read(rt_iic_t *pi2c, rt_uint8_t chip, rt_uint32_t addr, int alen, rt_uint8_t *buffer, int len);
int rt_hw_i2c_write(rt_iic_t *pi2c, rt_uint8_t chip, rt_uint32_t addr, int alen, rt_uint8_t *buffer, int len);


#endif /* __IIC_H__ */
