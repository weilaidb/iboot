/*
 * clock.c
 *
 *  Created on: 2011-5-8
 *      Author: sangwei
 */
#include <rtthread.h>
#include "s3c6410.h"
#include "tiny6410.h"
#include "clock.h"

#define APLL 0
#define MPLL 1
#define EPLL 2

/* ------------------------------------------------------------------------- */
/* NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_fclk(), get_hclk(), get_pclk() and get_uclk() return the clock of
 * the specified bus in HZ.
 */
static rt_uint32_t get_pll_clk(int pllreg)
{
	rt_uint32_t r, m, p, s;

	if (pllreg == APLL)
		r = s3c_readl(APLL_CON);
	else if (pllreg == MPLL)
		r = s3c_readl(MPLL_CON);
	else if (pllreg == EPLL)
		r = s3c_readl(EPLL_CON0);
	else
		return 0;

	m = (r>>16) & 0x3ff;
	p = (r>>8) & 0x3f;
	s = r & 0x7;

	return (m * (CONFIG_SYS_CLK_FREQ / (p * (1 << s))));
}

/* return ARMCORE frequency */
rt_uint32_t s3c_get_arm_clk(void)
{
	rt_uint32_t div;

	div = s3c_readl(CLK_DIV0);

	return (get_pll_clk(APLL) / ((div & 0x7) + 1));
}

/* return FCLK frequency */
rt_uint32_t s3c_get_fclk(void)
{
	return (get_pll_clk(APLL));
}

/* return HCLK frequency */
rt_uint32_t s3c_get_hclk(void)
{
	rt_uint32_t fclk;

	rt_uint32_t hclkx2_div = ((s3c_readl(CLK_DIV0)>>9) & 0x7) + 1;
	rt_uint32_t hclk_div = ((s3c_readl(CLK_DIV0)>>8) & 0x1) + 1;

	if (s3c_readl(OTHERS) & 0x80)
		fclk = s3c_get_fclk();			// SYNC Mode
	else
		fclk = get_pll_clk(MPLL);		// ASYNC Mode

	return fclk/(hclk_div * hclkx2_div);
}

/* return PCLK frequency */
rt_uint32_t s3c_get_pclk(void)
{
	rt_uint32_t fclk;
	rt_uint32_t hclkx2_div = ((s3c_readl(CLK_DIV0)>>9) & 0x7) + 1;
	rt_uint32_t pre_div = ((s3c_readl(CLK_DIV0)>>12) & 0xf) + 1;

	if(s3c_readl(OTHERS) & 0x80)
		fclk = s3c_get_fclk();			// SYNC Mode
	else
		fclk = get_pll_clk(MPLL);		// ASYNC Mode

	return fclk/(hclkx2_div * pre_div);
}

/* return UCLK frequency */
rt_uint32_t s3c_get_uclk(void)
{
	return (get_pll_clk(EPLL));
}

void rt_print_cpuinfo(void)
{
	rt_kprintf("\nCPU:  S3C6410@%dMHz\n", s3c_get_arm_clk()/1000000);
	rt_kprintf("      ARMclk = %dMHz, Hclk = %dMHz, Pclk = %dMHz, ",
			s3c_get_fclk()/1000000, s3c_get_hclk()/1000000, s3c_get_pclk()/1000000);
	if (s3c_readl(OTHERS) & 0x80)
		rt_kprintf("(SYNC Mode)\n");
	else
		rt_kprintf("(ASYNC Mode)\n");
}


/**
 * @brief System Clock Configuration
 */
void rt_hw_clock_init(void)
{
	// TODO
}
