/*
 * File      : sdcard.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * xxxx           u-boot
 * 2011-05-29     swkyer       tiny6410
 */
#include <rthw.h>
#include "s3c6410.h"
#include "tiny6410.h"
#include "clock.h"
#include "sdmmc_protocol.h"
#include "sdcard.h"




static void sd_sys_clk_init(void)
{
	rt_uint32_t regv, val;

	regv = s3c_readl(HCLK_GATE);
	regv &= ~(1 << 17);
	s3c_writel(regv, HCLK_GATE);	// close hsmmc0 clock

	regv = s3c_readl(CLK_SRC);
	regv &= ~(0x03 << 18);			// EPLL
	s3c_writel(regv, CLK_SRC);

	val = s3c_get_uclk() / SDCLK;
	regv = s3c_readl(CLK_DIV1);
	regv &= ~0x0f;
	regv |= (val - 1);
	s3c_writel(regv, CLK_DIV1);

	regv = s3c_readl(HCLK_GATE);
	regv |= (1 << 17);
	s3c_writel(regv, HCLK_GATE);	// open hsmmc0 clock, 20MHz
}
/*
static rt_uint32_t sd_clk_get(rt_card_t *psd)
{
	rt_uint32_t regv;

	regv = s3c_get_uclk();
	regv /= (s3c_readl(CLK_DIV1) & 0x0f) + 1;
	regv /= ((s3c_readw(psd->regbase + oCLKCON) & 0xff00) >> 8) * 2;

	return regv;
}

static void sd_clk_init(rt_card_t *psd, rt_uint8_t divider)
{
	// SD clock input is 20MHz
	s3c_writew((divider << 8), psd->regbase + oCLKCON);
}

static void sd_clk_enable(rt_card_t *psd)
{
	rt_uint16_t wval;

	wval = 0x0101;
	s3c_writew(wval, psd->regbase + oCLKCON);	// 21M/2 = 10.5M
	// wait for stable
	while (!(s3c_readw(psd->regbase + oCLKCON) & 0x02));

	wval = s3c_readw(psd->regbase + oCLKCON);
	wval |= (1 << 2);
	s3c_writew(wval, psd->regbase + oCLKCON);
}

static void sd_clk_disable(rt_card_t *psd)
{
	rt_uint16_t wval;

	wval = s3c_readw(psd->regbase + oCLKCON);
	wval &= ~(1 << 2);
	s3c_writew(wval, psd->regbase + oCLKCON);
}

static void sd_bit_width(rt_card_t *psd, int bitw)
{
	rt_uint8_t bval;

	bval = s3c_readb(psd->regbase + oHOSTCTL);
	if (bitw == 4)			// 4-bit
	{
		bval &= ~(1 << 5);
		bval |= (1 << 1);
	}
	else if (bitw == 8)		// 8-bit
	{
		bval |= (1 << 5);
		bval &= ~(1 << 1);
	}
	else					// 1-bit
	{
		bval &= ~(1 << 5);
		bval &= ~(1 << 1);
	}
	s3c_writeb(bval, psd->regbase + oHOSTCTL);
}


void sd_cfg(rt_card_t *psd)
{
	rt_uint32_t ctrl2, ctrl3;

	s3c_writel(SD_CONTROL4_DRIVE_9mA, psd->regbase + oCONTROL4);

	ctrl2 = s3c_readl(psd->regbase + oCONTROL2);
	ctrl2 &= SD_CTRL2_SELBASECLK_MASK;
	ctrl2 |= (SD_CTRL2_ENSTAASYNCCLR | SD_CTRL2_ENCMDCNFMSK
			| SD_CTRL2_ENFBCLKRX | SD_CTRL2_DFCNT_NONE
			| SD_CTRL2_ENCLKOUTHOLD);

	ctrl3 = (SD_CTRL3_FCSEL3 | SD_CTRL3_FCSEL2
			| SD_CTRL3_FCSEL1 | SD_CTRL3_FCSEL0);

	rt_kprintf("[SD%d] CTRL2=%08x, CTRL3=%08x\n", psd->index, ctrl2, ctrl3);
	s3c_writel(ctrl2, psd->regbase + oCONTROL2);
	s3c_writel(ctrl3, psd->regbase + oCONTROL3);

	// TODO, configure the multi-function pin
}
*/

static int wait_for_r_buf_ready(rt_card_t *psd)
{
	rt_uint32_t uloop = 0;

	while (!(s3c_readw(psd->regbase + oNORINTSTS) & 0x20))
	{
		if (uloop % 500000 == 0 && uloop > 0)
			return 0;
		uloop++;
	}
	return 1;
}

static int wait_for_cmd_done(rt_card_t *psd)
{
	rt_uint32_t i;
	rt_uint16_t n_int, e_int;

	rt_kprintf("SD[%d] wait_for_cmd_done\n", psd->index);
	for (i=0; i<0x20000000; i++)
	{
		n_int = s3c_readw(psd->regbase + oNORINTSTS);
		//rt_kprintf("SD[%d] HM_NORINTSTS: %04x\n", psd->index, n_int);
		if (n_int & 0x8000)
			break;
		if (n_int & 0x0001)
			return 0;
	}

	e_int = s3c_readw(psd->regbase + oERRINTSTS);
	s3c_writew(e_int, psd->regbase + oERRINTSTS);
	s3c_writew(n_int, psd->regbase + oNORINTSTS);
	rt_kprintf("SD[%d] cmd error1: 0x%04x, HM_NORINTSTS: 0x%04x\n", psd->index, e_int, n_int);
	return -1;
}

static int wait_for_data_done(rt_card_t *psd)
{
	while (!(s3c_readw(psd->regbase + oNORINTSTS) & 0x2))
		return 1;
	return 0;
}

static void clear_command_complete_status(rt_card_t *psd)
{
	s3c_writew(1 << 0, psd->regbase + oNORINTSTS);
	while (s3c_readw(psd->regbase + oNORINTSTS) & 0x1)
		s3c_writew(1 << 0, psd->regbase + oNORINTSTS);
}

