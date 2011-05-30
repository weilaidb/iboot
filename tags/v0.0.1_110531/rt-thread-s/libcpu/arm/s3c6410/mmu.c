/*
 * File      : mmu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-04-25     Yi.qiu       first version
 * 2009-12-18     Bernard      port to armcc
 */

#include <rtthread.h>
#include "s3c6410.h"

#define _MMUTT_STARTADDRESS	 (0x00004000)	// virtual address

#define DESC_SEC			(0x2|(1<<4))
#define CB					(3<<2)  //cache_on, write_back
#define CNB					(2<<2)  //cache_on, write_through
#define NCB					(1<<2)  //cache_off,WR_BUF on
#define NCNB				(0<<2)  //cache_off,WR_BUF off
#define AP_RW				(3<<10) //supervisor=RW, user=RW
#define AP_RO				(2<<10) //supervisor=RW, user=RO

#define DOMAIN_FAULT		(0x0)
#define DOMAIN_CHK			(0x1)
#define DOMAIN_NOTCHK		(0x3)
#define DOMAIN0				(0x0<<5)
#define DOMAIN1				(0x1<<5)

#define DOMAIN0_ATTR		(DOMAIN_CHK<<0)
#define DOMAIN1_ATTR		(DOMAIN_FAULT<<2)

#define RW_CB				(AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB				(AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB				(AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT			(AP_RW|DOMAIN1|NCNB|DESC_SEC)

#ifdef __GNUC__
void mmu_setttbase(register rt_uint32_t i)
{
	asm ("mcr	p15, 0, %0, c2, c0, 0": :"r" (i));
}

void mmu_set_domain(register rt_uint32_t i)
{
	asm ("mcr	p15, 0, %0, c3, c0, 0": :"r" (i));
}

void mmu_enable()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i |= 0x1;

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i &= ~0x1;

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_icache()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i |= (1 << 12);

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_dcache()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i |= (1 << 2);

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_icache()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i &= ~(1 << 12);

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_dcache()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i &= ~(1 << 2);

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_alignfault()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i |= (1 << 1);

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_alignfault()
{
	register rt_uint32_t i;

	/* read control register */
	asm ("mrc	p15, 0, %0, c1, c0, 0":"=r" (i));

	i &= ~(1 << 1);

	/* write back to control register */
	asm ("mcr	p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_clean_invalidated_cache_index(int index)
{
	asm ("mcr	p15, 0, %0, c7, c14, 2": :"r" (index));
}

void mmu_invalidate_tlb()
{
	asm ("mcr	p15, 0, %0, c8, c7, 0": :"r" (0));
}

void mmu_invalidate_icache()
{
	asm ("mcr	p15, 0, %0, c7, c5, 0": :"r" (0));
}
#endif

#ifdef __CC_ARM
void mmu_setttbase(rt_uint32_t i)
{
    __asm
    {
        mcr		p15, 0, i, c2, c0, 0
    }
}

void mmu_set_domain(rt_uint32_t i)
{
    __asm
    {
        mcr		p15, 0, i, c3, c0,  0
    }
}

void mmu_enable()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        orr		value, value, #0x01
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_disable()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        bic		value, value, #0x01
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_enable_icache()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        orr		value, value, #0x1000
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_enable_dcache()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        orr		value, value, #0x04
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_disable_icache()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        bic		value, value, #0x1000
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_disable_dcache()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        bic		value, value, #0x04
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_enable_alignfault()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        orr		value, value, #0x02
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_disable_alignfault()
{
    register rt_uint32_t value;

    __asm
    {
        mrc		p15, 0, value, c1, c0, 0
        bic		value, value, #0x02
        mcr		p15, 0, value, c1, c0, 0
    }
}

void mmu_clean_invalidated_cache_index(int index)
{
    __asm
    {
        mcr		p15, 0, index, c7, c14, 2
    }
}

void mmu_invalidate_tlb()
{
    register rt_uint32_t value;

    value = 0;
    __asm
    {
        mcr		p15, 0, value, c8, c7, 0
    }
}

void mmu_invalidate_icache()
{
    register rt_uint32_t value;

    value = 0;

    __asm
    {
        mcr		p15, 0, value, c7, c5, 0
    }
}
#endif

void mmu_setmtt(rt_uint32_t vaddrStart, rt_uint32_t vaddrEnd, rt_uint32_t paddrStart, rt_uint32_t attr)
{
    volatile rt_uint32_t *pTT;
    volatile int i, nSec;

    pTT = (rt_uint32_t *)_MMUTT_STARTADDRESS + (vaddrStart>>20);
    nSec = (vaddrEnd>>20) - (vaddrStart>>20);
    for (i=0; i<=nSec; i++)
    {
		*pTT = attr |(((paddrStart>>20)+i)<<20);
		pTT++;
    }
}

void rt_hw_mmu_init(void)
{

	// the SDLoader initialize MMU like this
/*	int i;

	mmu_disable_dcache();
	mmu_disable_icache();
	mmu_invalidate_dcache();
	mmu_invalidate_icache();

	// icache may be turned on here.
	mmu_enable_icache();

	mmu_disable();
	mmu_invalidate_tlb();

	// clear all page tables
	for (i=0; i<(4*1024); i++)
	{
		*(volatile unsigned int *)(_MMUTT_STARTADDRESS + i*4) = 0;
	}
	mmu_setmtt(0x00000000, 0x0c000000, 0x50000000, RW_CNB);		// ddr, 256M
	mmu_setmtt(0x0c000000, 0x10000000, 0x0c000000, RW_CNB);		// stepping-stone
	mmu_setmtt(0x70000000, 0x80000000, 0x70000000, RW_NCNB);	// SFR

	mmu_setttbase(_MMUTT_STARTADDRESS);

	// DOMAIN1: no_access, DOMAIN0,2~15=client(AP is checked)
	mmu_set_domain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR);

	mmu_enable_alignfault();

	mmu_enable();

	// ICache enable
	mmu_enable_icache();
	// DCache should be turned on after mmu is turned on.
	mmu_enable_dcache();
*/

	// re-init the memory map
	mmu_setmtt(0x00000000, 0x10000000, 0x50000000, RW_CNB);		// ddr, 256M
	mmu_setmtt(0x70000000, 0x80000000, 0x70000000, RW_NCNB);	// SFR
}

