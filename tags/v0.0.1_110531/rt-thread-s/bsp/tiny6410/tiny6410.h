/*
 * tiny6410.h
 *
 *  Created on: 2011-5-8
 *      Author: sangwei
 */
#ifndef __TINY6410_H__
#define __TINY6410_H__


#define MEMORY_BASE_ADDRESS			0x50000000

/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ			12000000	/* the Tiny6400 has 12MHz input clock */
/* the default serial baud-rate */
#define CONFIG_BAUDRATE				115200

/* CLK:532, HCLK:133, PCLK:66 */
#define CONFIG_CLK_532_133_66
#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)
#define APLL_MDIV					266
#define APLL_PDIV					3
#define APLL_SDIV					1
#define CONFIG_SYNC_MODE

#define APLL_VAL					set_pll(APLL_MDIV, APLL_PDIV, APLL_SDIV)
/* prevent overflow */
#define Startup_APLL				(CONFIG_SYS_CLK_FREQ/(APLL_PDIV<<APLL_SDIV)*APLL_MDIV)

#define MPLL_MDIV					266
#define MPLL_PDIV					3
#define MPLL_SDIV					1
#define MPLL_VAL					set_pll(MPLL_MDIV, MPLL_PDIV, MPLL_SDIV)
/* prevent overflow */
#define Startup_MPLL				((CONFIG_SYS_CLK_FREQ)/(MPLL_PDIV<<MPLL_SDIV)*MPLL_MDIV)

// Fout=84.67MHz
#define EPLL_MDIV					254
#define EPLL_PDIV					9
#define EPLL_SDIV					2
#define EPLL_KDIV					0
#define EPLL_VAL					set_pll(EPLL_MDIV, EPLL_PDIV, EPLL_SDIV)
#define EPLL_KVAL					EPLL_KDIV

#define Startup_APLLdiv				0
#define Startup_HCLKx2div			1
#define	Startup_PCLKdiv				3
#define Startup_HCLKdiv				1
#define Startup_MPLLdiv				1

#define CLK_DIV_VAL					((Startup_PCLKdiv<<12)|(Startup_HCLKx2div<<9)|(Startup_HCLKdiv<<8)\
									 |(Startup_MPLLdiv<<4)|Startup_APLLdiv)

#ifdef CONFIG_SYNC_MODE
#define Startup_HCLK				(Startup_APLL/(Startup_HCLKx2div+1)/(Startup_HCLKdiv+1))
#else /* !CONFIG_SYNC_MODE */
#define Startup_HCLK				(Startup_MPLL/(Startup_HCLKx2div+1)/(Startup_HCLKdiv+1))
#endif /* end of CONFIG_SYNC_MODE */


// FRIENDLYARM_BOOT_RAM256
#define DMC1_MEM_CFG				((1<<30) | (2<<15) | (3<<3) | (2<<0))
#define DMC1_CHIP0_CFG				0x150F0
#define PHYS_SDRAM_1_SIZE			0x10000000	/* 256 MB */

// Physical Memory Map
#define DMC1_MEM_CFG2				0xB41
#define DMC_DDR_32_CFG				0x0 		/* 32bit, DDR */

/* DDR Parameters */
#define DDR_tREFRESH				7800		/* ns */
#define DDR_tRAS					45			/* ns (min: 45ns)*/
#define DDR_tRC 					68			/* ns (min: 67.5ns)*/
#define DDR_tRCD					23			/* ns (min: 22.5ns)*/
#define DDR_tRFC					80			/* ns (min: 80ns)*/
#define DDR_tRP 					23			/* ns (min: 22.5ns)*/
#define DDR_tRRD					15			/* ns (min: 15ns)*/
#define DDR_tWR 					15			/* ns (min: 15ns)*/
#define DDR_tXSR					120			/* ns (min: 120ns)*/
#define DDR_CASL					3			/* CAS Latency 3 */