static void clear_transfer_complete_status(rt_card_t *psd)
{
	s3c_writew(s3c_readw(psd->regbase + oNORINTSTS) | 0x2, psd->regbase + oNORINTSTS);
	while (s3c_readw(psd->regbase + oNORINTSTS) & 0x2)
		s3c_writew(s3c_readw(psd->regbase + oNORINTSTS) | 0x2, psd->regbase + oNORINTSTS);
}

static void clear_buffer_read_ready_status(rt_card_t *psd)
{
	s3c_writew(s3c_readw(psd->regbase + oNORINTSTS) | 0x20, psd->regbase + oNORINTSTS);
	while (s3c_readw(psd->regbase + oNORINTSTS) & 0x20)
		s3c_writew(s3c_readw(psd->regbase + oNORINTSTS) | 0x20, psd->regbase + oNORINTSTS);
}

static void sdmmc_reset(rt_card_t *psd)
{
	s3c_writeb(0x3, psd->regbase + oSWRST);
}

static void sdmmc_set_gpio(rt_card_t *psd)
{
	rt_uint32_t regv;

	if (psd->index == 0)
	{
		// GPIOG0..6
		regv = s3c_readl(GPGCON) & 0xf0000000;
		s3c_writel(regv | 0x02222222, GPGCON);

		regv = s3c_readl(GPGPUD) & 0xfffff000;
		s3c_writel(regv, GPGPUD);
	}
	else if (psd->index == 1)
	{
		// GPIOH0..5
		s3c_writel(0x00222222, GPHCON0);
		s3c_writel(0x00000000, GPHCON1);

		regv = s3c_readl(GPHPUD) & 0xfffff000;
		s3c_writel(regv, GPHPUD);
	}
	else
		rt_kprintf("SD[%d] ### HS-MMC channel is not defined!\n", psd->index);
}

static void set_transfer_mode_register(rt_card_t *psd, rt_uint32_t MultiBlk,
										rt_uint32_t DataDirection, rt_uint32_t AutoCmd12En,
										rt_uint32_t BlockCntEn, rt_uint32_t DmaEn)
{
	s3c_writew((s3c_readw(psd->regbase + oTRNMOD) & ~(0xffff)) | (MultiBlk << 5)
			| (DataDirection << 4) | (AutoCmd12En << 2)
			| (BlockCntEn << 1) | (DmaEn << 0), psd->regbase + oTRNMOD);
	rt_kprintf("SD[%d] HM_TRNMOD = 0x%04x\n", psd->index, s3c_readw(psd->regbase + oTRNMOD));
}

static void set_arg_register(rt_card_t *psd, rt_uint32_t arg)
{
	s3c_writel(arg, psd->regbase + oARGUMENT);
}

static void set_blkcnt_register(rt_card_t *psd, rt_uint16_t uBlkCnt)
{
	s3c_writew(uBlkCnt, psd->regbase + oBLKCNT);
}

static void set_system_address_reg(rt_card_t *psd, rt_uint32_t SysAddr)
{
	s3c_writel(SysAddr, psd->regbase + oSDMASYSAD);
}

static void set_blksize_register(rt_card_t *psd, rt_uint16_t uDmaBufBoundary,
									rt_uint16_t uBlkSize)
{
	s3c_writew((uDmaBufBoundary << 12) | (uBlkSize), psd->regbase + oBLKSIZE);
}

static void clear_err_interrupt_status(rt_card_t *psd)
{
	while (s3c_readw(psd->regbase + oNORINTSTS) & (0x1 << 15))
	{
		s3c_writew(s3c_readw(psd->regbase + oNORINTSTS), psd->regbase + oNORINTSTS);
		s3c_writew(s3c_readw(psd->regbase + oERRINTSTS), psd->regbase + oERRINTSTS);
	}
}

static void interrupt_enable(rt_card_t *psd, rt_uint16_t NormalIntEn,
								rt_uint16_t ErrorIntEn)
{
	clear_err_interrupt_status(psd);
	s3c_writew(NormalIntEn, psd->regbase + oNORINTSTSEN);
	s3c_writew(ErrorIntEn, psd->regbase + oERRINTSTSEN);
}

static void clock_onoff(rt_card_t *psd, int on)
{
	rt_uint16_t reg16;

	if (on == 0)
	{
		reg16 = s3c_readw(psd->regbase + oCLKCON) & ~(0x1<<2);
		s3c_writew(reg16, psd->regbase + oCLKCON);
	}
	else
	{
		reg16 = s3c_readw(psd->regbase + oCLKCON);
		s3c_writew(reg16 | (0x1<<2), psd->regbase + oCLKCON);

		while (1)
		{
			reg16 = s3c_readw(psd->regbase + oCLKCON);
			if (reg16 & (0x1<<3))	//  SD_CLKSRC is Stable
				break;
		}
	}
}

static void set_clock(rt_card_t *psd, rt_uint32_t clksrc, rt_uint32_t div)
{
	rt_uint16_t reg16;
	rt_uint32_t i;

	// rx feedback control
	s3c_writel(0xC0004100 | (clksrc << 4), psd->regbase + oCONTROL2);
	// Low clock: 00008080
	s3c_writel(0x00008080, psd->regbase + oCONTROL3);
	s3c_writel(0x3 << 16, psd->regbase + oCONTROL4);

	s3c_writew(s3c_readw(psd->regbase + oCLKCON) & ~(0xff << 8), psd->regbase + oCLKCON);
	// SDCLK Value Setting + Internal Clock Enable
	s3c_writew(((div<<8) | 0x1), psd->regbase + oCLKCON);

	// CheckInternalClockStable
	for (i=0; i<0x10000; i++)
	{
		reg16 = s3c_readw(psd->regbase + oCLKCON);
		if (reg16 & 0x2)
			break;
	}
	if (i == 0x10000)
		rt_kprintf("SD[%d] internal clock stabilization failed\n", psd->index);

	rt_kprintf("SD[%d] HM_CONTROL2(0x80) = 0x%08x\n", psd->index, s3c_readl(psd->regbase + oCONTROL2));
	rt_kprintf("SD[%d] HM_CONTROL3(0x84) = 0x%08x\n", psd->index, s3c_readl(psd->regbase + oCONTROL3));
	rt_kprintf("SD[%d] HM_CLKCON  (0x2c) = 0x%04x\n", psd->index, s3c_readw(psd->regbase + oCLKCON));

	clock_onoff(psd, 1);
}

