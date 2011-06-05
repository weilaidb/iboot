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
#ifndef __SDCARD_H__
#define __SDCARD_H__


#define CARD_ONE_BLOCK_SIZE_VER1		512

#define MMC_RSP_PRESENT		(1 << 0)
#define MMC_RSP_136			(1 << 1)		// 136 bit response
#define MMC_RSP_CRC			(1 << 2)		// expect valid crc
#define MMC_RSP_BUSY		(1 << 3)		// card may send busy
#define MMC_RSP_OPCODE		(1 << 4)		// response contains opcode
#define MMC_CMD_MASK		(3 << 5)		// command type
#define MMC_CMD_AC			(0 << 5)
#define MMC_CMD_ADTC		(1 << 5)
#define MMC_CMD_BC			(2 << 5)
#define MMC_CMD_BCR			(3 << 5)

/*
 * These are the response types, and correspond to valid bit
 * patterns of the above flags.  One additional valid pattern
 * is all zeros, which means we don't expect a response.
 */
#define MMC_RSP_NONE		(0)
#define MMC_RSP_R1			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2			(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3			(MMC_RSP_PRESENT)
#define MMC_RSP_R6			(MMC_RSP_PRESENT|MMC_RSP_CRC)



#define	SD_CLKSRC_HCLK		1
#define	SD_CLKSRC_EPLL		2
#define	SD_CLKSRC_EXTCLK	3

#define	SD_SPEED_NORMAL		0
#define	SD_SPEED_HIGH		1

//Normal Interrupt Signal Enable
#define	READWAIT_SIG_INT_EN				(1<<10)
#define	CARD_SIG_INT_EN					(1<<8)
#define	CARD_REMOVAL_SIG_INT_EN			(1<<7)
#define	CARD_INSERT_SIG_INT_EN			(1<<6)
#define	BUFFER_READREADY_SIG_INT_EN		(1<<5)
#define	BUFFER_WRITEREADY_SIG_INT_EN	(1<<4)
#define	DMA_SIG_INT_EN					(1<<3)
#define	BLOCKGAP_EVENT_SIG_INT_EN		(1<<2)
#define	TRANSFERCOMPLETE_SIG_INT_EN		(1<<1)
#define	COMMANDCOMPLETE_SIG_INT_EN		(1<<0)

//Normal Interrupt Status Enable
#define	READWAIT_STS_INT_EN				(1<<10)
#define	CARD_STS_INT_EN					(1<<8)
#define	CARD_REMOVAL_STS_INT_EN			(1<<7)
#define	CARD_INSERT_STS_INT_EN			(1<<6)
#define	BUFFER_READREADY_STS_INT_EN		(1<<5)
#define	BUFFER_WRITEREADY_STS_INT_EN	(1<<4)
#define	DMA_STS_INT_EN					(1<<3)
#define	BLOCKGAP_EVENT_STS_INT_EN		(1<<2)
#define	TRANSFERCOMPLETE_STS_INT_EN		(1<<1)
#define	COMMANDCOMPLETE_STS_INT_EN		(1<<0)


#define SD_CTRL2_ENSTAASYNCCLR			(1 << 31)
#define SD_CTRL2_ENCMDCNFMSK			(1 << 30)
#define SD_CTRL2_CDINVRXD3				(1 << 29)
#define SD_CTRL2_SLCARDOUT				(1 << 28)

#define SD_CTRL2_FLTCLKSEL_MASK			(0xf << 24)
#define SD_CTRL2_FLTCLKSEL_SHIFT		(24)
#define SD_CTRL2_FLTCLKSEL(_x)			((_x) << 24)

#define SD_CTRL2_LVLDAT_MASK			(0xff << 16)
#define SD_CTRL2_LVLDAT_SHIFT			(16)
#define SD_CTRL2_LVLDAT(_x)				((_x) << 16)

#define SD_CTRL2_ENFBCLKTX				(1 << 15)
#define SD_CTRL2_ENFBCLKRX				(1 << 14)
#define SD_CTRL2_SDCDSEL				(1 << 13)
#define SD_CTRL2_SDSIGPC				(1 << 12)
#define SD_CTRL2_ENBUSYCHKTXSTART		(1 << 11)

#define SD_CTRL2_DFCNT_MASK				(0x3 << 9)
#define SD_CTRL2_DFCNT_SHIFT			(9)
#define SD_CTRL2_DFCNT_NONE				(0x0 << 9)
#define SD_CTRL2_DFCNT_4SDCLK			(0x1 << 9)
#define SD_CTRL2_DFCNT_16SDCLK			(0x2 << 9)
#define SD_CTRL2_DFCNT_64SDCLK			(0x3 << 9)

#define SD_CTRL2_ENCLKOUTHOLD			(1 << 8)
#define SD_CTRL2_RWAITMODE				(1 << 7)
#define SD_CTRL2_DISBUFRD				(1 << 6)
#define SD_CTRL2_SELBASECLK_MASK		(0x3 << 4)
#define SD_CTRL2_SELBASECLK_SHIFT		(4)
#define SD_CTRL2_PWRSYNC				(1 << 3)
#define SD_CTRL2_ENCLKOUTMSKCON			(1 << 1)
#define SD_CTRL2_HWINITFIN				(1 << 0)

#define SD_CTRL3_FCSEL3					(1 << 31)
#define SD_CTRL3_FCSEL2					(1 << 23)
#define SD_CTRL3_FCSEL1					(1 << 15)
#define SD_CTRL3_FCSEL0					(1 << 7)

#define SD_CTRL3_FIA3_MASK				(0x7f << 24)
#define SD_CTRL3_FIA3_SHIFT				(24)
#define SD_CTRL3_FIA3(_x)				((_x) << 24)

#define SD_CTRL3_FIA2_MASK				(0x7f << 16)
#define SD_CTRL3_FIA2_SHIFT				(16)
#define SD_CTRL3_FIA2(_x)				((_x) << 16)

#define SD_CTRL3_FIA1_MASK				(0x7f << 8)
#define SD_CTRL3_FIA1_SHIFT				(8)
#define SD_CTRL3_FIA1(_x)				((_x) << 8)

#define SD_CTRL3_FIA0_MASK				(0x7f << 0)
#define SD_CTRL3_FIA0_SHIFT				(0)
#define SD_CTRL3_FIA0(_x)				((_x) << 0)

#define SD_CONTROL4_DRIVE_MASK			(0x3 << 16)
#define SD_CONTROL4_DRIVE_SHIFT			(16)
#define SD_CONTROL4_DRIVE_2mA			(0x0 << 16)
#define SD_CONTROL4_DRIVE_4mA			(0x1 << 16)
#define SD_CONTROL4_DRIVE_7mA			(0x2 << 16)
#define SD_CONTROL4_DRIVE_9mA			(0x3 << 16)

#define SD_CONTROL4_BUSY				(1)


#define SDCLK			20000000
#define MMCCLK			15000000


typedef struct _rt_crad
{
	const rt_uint8_t data_width;
	rt_uint8_t mmc_card;
	rt_uint8_t dma_end;
	rt_uint8_t mmc_spec;
	rt_uint8_t sd_spec;
	rt_uint8_t ocr_check;
	rt_uint8_t card_mid;

	rt_uint16_t rca;

	int index;
	int present;

	rt_uint32_t regbase;
	rt_uint32_t clock;
} rt_card_t;


#endif /* end of __SDCARD_H__ */
