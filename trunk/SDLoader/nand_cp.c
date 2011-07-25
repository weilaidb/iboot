/***************************************************************************
*   Copyright (C) 2011 by swkyer <swkyer@gmail.com>                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "s3c6410.h"
#include "tiny6410.h"


/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0			0
#define NAND_CMD_READ1			1
#define NAND_CMD_RNDOUT			5
#define NAND_CMD_PAGEPROG		0x10
#define NAND_CMD_READOOB		0x50
#define NAND_CMD_ERASE1			0x60
#define NAND_CMD_STATUS			0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN			0x80
#define NAND_CMD_RNDIN			0x85
#define NAND_CMD_READID			0x90
#define NAND_CMD_ERASE2			0xd0
#define NAND_CMD_RESET			0xff

/* Extended commands for large page devices */
#define NAND_CMD_READSTART		0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG		0x15

/* Extended commands for AG-AND device */
/*
 * Note: the command for NAND_CMD_DEPLETE1 is really 0x00 but
 *       there is no way to distinguish that from NAND_CMD_READ0
 *       until the remaining sequence of commands has been completed
 *       so add a high order bit and mask it off in the command.
 */
#define NAND_CMD_DEPLETE1		0x100
#define NAND_CMD_DEPLETE2		0x38
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_STATUS_ERROR	0x72
/* multi-bank error status (banks 0-3) */
#define NAND_CMD_STATUS_ERROR0	0x73
#define NAND_CMD_STATUS_ERROR1	0x74
#define NAND_CMD_STATUS_ERROR2	0x75
#define NAND_CMD_STATUS_ERROR3	0x76
#define NAND_CMD_STATUS_RESET	0x7f
#define NAND_CMD_STATUS_CLEAR	0xff

#define NAND_CMD_NONE			-1

/* Status bits */
#define NAND_STATUS_FAIL		0x01
#define NAND_STATUS_FAIL_N1		0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY		0x40
#define NAND_STATUS_WP			0x80


/*
 * address format
 *              17 16         9 8            0
 * --------------------------------------------
 * | block(12bit) | page(5bit) | offset(9bit) |
 * --------------------------------------------
 */
static int nandll_read_page(unsigned char *buf, unsigned long addr, int large_block)
{
	int i;
	int page_size = 512;

	if (large_block)
		page_size = 2048;

	NAND_ENABLE_CE();
	s3c_writel(NAND_CMD_READ0, NFCMMD);
	/* Write Address */
	s3c_writel(0, NFADDR);

	if (large_block)
		s3c_writel(0, NFADDR);

	s3c_writel((addr) & 0xff, NFADDR);
	s3c_writel((addr >> 8) & 0xff, NFADDR);
	s3c_writel((addr >> 16) & 0xff, NFADDR);

	if (large_block)
		s3c_writel(NAND_CMD_READSTART, NFCMMD);

	NF_TRANSRnB();

	/* for compatibility(2460). u32 cannot be used. by scsuh */
	for (i=0; i < page_size; i++)
		*buf++ = s3c_readl(NFDATA) & 0xff;

	NAND_DISABLE_CE();
	return 0;
}

/*
 * Read data from NAND.
 */
static int nandll_read_blocks (unsigned long dst_addr, unsigned long size, int large_block)
{
	int i;
	unsigned char *buf = (unsigned char *)dst_addr;
	unsigned int page_shift = 9;

	if (large_block)
		page_shift = 11;

	/* Read pages */
	for (i = 0; i < (0x3c000>>page_shift); i++, buf+=(1<<page_shift))
		nandll_read_page(buf, i, large_block);

	return 0;
}

int copy_bl2_to_ram(void)
{
	int i, large_block = 0;
	unsigned char id;

	NAND_ENABLE_CE();
	s3c_writel(NAND_CMD_READID, NFCMMD);
	s3c_writel(0x00, NFADDR);

	/* wait for a while */
	for (i=0; i<200; i++);
	id = s3c_readl(NFDATA);
	id = s3c_readl(NFDATA);

	if (id > 0x80)
		large_block = 1;

	/* read NAND Block.
	 * 128KB ->240KB because of U-Boot size increase. by scsuh
	 * So, read 0x3c000 bytes not 0x20000(128KB).
	 */
	return nandll_read_blocks(BL2_ADDR, 0x3c000, large_block);
}