static void set_cmd_register(rt_card_t *psd, rt_uint16_t cmd,
								rt_uint32_t data, rt_uint32_t flags)
{
	rt_uint16_t val = (cmd << 8);

	if (cmd == MMC_STOP_TRANSMISSION)
		val |= (3 << 6);

	if (flags & MMC_RSP_136)			// Long RSP
		val |= 0x01;
	else if (flags & MMC_RSP_BUSY)		// R1B
		val |= 0x03;
	else if (flags & MMC_RSP_PRESENT)	// Normal RSP
		val |= 0x02;

	if (flags & MMC_RSP_OPCODE)
		val |= (1<<4);

	if (flags & MMC_RSP_CRC)
		val |= (1<<3);

	if (data)
		val |= (1<<5);

	rt_kprintf("SD[%d] cmdreg =  0x%04x\n", psd->index, val);
	s3c_writew(val, psd->regbase + oCMDREG);
}


#define WAIT_PRNSTS_CNT		0x1000000
static int issue_command(rt_card_t *psd, rt_uint16_t cmd,
						rt_uint32_t arg, rt_uint32_t data, rt_uint32_t flags)
{
	int i;

	rt_kprintf("SD[%d] ### issue_command: %d, %08x, %d, %08x\n", psd->index, cmd, arg, data, flags);
	// Check CommandInhibit_CMD
	for (i=0; i<WAIT_PRNSTS_CNT; i++)
	{
		if (!(s3c_readl(psd->regbase + oPRNSTS) & 0x1))
			break;
	}
	if (i == WAIT_PRNSTS_CNT)
		rt_kprintf("SD[%d] ### rHM_PRNSTS: %08lx\n", psd->index, s3c_readl(psd->regbase + oPRNSTS));

	// Check CommandInhibit_DAT
	if (flags & MMC_RSP_BUSY)
	{
		for (i=0; i<WAIT_PRNSTS_CNT; i++)
		{
			if (!(s3c_readl(psd->regbase + oPRNSTS) & 0x2))
				break;
		}
		if (i == WAIT_PRNSTS_CNT)
			rt_kprintf("SD[%d] ### rHM_PRNSTS: %08lx\n", psd->index, s3c_readl(psd->regbase + oPRNSTS));
	}

	// set CMD & ARG
	s3c_writel(arg, psd->regbase + oARGUMENT);
	set_cmd_register(psd, cmd, data, flags);

	if (wait_for_cmd_done(psd))
		return 0;
	clear_command_complete_status(psd);

	if (!(s3c_readw(psd->regbase + oNORINTSTS) & 0x8000))
	{
		return 1;
	}
	else
	{
		if (psd->ocr_check == 1)
			return 0;
		else
		{
			rt_kprintf("SD[%d] Command = %d, Error Stat = 0x%04x\n", psd->index,
					(s3c_readw(psd->regbase + oCMDREG) >> 8),
					s3c_readw(psd->regbase + oERRINTSTS));
			return 0;
		}
	}
}

static void set_mmc_speed(rt_card_t *psd, rt_uint32_t eSDSpeedMode)
{
	rt_uint32_t ucSpeedMode, arg;

	ucSpeedMode = (eSDSpeedMode == SD_SPEED_HIGH) ? 1 : 0;
	//  Change to the high-speed mode
	arg = (3 << 24) | (185 << 16) | (ucSpeedMode << 8);
	while (!issue_command(psd, MMC_SWITCH, arg, 0, MMC_RSP_R1B));
}

static void set_sd_speed(rt_card_t *psd, rt_uint32_t eSDSpeedMode)
{
	rt_uint32_t arg = 0;
	rt_uint8_t ucSpeedMode;
	int i;

	ucSpeedMode = (eSDSpeedMode == SD_SPEED_HIGH) ? 1 : 0;

	if (!issue_command(psd, MMC_SET_BLOCKLEN, 64, 0, MMC_RSP_R1))
	{
		rt_kprintf("SD[%d] CMD16 fail\n", psd->index);
	}
	else
	{
		set_blksize_register(psd, 7, 64);
		set_blkcnt_register(psd, 1);
		set_arg_register(psd, 0 * 64);

		set_transfer_mode_register(psd, 0, 1, 0, 0, 0);

		arg = (0x1 << 31) | (0xffff << 8) | (ucSpeedMode << 0);
		if (!issue_command(psd, MMC_SWITCH, arg, 0, MMC_RSP_R1B))
		{
			rt_kprintf("SD[%d] CMD6 fail\n", psd->index);
		}
		else
		{
			wait_for_r_buf_ready(psd);
			clear_buffer_read_ready_status(psd);

			for (i = 0; i < 16; i++)
				s3c_readl(psd->regbase + oBDATA);

			if (!wait_for_data_done(psd))
				rt_kprintf("SD[%d] Transfer NOT Complete\n", psd->index);

			clear_transfer_complete_status(psd);
		}
	}
}