// mDDR memory configuration
#define DMC_DDR_BA_EMRS 			2
#define DMC_DDR_MEM_CASLAT			3
#define DMC_DDR_CAS_LATENCY			(DDR_CASL<<1)
#define DMC_DDR_t_DQSS				1
#define DMC_DDR_t_MRD				2
#define DMC_DDR_t_RAS				(((Startup_HCLK / 1000 * DDR_tRAS) - 1) / 1000000 + 1)	//7, Min 45ns
#define DMC_DDR_t_RC				(((Startup_HCLK / 1000 * DDR_tRC) - 1) / 1000000 + 1) 	//10, Min 67.5ns
#define DMC_DDR_t_RCD				(((Startup_HCLK / 1000 * DDR_tRCD) - 1) / 1000000 + 1) 	//4,5(TRM), Min 22.5ns
#define DMC_DDR_schedule_RCD		((DMC_DDR_t_RCD - 3) << 3)
#define DMC_DDR_t_RFC				(((Startup_HCLK / 1000 * DDR_tRFC) - 1) / 1000000 + 1) 	//11,18(TRM) Min 80ns
#define DMC_DDR_schedule_RFC		((DMC_DDR_t_RFC - 3) << 5)
#define DMC_DDR_t_RP				(((Startup_HCLK / 1000 * DDR_tRP) - 1) / 1000000 + 1) 	//4, 5(TRM) Min 22.5ns
#define DMC_DDR_schedule_RP			((DMC_DDR_t_RP - 3) << 3)
#define DMC_DDR_t_RRD				(((Startup_HCLK / 1000 * DDR_tRRD) - 1) / 1000000 + 1)	//3, Min 15ns
#define DMC_DDR_t_WR				(((Startup_HCLK / 1000 * DDR_tWR) - 1) / 1000000 + 1)	//Min 15ns
#define DMC_DDR_t_WTR				2
#define DMC_DDR_t_XP				2							//1tck + tIS(1.5ns)
#define DMC_DDR_t_XSR				(((Startup_HCLK / 1000 * DDR_tXSR) - 1) / 1000000 + 1)	//17, Min 120ns
#define DMC_DDR_t_ESR				DMC_DDR_t_XSR
#define DMC_DDR_REFRESH_PRD			(((Startup_HCLK / 1000 * DDR_tREFRESH) - 1) / 1000000) 	// TRM 2656
#define DMC_DDR_USER_CONFIG			1							// 2b01 : mDDR

#define CONFIG_NR_DRAM_BANKS		1	   /* we have 2 bank of DRAM */
#define PHYS_SDRAM_1				MEMORY_BASE_ADDRESS /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE			0x10000000	/* 256M */


#define CFG_FLASH_BASE				0x00000000

// FLASH and environment organization
#define CFG_MAX_FLASH_BANKS			0	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT			1024
#define CONFIG_AMD_LV800
#define PHYS_FLASH_SIZE				0x100000


/* NAND configuration */
#define CFG_MAX_NAND_DEVICE     	1
#define CFG_NAND_BASE           	(0x70200010)
#define NAND_MAX_CHIPS          	1

#define NAND_DISABLE_CE()			s3c_writel((s3c_readl(NFCONT) | (1 << 1)), NFCONT)
#define NAND_ENABLE_CE()			s3c_writel((s3c_readl(NFCONT) & ~(1 << 1)), NFCONT)
#define NF_TRANSRnB()				do { while(!(s3c_readl(NFCONT) & (1 << 0))); } while(0)

#define CFG_NAND_SKIP_BAD_DOT_I		1  /* ".i" read skips bad blocks   */
#define	CFG_NAND_WP					1
#define CFG_NAND_YAFFS_WRITE		1  /* support yaffs write */



#define BLOCK_SIZE					512
#define BL1_SIZE					(8*1024)
#define MANDATORY_SIZE				(BL1_SIZE + 512 + 512)
#define BL1_BLOCK_OFFSET			(MANDATORY_SIZE/BLOCK_SIZE)

#define BL2_SIZE					(1024*1024 - MANDATORY_SIZE)
#define BL2_BLOCK_OFFSET			((BL2_SIZE + MANDATORY_SIZE)/BLOCK_SIZE)

#define BL2_ADDR					0x00008000


#endif /* end of __TINY6410_H__ */
