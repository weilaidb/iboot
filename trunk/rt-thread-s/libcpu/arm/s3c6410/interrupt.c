/*
 * File      : trap.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-13     Bernard      first version
 */

#include <rtthread.h>
#include "s3c6410.h"

#define MAX_HANDLERS	IRQ_TOTAL

extern rt_uint32_t rt_interrupt_nest;

/* exception and interrupt handler table */
rt_uint32_t rt_interrupt_from_thread, rt_interrupt_to_thread;
rt_uint32_t rt_thread_switch_interrput_flag;

/**
 * @addtogroup S3C6410
 */
/*@{*/

rt_isr_handler_t rt_hw_interrupt_handle(int vector)
{
	rt_kprintf("Unhandled interrupt occured, addr: 0x%08x!!!\n", s3c_readl(VIC0VECTADDRESS));
	// clear the interrupt
	s3c_writel(0, VIC0VECTADDRESS);
	s3c_writel(0, VIC1VECTADDRESS);
	return RT_NULL;
}

/**
 * This function will initialize hardware interrupt
 */
void rt_hw_interrupt_init(void)
{
	register rt_uint32_t idx;

	/* all=IRQ mode */
	s3c_writel(0x0, VIC0INTSELECT);
	s3c_writel(0x0, VIC1INTSELECT);

	/* all interrupt disabled include global bit */
    s3c_writel(IRQ_ALLMSK, VIC0INTENCLEAR);
    s3c_writel(IRQ_ALLMSK, VIC1INTENCLEAR);

	/* all clear interrupt pending */


	/* init exceptions table */
	for (idx=0; idx < MAX_HANDLERS; idx++)
	{
		if (idx < IRQ_VIC1_BASE)
			s3c_writel((rt_ubase_t)rt_hw_interrupt_handle, VIC0VECTADDR(idx));
		else
			s3c_writel((rt_ubase_t)rt_hw_interrupt_handle, VIC1VECTADDR(idx-IRQ_VIC1_BASE));
	}

	/* init interrupt nest, and context in thread sp */
	rt_interrupt_nest = 0;
	rt_interrupt_from_thread = 0;
	rt_interrupt_to_thread = 0;
	rt_thread_switch_interrput_flag = 0;
}

/**
 * This function will mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_mask(int vector)
{
	register rt_uint32_t regv;

	if (vector < IRQ_VIC1_BASE)
	{
		regv = s3c_readl(VIC0INTENABLE);
		regv &= ~BIT(vector);
		s3c_writel(regv, VIC0INTENABLE);
	}
	else
	{
		regv = s3c_readl(VIC1INTENABLE);
		regv &= ~BIT(vector-IRQ_VIC1_BASE);
		s3c_writel(regv, VIC1INTENABLE);
	}
}

/**
 * This function will un-mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_umask(int vector)
{
	register rt_uint32_t regv;

	if (vector < IRQ_VIC1_BASE)
	{
		regv = s3c_readl(VIC0INTENABLE);
		regv |= BIT(vector);
		s3c_writel(regv, VIC0INTENABLE);
	}
	else
	{
		regv = s3c_readl(VIC1INTENABLE);
		regv |= BIT(vector-IRQ_VIC1_BASE);
		s3c_writel(regv, VIC1INTENABLE);
	}
}

/**
 * This function will install a interrupt service routine to a interrupt.
 * @param vector the interrupt number
 * @param new_handler the interrupt service routine to be installed
 * @param old_handler the old interrupt service routine
 */
void rt_hw_interrupt_install(int vector, rt_isr_handler_t new_handler, rt_isr_handler_t *old_handler)
{
	if (vector < IRQ_VIC1_BASE)
	{
		if (*old_handler != RT_NULL)
			*old_handler = (rt_isr_handler_t)s3c_readl(VIC0VECTADDR(vector));
		s3c_writel((rt_ubase_t)new_handler, VIC0VECTADDR(vector));
		return;
	}

	if (vector < MAX_HANDLERS)
	{
		vector -= IRQ_VIC1_BASE;

		if (*old_handler != RT_NULL)
			*old_handler = (rt_isr_handler_t)s3c_readl(VIC1VECTADDR(vector));
		s3c_writel((rt_ubase_t)new_handler, VIC1VECTADDR(vector));
		return;
	}
}

/*@}*/