static int get_sd_scr(rt_card_t *psd)
{
	rt_uint32_t uSCR1, uSCR2;

	if (!issue_command(psd, MMC_SET_BLOCKLEN, 8, 0, MMC_RSP_R1))
		return 0;
	else
	{
		set_blksize_register(psd, 7, 8);
		set_blkcnt_register(psd, 1);
		set_arg_register(psd, 0 * 8);

		set_transfer_mode_register(psd, 0, 1, 0, 0, 0);
		if (!issue_command(psd, MMC_APP_CMD, psd->rca<<16, 0, MMC_RSP_R1))
			return 0;
		else
		{
			if (!issue_command(psd, SD_APP_SEND_SCR, 0, 1, MMC_RSP_R1))
				return 0;
			else
			{
				wait_for_r_buf_ready(psd);
				clear_buffer_read_ready_status(psd);

				uSCR1 = s3c_readl(psd->regbase + oBDATA);
				uSCR2 = s3c_readl(psd->regbase + oBDATA);

				if (!wait_for_data_done(psd))
					rt_kprintf("SD[%d] Transfer NOT Complete\n", psd->index);

				clear_transfer_complete_status(psd);

				if (uSCR1 & 0x1)
					psd->sd_spec = 1;	/*  Version 1.10, support cmd6 */
				else
					psd->sd_spec = 0;	/*  Version 1.0 ~ 1.01 */

				rt_kprintf("SD[%d] sd_spec = %d(0x%08x)\n", psd->index, psd->sd_spec, uSCR1);
				return 1;
			}
		}
	}
}

static int check_card_status(rt_card_t *psd)
{
	if (!issue_command(psd, MMC_SEND_STATUS, psd->rca<<16, 0, MMC_RSP_R1))
	{
		return 0;
	}
	else
	{
		if (((s3c_readl(psd->regbase + oRSPREG0) >> 9) & 0xf) == 4)
		{
			rt_kprintf("SD[%d] Card is transfer status\n", psd->index);
			return 1;
		}
	}

	return 1;
}

static void set_hostctl_speed(rt_card_t *psd, rt_uint8_t mode)
{
	rt_uint8_t reg8;

	reg8 = s3c_readb(psd->regbase + oHOSTCTL) & ~(0x1<<2);
	s3c_writeb(reg8 | (mode<<2), psd->regbase + oHOSTCTL);
}

/* return 0: OK
 * return -1: error
 */
static int set_bus_width(rt_card_t *psd, rt_uint8_t width)
{
	rt_uint32_t arg = 0;
	rt_uint8_t reg = s3c_readb(psd->regbase + oHOSTCTL);
	rt_uint8_t bitmode = 0;

	rt_kprintf("SD[%d] bus width: %d\n", psd->index, width);

	switch (width)
	{
	case 8:
		width = psd->mmc_card ? 8 : 4;
		break;
	case 4:
	case 1:
		break;
	default:
		return -1;
	}

	rt_hw_interrupt_mask(IRQ_HSMMC0);	// Disable sd card interrupt

	if (psd->mmc_card)	 /* MMC Card */
	{
		/* MMC Spec 4.x or above */
		if (psd->mmc_spec == 4)
		{
			if (width == 1)
				bitmode = 0;
			else if (width == 4)
				bitmode = 1;
			else if (width == 8)
				bitmode = 2;
			else
			{
				rt_kprintf("SD[%d] #### unknown mode\n", psd->index);
				return -1;
			}

			arg = ((3 << 24) | (183 << 16) | (bitmode << 8));
			while (!issue_command(psd, MMC_SWITCH, arg, 0, MMC_RSP_R1B));
		}
		else
			bitmode = 0;
	}
	else	 /* SD Card */
	{
		if (!issue_command(psd, MMC_APP_CMD, psd->rca<<16, 0, MMC_RSP_R1))
			return -1;
		else
		{
			if (width == 1)	// 1-bits
			{
				bitmode = 0;
				if (!issue_command(psd, MMC_SWITCH, 0, 0, MMC_RSP_R1B))
					return -1;
			}
			else			// 4-bits
			{
				bitmode = 1;
				if (!issue_command(psd, MMC_SWITCH, 2, 0, MMC_RSP_R1B))
					return -1;
			}
		}
	}

	if (bitmode == 2)
		reg |= 1 << 5;
	else
		reg |= bitmode << 1;

	s3c_writeb(reg, psd->regbase + oHOSTCTL);
	rt_hw_interrupt_umask(IRQ_HSMMC0);
	rt_kprintf("SD[%d] transfer rHM_HOSTCTL(0x28) = 0x%02x\n", psd->index,
				s3c_readb(psd->regbase + oHOSTCTL));

	return 0;
}

static int set_sd_ocr(rt_card_t *psd)
{
	rt_uint32_t i, ocr;

	issue_command(psd, MMC_APP_CMD, 0x0, 0, MMC_RSP_R1);
	issue_command(psd, SD_APP_OP_COND, 0x0, 0, MMC_RSP_R3);
	ocr = s3c_readl(psd->regbase + oRSPREG0);
	rt_kprintf("SD[%d] ocr1: %08x\n", psd->index, ocr);

	for (i = 0; i < 250; i++)
	{
		issue_command(psd, MMC_APP_CMD, 0x0, 0, MMC_RSP_R1);
		issue_command(psd, SD_APP_OP_COND, ocr, 0, MMC_RSP_R3);

		ocr = s3c_readl(psd->regbase + oRSPREG0);
		rt_kprintf("SD[%d] ocr2: %08x\n", psd->index, ocr);
		if (ocr & (0x1 << 31))
		{
			rt_kprintf("SD[%d] Voltage range: ", psd->index);
			if (ocr & (1 << 21))
				rt_kprintf("2.7V ~ 3.4V");
			else if (ocr & (1 << 20))
				rt_kprintf("2.7V ~ 3.3V");
			else if (ocr & (1 << 19))
				rt_kprintf("2.7V ~ 3.2V");
			else if (ocr & (1 << 18))
				rt_kprintf("2.7V ~ 3.1V");

			if (ocr & (1 << 7))
				rt_kprintf(", 1.65V ~ 1.95V\n");
			else
				rt_kprintf("\n");

			psd->mmc_card = 0;
			return 1;
		}
		//udelay(1000);
	}

	// The current card is MMC card, then there's time out error, need to be cleared.
	clear_err_interrupt_status(psd);
	return 0;
}

static int set_mmc_ocr(rt_card_t *psd)
{
	rt_uint32_t i, ocr;

	for (i = 0; i < 250; i++)
	{
		issue_command(psd, MMC_SEND_OP_COND, 0x40FF8000, 0, MMC_RSP_R3);

		ocr = s3c_readl(psd->regbase + oRSPREG0);
		rt_kprintf("SD[%d] ocr1: %08x\n", psd->index, ocr);

		if (ocr & (0x1 << 31)) {
			rt_kprintf("SD[%d] Voltage range: ", psd->index);
			if (ocr & (1 << 21))
				rt_kprintf("2.7V ~ 3.4V");
			else if (ocr & (1 << 20))
				rt_kprintf("2.7V ~ 3.3V");
			else if (ocr & (1 << 19))
				rt_kprintf("2.7V ~ 3.2V");
			else if (ocr & (1 << 18))
				rt_kprintf("2.7V ~ 3.1V");
			psd->mmc_card = 1;
			if (ocr & (1 << 7))
				rt_kprintf(", 1.65V ~ 1.95V\n");
			else
				rt_kprintf("\n");
			return 1;
		}
	}

	// The current card is SD card, then there's time out error, need to be cleared.
	clear_err_interrupt_status(psd);
	return 0;
}

static void clock_config(rt_card_t *psd, rt_uint32_t clksrc, rt_uint32_t Divisior)
{
	rt_uint32_t SrcFreq, WorkingFreq;

	if (clksrc == SD_CLKSRC_HCLK)
		SrcFreq = s3c_get_hclk();
	else if (clksrc == SD_CLKSRC_EPLL)	// Epll Out 84MHz
		SrcFreq = s3c_get_uclk();
	else
		SrcFreq = s3c_get_hclk();

	WorkingFreq = SrcFreq / (Divisior * 2);
	rt_kprintf("SD[%d] Card Working Frequency = %dMHz\n", psd->index, WorkingFreq / (1000000));

	if (psd->mmc_card)
	{
		if (psd->mmc_spec == 4)
		{
			if (WorkingFreq > 20000000)
			{
				// It is necessary to enable the high speed mode in the card before changing the clock freq to a freq higher than 20MHz.
				set_mmc_speed(psd, SD_SPEED_HIGH);
				rt_kprintf("SD[%d] Set MMC High speed mode OK!!\n", psd->index);
			}
			else
			{
				set_mmc_speed(psd, SD_SPEED_NORMAL);
				rt_kprintf("SD[%d] Set MMC Normal speed mode OK!!\n", psd->index);
			}
		}
		else		// old version
			rt_kprintf("SD[%d] Old version MMC card can not support working frequency higher than 25MHz", psd->index);
	}

	if (WorkingFreq > 25000000)
		// Higher than 25MHz, it is necessary to enable high speed mode of the host controller.
		set_hostctl_speed(psd, SD_SPEED_HIGH);
	else
		set_hostctl_speed(psd, SD_SPEED_NORMAL);

	// when change the sd clock frequency, need to stop sd clock.
	clock_onoff(psd, 0);
	set_clock(psd, clksrc, Divisior);
	rt_kprintf("SD[%d] clock config rHM_HOSTCTL = 0x%02x\n", psd->index, s3c_readb(psd->regbase + oHOSTCTL));

}

static void check_dma_int(rt_card_t *psd)
{
	rt_uint32_t i;

	for (i = 0; i < 0x1000000; i++)
	{
		if (s3c_readw(psd->regbase + oNORINTSTS) & 0x0002)
		{
			rt_kprintf("SD[%d] Transfer Complete\n", psd->index);
			psd->dma_end = 1;
			s3c_writew(s3c_readw(psd->regbase + oNORINTSTS) | 0x0002, psd->regbase + oNORINTSTS);
			break;
		}
		if (s3c_readw(psd->regbase + oNORINTSTS) & 0x8000)
		{
			rt_kprintf("SD[%d] error found: %04x\n", psd->index, s3c_readw(psd->regbase + oERRINTSTS));
			break;
		}
	}
}
/*
static rt_uint32_t process_ext_csd(rt_card_t *psd)
{
	rt_uint8_t ext_csd[512];

	rt_memset(ext_csd, 0, sizeof(ext_csd));

	if (ext_csd >= (rt_uint8_t *)0xc0000000)
		set_system_address_reg(psd, VIRT_2_PHYS((rt_uint32_t)ext_csd));
	else
		set_system_address_reg(psd, (rt_uint32_t)ext_csd);

	set_blksize_register(psd, 7, 512);
	set_blkcnt_register(psd, 1);
	set_transfer_mode_register(psd, 0, 1, 0, 1, 1);

	while (!issue_command(psd, MMC_SEND_EXT_CSD, 0, 1, MMC_RSP_R1 | MMC_CMD_ADTC));

	check_dma_int(psd);
	while (!psd->dma_end);

	return (((ext_csd[215] << 24) | (ext_csd[214] << 16) | (ext_csd[213] << 8) | ext_csd[212]) / (2 * 1024));
}
*/
static void display_card_info(rt_card_t *psd)
{
	rt_uint32_t card_size;
	rt_uint32_t i, resp[4];
	rt_uint32_t c_size, c_size_multi, read_bl_len, read_bl_partial, blk_size;

	for (i=0; i<4; i++)
	{
		resp[i] = s3c_readl(psd->regbase + oRSPREG0+i*4);
		rt_kprintf("%08x\n", resp[i]);
	}

	read_bl_len = ((resp[2] >> 8) & 0xf);
	read_bl_partial = ((resp[2] >> 7) & 0x1);
	c_size = ((resp[2] & 0x3) << 10) | ((resp[1] >> 22) & 0x3ff);
	c_size_multi = ((resp[1] >> 7) & 0x7);

	card_size = (1 << read_bl_len) * (c_size + 1) * (1 << (c_size_multi + 2)) / 1048576;
	blk_size = (1 << read_bl_len);

	rt_kprintf("SD[%d] read_bl_len: %d\n", psd->index, read_bl_len);
	rt_kprintf("SD[%d] read_bl_partial: %d\n", psd->index, read_bl_partial);
	rt_kprintf("SD[%d] c_size: %d\n", psd->index, c_size);
	rt_kprintf("SD[%d] c_size_multi: %d\n", psd->index, c_size_multi);

	rt_kprintf("SD[%d] One Block Size: %dByte\n", psd->index, blk_size);
	rt_kprintf("SD[%d] Total Card Size: %dMByte\n\n", psd->index, card_size + 1);

	rt_kprintf("SD[%d] %d MB ", psd->index, card_size + 1);
	if (psd->card_mid == 0x15)
		rt_kprintf("(MoviNAND)");
	rt_kprintf("\n");
}
/*
static void DataRead_ForCompare(rt_card_t *psd, int StartAddr)
{
	rt_uint32_t i = 0, j = 0;

	COMPARE_INT_DONE = 0;

	s3c_writew(s3c_readw(psd->regbase + oNORINTSIGEN) & ~(0xffff), psd->regbase + oNORINTSIGEN);

	Compare_buffer_HSMMC = (rt_uint32_t *) SDI_Compare_buffer_HSMMC;
	for (i = 0; i < (512 * BlockNum_HSMMC) / 4; i++)
		*(Compare_buffer_HSMMC + i) = 0x0;

	rt_kprintf("SD[%d] Polling mode data read1\n", psd->index);
	rt_kprintf("SD[%d] Read BlockNum = %d\n", psd->index, BlockNum_HSMMC);

	while (!check_card_status(psd));

	//  Maximum DMA Buffer Size, Block Size
	set_blksize_register(psd, 7, 512);
	//  Block Numbers to Write
	set_blkcnt_register(psd, BlockNum_HSMMC);

	if (movi_hc)
		set_arg_register(psd, StartAddr);
	else
		set_arg_register(psd, StartAddr * 512);

	if (BlockNum_HSMMC == 1)
	{
		rt_kprintf("SD[%d] Single block read\n", psd->index);
		set_transfer_mode_register(psd, 0, 1, 1, 1, 0);
		// MMC_READ_SINGLE_BLOCK
		set_cmd_register(psd, 17, 1, MMC_RSP_R1);
	}
	else
	{
		rt_kprintf("SD[%d] Multi block read\n", psd->index);
		set_transfer_mode_register(psd, 1, 1, 1, 1, 0);
		// MMC_READ_MULTIPLE_BLOCK
		set_cmd_register(psd, 18, 1, MMC_RSP_R1);
	}

	if (wait_for_cmd_done(psd))
		rt_kprintf("SD[%d] Command is NOT completed1\n", psd->index);
	clear_command_complete_status();

	for (j = 0; j < BlockNum_HSMMC; j++)
	{
		if (!wait_for_r_buf_ready(psd))
			rt_kprintf("SD[%d] ReadBuffer NOT Ready\n", psd->index);
		else
			clear_buffer_read_ready_status(psd);
		for (i = 0; i < 512 / 4; i++)
		{
			*Compare_buffer_HSMMC++ = s3c_readl(psd->regbase + oBDATA);
			CompareCnt_INT++;
		}
	}

	rt_kprintf("SD[%d] Read count=0x%08x\n", psd->index, CompareCnt_INT);
	if (!wait_for_data_done(psd))
		rt_kprintf("SD[%d] Transfer NOT Complete\n", psd->index);
	clear_transfer_complete_status(psd);

	rt_kprintf("SD[%d] HM_NORINTSTS = %x", psd->index, s3c_readw(psd->regbase + oNORINTSTS));
}

static void DataCompare_HSMMC(rt_card_t *psd, rt_uint32_t a0, rt_uint32_t a1, rt_uint32_t bytes)
{
	rt_uint32_t *pD0 = (rt_uint32_t *) a0;
	rt_uint32_t *pD1 = (rt_uint32_t *) a1;
	rt_uint32_t i, ErrCnt = 0;

	for (i = 0; i < bytes; i++)
	{
		if (*pD0 != *pD1)
		{
			rt_kprintf("\n%08x=%02x <-> %08x=%02x", pD0, *pD0, pD1, *pD1);
			ErrCnt++;
		}
		pD0++;
		pD1++;
	}
	rt_kprintf("\nTotal Error cnt = %d", ErrCnt);

	if (ErrCnt == 0)
		rt_kprintf("\nData Compare Ok\n");
}*/

int sdmmc_init(rt_card_t *psd)
{
	rt_uint32_t reg;

	clock_onoff(psd, 0);

	reg = s3c_readl(SCLK_GATE);
	s3c_writel(reg | (1<<27), SCLK_GATE);

	set_clock(psd, SD_CLKSRC_EPLL, 0x80);
	s3c_writeb(0xe, psd->regbase + oTIMEOUTCON);
	set_hostctl_speed(psd, SD_SPEED_NORMAL);

	interrupt_enable(psd, 0xff, 0xff);

	rt_kprintf("SD[%d] HM_NORINTSTS = %x\n", psd->index, s3c_readw(psd->regbase + oNORINTSTS));

	/* MMC_GO_IDLE_STATE */
	issue_command(psd, MMC_GO_IDLE_STATE, 0x00, 0, 0);

	psd->ocr_check = 1;
	if (set_mmc_ocr(psd))
	{
		psd->mmc_card = 1;
		rt_kprintf("SD[%d] MMC card is detected\n", psd->index);
	}
	else if (set_sd_ocr(psd))
	{
		psd->mmc_card = 0;
		rt_kprintf("SD[%d] SD card is detected\n", psd->index);
	}
	else
	{
		rt_kprintf("SD[%d] 0 MB\n", psd->index);
		return -1;
	}
	psd->ocr_check = 0;

	// Check the attached card and place the card
	// in the IDENT state rHM_RSPREG0
	issue_command(psd, MMC_ALL_SEND_CID, 0, 0, MMC_RSP_R2);

	// Manufacturer ID
	psd->card_mid = (s3c_readl(psd->regbase + oRSPREG3) >> 16) & 0xFF;

	rt_kprintf("Product Name : %c%c%c%c%c%c\n", ((s3c_readl(psd->regbase + oRSPREG2) >> 24) & 0xFF),
	       ((s3c_readl(psd->regbase + oRSPREG2) >> 16) & 0xFF), ((s3c_readl(psd->regbase + oRSPREG2) >> 8) & 0xFF), (s3c_readl(psd->regbase + oRSPREG2) & 0xFF),
	       ((s3c_readl(psd->regbase + oRSPREG1) >> 24) & 0xFF), ((s3c_readl(psd->regbase + oRSPREG1) >> 16) & 0xFF));

	// Send RCA(Relative Card Address). It places the card in the STBY state
	psd->rca = (psd->mmc_card) ? 0x0001 : 0x0000;
	issue_command(psd, MMC_SET_RELATIVE_ADDR, psd->rca<<16, 0, MMC_RSP_R1);

	if (!psd->mmc_card)
	{
		psd->rca = (s3c_readl(psd->regbase + oRSPREG0) >> 16) & 0xFFFF;
		//printf("=>  rca=0x%08x\n", rca);
	}

	rt_kprintf("SD[%d] Enter to the Stand-by State\n", psd->index);

	issue_command(psd, MMC_SEND_CSD, psd->rca<<16, 0, MMC_RSP_R2);

	if (psd->mmc_card)
	{
		psd->mmc_spec = (s3c_readl(psd->regbase + oRSPREG3) >> 18) & 0xF;
		rt_kprintf("SD[%d] mmc_spec=%d\n", psd->index, psd->mmc_spec);
	}

	issue_command(psd, MMC_SELECT_CARD, psd->rca<<16, 0, MMC_RSP_R1);
	rt_kprintf("SD[%d] Enter to the Transfer State\n", psd->index);

	display_card_info(psd);

	// Operating Clock setting
	// Divisor 1 = Base clk /2, Divisor 2 = Base clk /4, Divisor 4 = Base clk /8 ...
	clock_config(psd, SD_CLKSRC_EPLL, 2);

	while (set_bus_width(psd, psd->data_width));
	while (!check_card_status(psd));

	// MMC_SET_BLOCKLEN
	while (!issue_command(psd, MMC_SET_BLOCKLEN, 512, 0, MMC_RSP_R1));

	s3c_writew(0xffff, psd->regbase + oNORINTSTS);
	return 0;
}

void sdmmc_write(rt_card_t *psd, rt_uint32_t addr, rt_uint32_t start_blk, rt_uint32_t blknum)
{
	rt_uint16_t cmd, multi;

	psd->dma_end = 0;

	cmd = s3c_readw(psd->regbase + oNORINTSTSEN);
	//cmd &= ~0xffff;
	cmd = BUFFER_READREADY_STS_INT_EN | BUFFER_WRITEREADY_STS_INT_EN |
			TRANSFERCOMPLETE_STS_INT_EN | COMMANDCOMPLETE_STS_INT_EN;
	s3c_writew(cmd, psd->regbase + oNORINTSTSEN);

	cmd = s3c_readw(psd->regbase + oNORINTSIGEN);
	//cmd &= ~0xffff;
	cmd = TRANSFERCOMPLETE_SIG_INT_EN;
	s3c_writew(cmd, psd->regbase + oNORINTSIGEN);

	// AHB System Address For Write
	set_system_address_reg(psd, addr);
	// Maximum DMA Buffer Size, Block Size
	set_blksize_register(psd, 7, CARD_ONE_BLOCK_SIZE_VER1);
	// Block Numbers to Write
	set_blkcnt_register(psd, blknum);

	set_arg_register(psd, start_blk * CARD_ONE_BLOCK_SIZE_VER1);

	cmd = (blknum > 1) ? MMC_WRITE_MULTIPLE_BLOCK : MMC_WRITE_BLOCK;
	multi = (blknum > 1);

	set_transfer_mode_register(psd, multi, 0, 1, 1, 1);
	set_cmd_register(psd, cmd, 1, MMC_RSP_R1);

	if (wait_for_cmd_done(psd))
		rt_kprintf("SD[%d] Command is NOT completed3\n", psd->index);
	clear_command_complete_status(psd);

	// wait for DMA transfer
	check_dma_int(psd);
	while (!psd->dma_end);

	if (!wait_for_data_done(psd))
		rt_kprintf("SD[%d] Transfer is NOT Complete\n", psd->index);
	clear_transfer_complete_status(psd);

	s3c_writew(s3c_readw(psd->regbase + oNORINTSTS) | (1 << 3), psd->regbase + oNORINTSTS);
	psd->dma_end = 0;
}

void sdmmc_read(rt_card_t *psd, rt_uint32_t addr, rt_uint32_t start_blk, rt_uint32_t blknum)
{
	rt_uint16_t cmd, multi;

	psd->dma_end = 0;

	while (!check_card_status(psd));

	cmd = s3c_readw(psd->regbase + oNORINTSTSEN);
	cmd &= ~(DMA_STS_INT_EN | BLOCKGAP_EVENT_STS_INT_EN);
	s3c_writew(cmd,	psd->regbase + oNORINTSTSEN);

	cmd = s3c_readw(psd->regbase + oNORINTSIGEN);
	//cmd &= ~0xffff;
	cmd = TRANSFERCOMPLETE_SIG_INT_EN;
	s3c_writew(cmd, psd->regbase + oNORINTSIGEN);

	// AHB System Address For Write
	set_system_address_reg(psd, addr);
	// Maximum DMA Buffer Size, Block Size
	set_blksize_register(psd, 7, CARD_ONE_BLOCK_SIZE_VER1);
	// Block Numbers to Write
	set_blkcnt_register(psd, blknum);

	set_arg_register(psd, start_blk * CARD_ONE_BLOCK_SIZE_VER1);// Card Start Block Address to Write

	cmd = (blknum > 1) ? MMC_READ_MULTIPLE_BLOCK : MMC_READ_SINGLE_BLOCK;
	multi = (blknum > 1);

	set_transfer_mode_register(psd, multi, 1, multi, 1, 1);
	set_cmd_register(psd, cmd, 1, MMC_RSP_R1);

	if (wait_for_cmd_done(psd))
		rt_kprintf("SD[%d] Command is NOT completed\n", psd->index);
	else
		clear_command_complete_status(psd);

	check_dma_int(psd);
	while (!psd->dma_end);
	rt_kprintf("SD[%d] DMA Read End\n", psd->index);

	psd->dma_end = 0;
}


static rt_card_t sd0_card =
{
	.data_width = 4,
	.index = 0,
	.regbase = ELFIN_HSMMC_0_BASE,
};


void rt_hw_sdcard_init(void)
{
	rt_card_t *psd = &sd0_card;

	sd_sys_clk_init();

	sdmmc_set_gpio(psd);
	sdmmc_reset(psd);
	if (sdmmc_init(psd) != 0)
	{
		rt_kprintf("SD[%d] Card Initialization FAILED\n", psd->index);
		return;
	}
}


#if 0

#ifdef RT_USING_DFS
/* RT-Thread Device Driver Interface */
#include <dfs_fs.h>

struct rt_device sdcard_device[4];
struct dfs_partition part[4];

static rt_err_t rt_sdcard_init(rt_device_t dev)
{
	return RT_EOK;
}

static rt_err_t rt_sdcard_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t rt_sdcard_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_err_t rt_sdcard_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
	return RT_EOK;
}

static rt_size_t rt_sdcard_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	int i;
	struct dfs_partition *part = (struct dfs_partition *)dev->user_data;

	if ( dev == RT_NULL )
	{
		rt_set_errno(-DFS_STATUS_EINVAL);
		return 0;
	}

	/* read all sectors */
	for (i = 0; i < size; i ++)
	{
		rt_sem_take(part->lock, RT_WAITING_FOREVER);
		sd_readblock((part->offset + i + pos)*SECTOR_SIZE,
			(rt_uint8_t*)((rt_uint8_t*)buffer + i * SECTOR_SIZE));
		rt_sem_release(part->lock);
	}

	/* the length of reading must align to SECTOR SIZE */
	return size;
}

static rt_size_t rt_sdcard_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	int i;
	struct dfs_partition *part = (struct dfs_partition *)dev->user_data;

	if ( dev == RT_NULL )
	{
		rt_set_errno(-DFS_STATUS_EINVAL);
		return 0;
	}

	/* read all sectors */
	for (i = 0; i < size; i++)
	{
		rt_sem_take(part->lock, RT_WAITING_FOREVER);
		sd_writeblock((part->offset + i + pos)*SECTOR_SIZE,
			(rt_uint8_t*)((rt_uint8_t*)buffer + i * SECTOR_SIZE));
		rt_sem_release(part->lock);
	}

	/* the length of reading must align to SECTOR SIZE */
	return size;
}

void rt_hw_sdcard_init(void)
{
	rt_uint8_t i, status;
	rt_uint8_t *sector;
	char dname[4];
	char sname[8];

	/* Enable PCLK into SDI Block */
	CLKCON |= 1 << 9;

	/* Setup GPIO as SD and SDCMD, SDDAT[3:0] Pull up En */
	GPEUP  = GPEUP  & (~(0x3f << 5))   | (0x01 << 5);
	GPECON = GPECON & (~(0xfff << 10)) | (0xaaa << 10);

	RCA = 0;

	if (sd_init() == RT_EOK)
	{
		/* get the first sector to read partition table */
		sector = (rt_uint8_t*) rt_malloc (512);
		if (sector == RT_NULL)
		{
			rt_kprintf("allocate partition sector buffer failed\n");
			return;
		}
		status = sd_readblock(0, sector);
		if (status == RT_EOK)
		{
			for(i=0; i<4; i++)
			{
				/* get the first partition */
				status = dfs_filesystem_get_partition(&part[i], sector, i);
				if (status == RT_EOK)
				{
					rt_snprintf(dname, 4, "sd%d",  i);
					rt_snprintf(sname, 8, "sem_sd%d",  i);
					part[i].lock = rt_sem_create(sname, 1, RT_IPC_FLAG_FIFO);

					/* register sdcard device */
					sdcard_device[i].type  = RT_Device_Class_Block;
					sdcard_device[i].init = rt_sdcard_init;
					sdcard_device[i].open = rt_sdcard_open;
					sdcard_device[i].close = rt_sdcard_close;
					sdcard_device[i].read = rt_sdcard_read;
					sdcard_device[i].write = rt_sdcard_write;
					sdcard_device[i].control = rt_sdcard_control;
					sdcard_device[i].user_data = &part[i];

					rt_device_register(&sdcard_device[i], dname,
						RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_REMOVABLE | RT_DEVICE_FLAG_STANDALONE);
				}
				else
				{
					if(i == 0)
					{
						/* there is no partition table */
						part[0].offset = 0;
						part[0].size   = 0;
						part[0].lock = rt_sem_create("sem_sd0", 1, RT_IPC_FLAG_FIFO);

						/* register sdcard device */
						sdcard_device[0].type  = RT_Device_Class_Block;
						sdcard_device[0].init = rt_sdcard_init;
						sdcard_device[0].open = rt_sdcard_open;
						sdcard_device[0].close = rt_sdcard_close;
						sdcard_device[0].read = rt_sdcard_read;
						sdcard_device[0].write = rt_sdcard_write;
						sdcard_device[0].control = rt_sdcard_control;
						sdcard_device[0].user_data = &part[0];

						rt_device_register(&sdcard_device[0], "sd0",
							RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_REMOVABLE | RT_DEVICE_FLAG_STANDALONE);

						break;
					}
				}
			}
		}
		else
		{
			rt_kprintf("read sdcard first sector failed\n");
		}

		/* release sector buffer */
		rt_free(sector);

		return;
	}
	else
	{
		rt_kprintf("sdcard init failed\n");
	}
}

#endif /* end of RT_USING_DFS */
#endif

