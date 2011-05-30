/*
 * s3c6410.h
 *
 *  Created on: 2011-5-11
 *      Author: sangwei
 */
#ifndef __S3C6410_H__
#define __S3C6410_H__

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __ASSEMBLY__
#define Fld(Size, Shft)			(((Size) << 16) + (Shft))

#define FSize(Field)			((Field) >> 16)
#define FShft(Field)			((Field) & 0x0000FFFF)
#define FMsk(Field)				(((UData (1) << FSize(Field)) - 1) << FShft (Field))
#define FAlnMsk(Field)			((UData (1) << FSize(Field)) - 1)
#define F1stBit(Field)			(UData (1) << FShft(Field))

#define FClrBit(Data, Bit)		(Data = (Data & ~(Bit)))
#define FClrFld(Data, Field)	(Data = (Data & ~FMsk(Field)))

#define FInsrt(Value, Field) 	(UData(Value) << FShft(Field))
#define FExtr(Data, Field) 		((UData(Data) >> FShft(Field)) & FAlnMsk(Field))


typedef unsigned char rt_uint8_t;
typedef unsigned short rt_uint16_t;
typedef unsigned int rt_uint32_t;

static inline unsigned char s3c_readb(unsigned int addr)
{
	return *(volatile unsigned char *)(addr);
}
static inline unsigned short s3c_readw(unsigned int addr)
{
	return *(volatile unsigned short *)(addr);
}
static inline unsigned int s3c_readl(unsigned int addr)
{
	return *(volatile unsigned int *)(addr);
}
static inline void s3c_writeb(unsigned char bval, unsigned int addr)
{
	*(volatile unsigned char *)(addr) = bval;
}
static inline void s3c_writew(unsigned short wval, unsigned int addr)
{
	*(volatile unsigned short *)(addr) = wval;
}
static inline void s3c_writel(unsigned int lval, unsigned int addr)
{
	*(volatile unsigned int *)(addr) = lval;
}
#endif /* end of __ASSEMBLY__ */


#define S3C64XX_UART_CHANNELS	3
#define S3C64XX_SPI_CHANNELS	2

#define BIT(X)					(1<<(X))

#define ROM_BASE0				0x00000000      /* base address of rom bank 0 */
#define ROM_BASE1				0x04000000      /* base address of rom bank 1 */
#define DRAM_BASE0				0x40000000      /* base address of dram bank 0 */
#define DRAM_BASE1				0x50000000      /* base address of dram bank 1 */


/* S3C6400 device base addresses */
#define ELFIN_DMA_BASE			0x75000000
#define ELFIN_LCD_BASE			0x77100000
#define ELFIN_USB_HOST_BASE		0x74300000
#define ELFIN_I2C_BASE			0x7f004000
#define ELFIN_I2S_BASE			0x7f002000
#define ELFIN_ADC_BASE			0x7e00b000
#define ELFIN_SPI_BASE			0x7f00b000
#define ELFIN_HSMMC_0_BASE		0x7c200000
#define ELFIN_HSMMC_1_BASE		0x7c300000
#define ELFIN_HSMMC_2_BASE		0x7c400000


/* Clock & Power Controller for mDirac3*/
#define ELFIN_CLOCK_POWER_BASE	0x7e00f000

#define oAPLL_LOCK				0x00
#define oMPLL_LOCK				0x04
#define oEPLL_LOCK				0x08
#define oAPLL_CON				0x0C
#define oMPLL_CON				0x10
#define oEPLL_CON0				0x14
#define oEPLL_CON1				0x18
#define oCLK_SRC				0x1C
#define oCLK_DIV0				0x20
#define oCLK_DIV1				0x24
#define oCLK_DIV2				0x28
#define oCLK_OUT				0x2C
#define oHCLK_GATE				0x30
#define oPCLK_GATE				0x34
#define oSCLK_GATE				0x38
#define oAHB_CON0				0x100
#define oAHB_CON1				0x104
#define oAHB_CON2				0x108
#define oSELECT_DMA				0x110
#define oSW_RST					0x114
#define oSYS_ID					0x118
#define oMEM_SYS_CFG			0x120
#define oQOS_OVERRIDE0			0x124
#define oQOS_OVERRIDE1			0x128
#define oMEM_CFG_STAT			0x12C
#define oPWR_CFG				0x804
#define oEINT_MASK				0x808
#define oNOR_CFG				0x810
#define oSTOP_CFG				0x814
#define oSLEEP_CFG				0x818
#define oOSC_FREQ				0x820
#define oOSC_STABLE				0x824
#define oPWR_STABLE				0x828
#define oFPC_STABLE				0x82C
#define oMTC_STABLE				0x830
#define oOTHERS					0x900
#define oRST_STAT				0x904
#define oWAKEUP_STAT			0x908
#define oBLK_PWR_STAT			0x90C
#define oINF_REG0				0xA00
#define oINF_REG1				0xA04
#define oINF_REG2				0xA08
#define oINF_REG3				0xA0C
#define oINF_REG4				0xA10
#define oINF_REG5				0xA14
#define oINF_REG6				0xA18
#define oINF_REG7				0xA1C

#define oOSC_CNT_VAL			0x824
#define oPWR_CNT_VAL			0x828
#define oFPC_CNT_VAL			0x82C
#define oMTC_CNT_VAL			0x830

#define APLL_LOCK				(ELFIN_CLOCK_POWER_BASE + oAPLL_LOCK)
#define MPLL_LOCK				(ELFIN_CLOCK_POWER_BASE + oMPLL_LOCK)
#define EPLL_LOCK				(ELFIN_CLOCK_POWER_BASE + oEPLL_LOCK)
#define APLL_CON				(ELFIN_CLOCK_POWER_BASE + oAPLL_CON)
#define MPLL_CON				(ELFIN_CLOCK_POWER_BASE + oMPLL_CON)
#define EPLL_CON0				(ELFIN_CLOCK_POWER_BASE + oEPLL_CON0)
#define EPLL_CON1				(ELFIN_CLOCK_POWER_BASE + oEPLL_CON1)
#define CLK_SRC					(ELFIN_CLOCK_POWER_BASE + oCLK_SRC)
#define CLK_DIV0				(ELFIN_CLOCK_POWER_BASE + oCLK_DIV0)
#define CLK_DIV1				(ELFIN_CLOCK_POWER_BASE + oCLK_DIV1)
#define CLK_DIV2				(ELFIN_CLOCK_POWER_BASE + oCLK_DIV2)
#define CLK_OUT					(ELFIN_CLOCK_POWER_BASE + oCLK_OUT)
#define HCLK_GATE				(ELFIN_CLOCK_POWER_BASE + oHCLK_GATE)
#define PCLK_GATE				(ELFIN_CLOCK_POWER_BASE + oPCLK_GATE)
#define SCLK_GATE				(ELFIN_CLOCK_POWER_BASE + oSCLK_GATE)
#define AHB_CON0				(ELFIN_CLOCK_POWER_BASE + oAHB_CON0)
#define AHB_CON1				(ELFIN_CLOCK_POWER_BASE + oAHB_CON1)
#define AHB_CON2				(ELFIN_CLOCK_POWER_BASE + oAHB_CON2)
#define SELECT_DMA				(ELFIN_CLOCK_POWER_BASE + oSELECT_DMA)
#define SW_RST					(ELFIN_CLOCK_POWER_BASE + oSW_RST)
#define SYS_ID					(ELFIN_CLOCK_POWER_BASE + oSYS_ID)
#define MEM_SYS_CFG				(ELFIN_CLOCK_POWER_BASE + oMEM_SYS_CFG)
#define QOS_OVERRIDE0			(ELFIN_CLOCK_POWER_BASE + oQOS_OVERRIDE0)
#define QOS_OVERRIDE1			(ELFIN_CLOCK_POWER_BASE + oQOS_OVERRIDE1)
#define MEM_CFG_STAT			(ELFIN_CLOCK_POWER_BASE + oMEM_CFG_STAT)
#define PWR_CFG					(ELFIN_CLOCK_POWER_BASE + oPWR_CFG)
#define EINT_MASK				(ELFIN_CLOCK_POWER_BASE + oEINT_MASK)
#define NOR_CFG					(ELFIN_CLOCK_POWER_BASE + oNOR_CFG)
#define STOP_CFG				(ELFIN_CLOCK_POWER_BASE + oSTOP_CFG)
#define SLEEP_CFG				(ELFIN_CLOCK_POWER_BASE + oSLEEP_CFG)
#define OSC_FREQ				(ELFIN_CLOCK_POWER_BASE + oOSC_FREQ)
#define OSC_CNT_VAL				(ELFIN_CLOCK_POWER_BASE + oOSC_CNT_VAL)
#define PWR_CNT_VAL				(ELFIN_CLOCK_POWER_BASE + oPWR_CNT_VAL)
#define FPC_CNT_VAL				(ELFIN_CLOCK_POWER_BASE + oFPC_CNT_VAL)
#define MTC_CNT_VAL				(ELFIN_CLOCK_POWER_BASE + oMTC_CNT_VAL)
#define OTHERS					(ELFIN_CLOCK_POWER_BASE + oOTHERS)
#define RST_STAT				(ELFIN_CLOCK_POWER_BASE + oRST_STAT)
#define WAKEUP_STAT				(ELFIN_CLOCK_POWER_BASE + oWAKEUP_STAT)
#define BLK_PWR_STAT			(ELFIN_CLOCK_POWER_BASE + oBLK_PWR_STAT)
#define INF_REG0				(ELFIN_CLOCK_POWER_BASE + oINF_REG0)
#define INF_REG1				(ELFIN_CLOCK_POWER_BASE + oINF_REG1)
#define INF_REG2				(ELFIN_CLOCK_POWER_BASE + oINF_REG2)
#define INF_REG3				(ELFIN_CLOCK_POWER_BASE + oINF_REG3)
#define INF_REG4				(ELFIN_CLOCK_POWER_BASE + oINF_REG4)
#define INF_REG5				(ELFIN_CLOCK_POWER_BASE + oINF_REG5)
#define INF_REG6				(ELFIN_CLOCK_POWER_BASE + oINF_REG6)
#define INF_REG7				(ELFIN_CLOCK_POWER_BASE + oINF_REG7)


/*
 * GPIO
 */
#define ELFIN_GPIO_BASE			0x7f008000

#define oGPACON					0x00
#define oGPADAT					0x04
#define oGPAPUD					0x08
#define oGPACONSLP				0x0C
#define oGPAPUDSLP				0x10
#define oGPBCON					0x20
#define oGPBDAT					0x24
#define oGPBPUD					0x28
#define oGPBCONSLP				0x2C
#define oGPBPUDSLP				0x30
#define oGPCCON					0x40
#define oGPCDAT					0x44
#define oGPCPUD					0x48
#define oGPCCONSLP				0x4C
#define oGPCPUDSLP				0x50
#define oGPDCON					0x60
#define oGPDDAT					0x64
#define oGPDPUD					0x68
#define oGPDCONSLP				0x6C
#define oGPDPUDSLP				0x70
#define oGPECON					0x80
#define oGPEDAT					0x84
#define oGPEPUD					0x88
#define oGPECONSLP				0x8C
#define oGPEPUDSLP				0x90
#define oGPFCON					0xA0
#define oGPFDAT					0xA4
#define oGPFPUD					0xA8
#define oGPFCONSLP				0xAC
#define oGPFPUDSLP				0xB0
#define oGPGCON					0xC0
#define oGPGDAT					0xC4
#define oGPGPUD					0xC8
#define oGPGCONSLP				0xCC
#define oGPGPUDSLP				0xD0
#define oGPHCON0				0xE0
#define oGPHCON1				0xE4
#define oGPHDAT					0xE8
#define oGPHPUD					0xEC
#define oGPHCONSLP				0xF0
#define oGPHPUDSLP				0xF4
#define oGPICON					0x100
#define oGPIDAT					0x104
#define oGPIPUD					0x108
#define oGPICONSLP				0x10C
#define oGPIPUDSLP				0x110
#define oGPJCON					0x120
#define oGPJDAT					0x124
#define oGPJPUD					0x128
#define oGPJCONSLP				0x12C
#define oGPJPUDSLP				0x130
#define oSPCON					0x1A0
#define oMEM0DRVCON				0x1D0
#define oMEM1DRVCON				0x1D4
#define oGPKCON0				0x800
#define oGPKCON1				0x804
#define oGPKDAT					0x808
#define oGPKPUD					0x80C
#define oGPLCON0				0x810
#define oGPLCON1				0x814
#define oGPLDAT					0x818
#define oGPLPUD					0x81C
#define oGPMCON					0x820
#define oGPMDAT					0x824
#define oGPMPUD					0x828
#define oGPNCON					0x830
#define oGPNDAT					0x834
#define oGPNPUD					0x838
#define oGPOCON					0x140
#define oGPODAT					0x144
#define oGPOPUD					0x148
#define oGPOCONSLP				0x14C
#define oGPOPUDSLP				0x150
#define oGPPCON					0x160
#define oGPPDAT					0x164
#define oGPPPUD					0x168
#define oGPPCONSLP				0x16C
#define oGPPPUDSLP				0x170
#define oGPQCON					0x180
#define oGPQDAT					0x184
#define oGPQPUD					0x188
#define oGPQCONSLP				0x18C
#define oGPQPUDSLP				0x190
#define oEINTPEND				0x924

#define GPACON					(ELFIN_GPIO_BASE + oGPACON)
#define GPADAT					(ELFIN_GPIO_BASE + oGPADAT)
#define GPAPUD					(ELFIN_GPIO_BASE + oGPAPUD)
#define GPACONSLP				(ELFIN_GPIO_BASE + oGPACONSLP)
#define GPAPUDSLP				(ELFIN_GPIO_BASE + oGPAPUDSLP)
#define GPBCON					(ELFIN_GPIO_BASE + oGPBCON)
#define GPBDAT					(ELFIN_GPIO_BASE + oGPBDAT)
#define GPBPUD					(ELFIN_GPIO_BASE + oGPBPUD)
#define GPBCONSLP				(ELFIN_GPIO_BASE + oGPBCONSLP)
#define GPBPUDSLP				(ELFIN_GPIO_BASE + oGPBPUDSLP)
#define GPCCON					(ELFIN_GPIO_BASE + oGPCCON)
#define GPCDAT					(ELFIN_GPIO_BASE + oGPCDAT)
#define GPCPUD					(ELFIN_GPIO_BASE + oGPCPUD)
#define GPCCONSLP				(ELFIN_GPIO_BASE + oGPCCONSLP)
#define GPCPUDSLP				(ELFIN_GPIO_BASE + oGPCPUDSLP)
#define GPDCON					(ELFIN_GPIO_BASE + oGPDCON)
#define GPDDAT					(ELFIN_GPIO_BASE + oGPDDAT)
#define GPDPUD					(ELFIN_GPIO_BASE + oGPDPUD)
#define GPDCONSLP				(ELFIN_GPIO_BASE + oGPDCONSLP)
#define GPDPUDSLP				(ELFIN_GPIO_BASE + oGPDPUDSLP)
#define GPECON					(ELFIN_GPIO_BASE + oGPECON)
#define GPEDAT					(ELFIN_GPIO_BASE + oGPEDAT)
#define GPEPUD					(ELFIN_GPIO_BASE + oGPEPUD)
#define GPECONSLP				(ELFIN_GPIO_BASE + oGPECONSLP)
#define GPEPUDSLP				(ELFIN_GPIO_BASE + oGPEPUDSLP)
#define GPFCON					(ELFIN_GPIO_BASE + oGPFCON)
#define GPFDAT					(ELFIN_GPIO_BASE + oGPFDAT)
#define GPFPUD					(ELFIN_GPIO_BASE + oGPFPUD)
#define GPFCONSLP				(ELFIN_GPIO_BASE + oGPFCONSLP)
#define GPFPUDSLP				(ELFIN_GPIO_BASE + oGPFPUDSLP)
#define GPGCON					(ELFIN_GPIO_BASE + oGPGCON)
#define GPGDAT					(ELFIN_GPIO_BASE + oGPGDAT)
#define GPGPUD					(ELFIN_GPIO_BASE + oGPGPUD)
#define GPGCONSLP				(ELFIN_GPIO_BASE + oGPGCONSLP)
#define GPGPUDSLP				(ELFIN_GPIO_BASE + oGPGPUDSLP)
#define GPHCON0					(ELFIN_GPIO_BASE + oGPHCON0)
#define GPHCON1					(ELFIN_GPIO_BASE + oGPHCON1)
#define GPHDAT					(ELFIN_GPIO_BASE + oGPHDAT)
#define GPHPUD					(ELFIN_GPIO_BASE + oGPHPUD)
#define GPHCONSLP				(ELFIN_GPIO_BASE + oGPHCONSLP)
#define GPHPUDSLP				(ELFIN_GPIO_BASE + oGPHPUDSLP)
#define GPICON					(ELFIN_GPIO_BASE + oGPICON)
#define GPIDAT					(ELFIN_GPIO_BASE + oGPIDAT)
#define GPIPUD					(ELFIN_GPIO_BASE + oGPIPUD)
#define GPICONSLP				(ELFIN_GPIO_BASE + oGPICONSLP)
#define GPIPUDSLP				(ELFIN_GPIO_BASE + oGPIPUDSLP)
#define GPJCON					(ELFIN_GPIO_BASE + oGPJCON)
#define GPJDAT					(ELFIN_GPIO_BASE + oGPJDAT)
#define GPJPUD					(ELFIN_GPIO_BASE + oGPJPUD)
#define GPJCONSLP				(ELFIN_GPIO_BASE + oGPJCONSLP)
#define GPJPUDSLP				(ELFIN_GPIO_BASE + oGPJPUDSLP)
#define GPKCON0					(ELFIN_GPIO_BASE + oGPKCON0)
#define GPKCON1					(ELFIN_GPIO_BASE + oGPKCON1)
#define GPKDAT					(ELFIN_GPIO_BASE + oGPKDAT)
#define GPKPUD					(ELFIN_GPIO_BASE + oGPKPUD)
#define GPLCON0					(ELFIN_GPIO_BASE + oGPLCON0)
#define GPLCON1					(ELFIN_GPIO_BASE + oGPLCON1)
#define GPLDAT					(ELFIN_GPIO_BASE + oGPLDAT)
#define GPLPUD					(ELFIN_GPIO_BASE + oGPLPUD)
#define GPMCON					(ELFIN_GPIO_BASE + oGPMCON)
#define GPMDAT					(ELFIN_GPIO_BASE + oGPMDAT)
#define GPMPUD					(ELFIN_GPIO_BASE + oGPMPUD)
#define GPNCON					(ELFIN_GPIO_BASE + oGPNCON)
#define GPNDAT					(ELFIN_GPIO_BASE + oGPNDAT)
#define GPNPUD					(ELFIN_GPIO_BASE + oGPNPUD)
#define GPOCON					(ELFIN_GPIO_BASE + oGPOCON)
#define GPODAT					(ELFIN_GPIO_BASE + oGPODAT)
#define GPOPUD					(ELFIN_GPIO_BASE + oGPODAT)
#define GPOCONSLP				(ELFIN_GPIO_BASE + oGPOCONSLP)
#define GPOPUDSLP				(ELFIN_GPIO_BASE + oGPOPUDSLP)
#define GPPCON					(ELFIN_GPIO_BASE + oGPPCON)
#define GPPDAT					(ELFIN_GPIO_BASE + oGPPDAT)
#define GPPPUD					(ELFIN_GPIO_BASE + oGPPPUD)
#define GPPCONSLP				(ELFIN_GPIO_BASE + oGPPCONSLP)
#define GPPPUDSLP				(ELFIN_GPIO_BASE + oGPPPUDSLP)
#define GPQCON					(ELFIN_GPIO_BASE + oGPQCON)
#define GPQDAT					(ELFIN_GPIO_BASE + oGPQDAT)
#define GPQPUD					(ELFIN_GPIO_BASE + oGPQPUD)
#define GPQCONSLP				(ELFIN_GPIO_BASE + oGPQCONSLP)
#define GPQPUDSLP				(ELFIN_GPIO_BASE + oGPQPUDSLP)
#define SPCON					(ELFIN_GPIO_BASE + oSPCON)
#define MEM0DRVCON				(ELFIN_GPIO_BASE + oMEM0DRVCON)
#define MEM1DRVCON				(ELFIN_GPIO_BASE + oMEM1DRVCON)

/*
 * Bus Matrix
 */
#define ELFIN_MEM_SYS_CFG		0x7e00f120


/*
 * Memory controller
 */
#define ELFIN_SROM_BASE			0x70000000

#define SROM_BW					(ELFIN_SROM_BASE + 0x00)
#define SROM_BC0				(ELFIN_SROM_BASE + 0x04)
#define SROM_BC1				(ELFIN_SROM_BASE + 0x08)
#define SROM_BC2				(ELFIN_SROM_BASE + 0x0C)
#define SROM_BC3				(ELFIN_SROM_BASE + 0x10)
#define SROM_BC4				(ELFIN_SROM_BASE + 0x14)
#define SROM_BC5				(ELFIN_SROM_BASE + 0x18)


/*
 * SDRAM Controller
 */
#define ELFIN_DMC0_BASE			0x7e000000
#define ELFIN_DMC1_BASE			0x7e001000

#define INDEX_DMC_MEMC_STATUS   (0x00)
#define INDEX_DMC_MEMC_CMD      (0x04)
#define INDEX_DMC_DIRECT_CMD    (0x08)
#define INDEX_DMC_MEMORY_CFG    (0x0C)
#define INDEX_DMC_REFRESH_PRD   (0x10)
#define INDEX_DMC_CAS_LATENCY   (0x14)
#define INDEX_DMC_T_DQSS        (0x18)
#define INDEX_DMC_T_MRD         (0x1C)
#define INDEX_DMC_T_RAS         (0x20)
#define INDEX_DMC_T_RC          (0x24)
#define INDEX_DMC_T_RCD         (0x28)
#define INDEX_DMC_T_RFC         (0x2C)
#define INDEX_DMC_T_RP          (0x30)
#define INDEX_DMC_T_RRD         (0x34)
#define INDEX_DMC_T_WR          (0x38)
#define INDEX_DMC_T_WTR         (0x3C)
#define INDEX_DMC_T_XP          (0x40)
#define INDEX_DMC_T_XSR         (0x44)
#define INDEX_DMC_T_ESR         (0x48)
#define INDEX_DMC_MEMORY_CFG2	(0x4C)
#define INDEX_DMC_CHIP_0_CFG    (0x200)
#define INDEX_DMC_CHIP_1_CFG    (0x204)
#define INDEX_DMC_CHIP_2_CFG    (0x208)
#define INDEX_DMC_CHIP_3_CFG    (0x20C)
#define INDEX_DMC_USER_STATUS	(0x300)
#define INDEX_DMC_USER_CONFIG	(0x304)

/*
* Memory Chip direct command
*/
#define DMC_NOP0 				0x0c0000
#define DMC_NOP1				0x1c0000
#define DMC_PA0 				0x000000	//Precharge all
#define DMC_PA1 				0x100000
#define DMC_AR0 				0x040000	//Autorefresh
#define DMC_AR1 				0x140000
#define DMC_SDR_MR0				0x080032	//MRS, CAS 3,  Burst Length 4
#define DMC_SDR_MR1				0x180032
#define DMC_DDR_MR0				0x080162
#define DMC_DDR_MR1				0x180162
#define DMC_mDDR_MR0			0x080032	//CAS 3, Burst Length 4
#define DMC_mDDR_MR1			0x180032
#define DMC_mSDR_EMR0			0x0a0000	//EMRS, DS:Full, PASR:Full Array
#define DMC_mSDR_EMR1			0x1a0000
#define DMC_DDR_EMR0			0x090000
#define DMC_DDR_EMR1			0x190000
#define DMC_mDDR_EMR0			0x0a0000	// DS:Full, PASR:Full Array
#define DMC_mDDR_EMR1			0x1a0000


/****************************************************************
 Definitions for memory configuration
 Set memory configuration
	active_chips	 = 1'b0 (1 chip)
	qos_master_chip  = 3'b000(ARID[3:0])
	memory burst	 = 3'b010(burst 4)
	stop_mem_clock	 = 1'b0(disable dynamical stop)
	auto_power_down  = 1'b0(disable auto power-down mode)
	power_down_prd	 = 6'b00_0000(0 cycle for auto power-down)
	ap_bit		 = 1'b0 (bit position of auto-precharge is 10)
	row_bits	 = 3'b010(# row address 13)
	column_bits	 = 3'b010(# column address 10 )

 Set user configuration
	2'b10=SDRAM/mSDRAM, 2'b11=DDR, 2'b01=mDDR

 Set chip select for chip [n]
	 row bank control, bank address 0x3000_0000 ~ 0x37ff_ffff
	 CHIP_[n]_CFG=0x30F8,  30: ADDR[31:24], F8: Mask[31:24]
******************************************************************/

/*
 * HS MMC Interface
 */
#define ELFIN_HSMMC_BASE		0x7C200000

#define HM_SYSAD				(0x00)
#define HM_BLKSIZE				(0x04)
#define HM_BLKCNT				(0x06)
#define HM_ARGUMENT				(0x08)
#define HM_TRNMOD				(0x0c)
#define HM_CMDREG				(0x0e)
#define HM_RSPREG0				(0x10)
#define HM_RSPREG1				(0x14)
#define HM_RSPREG2				(0x18)
#define HM_RSPREG3				(0x1c)
#define HM_BDATA				(0x20)
#define HM_PRNSTS				(0x24)
#define HM_HOSTCTL				(0x28)
#define HM_PWRCON				(0x29)
#define HM_BLKGAP				(0x2a)
#define HM_WAKCON				(0x2b)
#define HM_CLKCON				(0x2c)
#define HM_TIMEOUTCON			(0x2e)
#define HM_SWRST				(0x2f)
#define HM_NORINTSTS			(0x30)
#define HM_ERRINTSTS			(0x32)
#define HM_NORINTSTSEN			(0x34)
#define HM_ERRINTSTSEN			(0x36)
#define HM_NORINTSIGEN			(0x38)
#define HM_ERRINTSIGEN			(0x3a)
#define HM_ACMD12ERRSTS			(0x3c)
#define HM_CAPAREG				(0x40)
#define HM_MAXCURR				(0x48)
#define HM_CONTROL2				(0x80)
#define HM_CONTROL3				(0x84)
#define HM_CONTROL4				(0x8c)
#define HM_HCVER				(0xfe)

/*
 * Nand flash controller
 */
#define ELFIN_NAND_BASE			0x70200000

#define oNFCONF					0x00
#define oNFCONT					0x04
#define oNFCMMD					0x08
#define oNFADDR					0x0c
#define oNFDATA					0x10
#define oNFMECCDATA0			0x14
#define oNFMECCDATA1			0x18
#define oNFSECCDATA0			0x1c
#define oNFSBLK					0x20
#define oNFEBLK					0x24
#define oNFSTAT					0x28
#define oNFESTAT0				0x2c
#define oNFESTAT1				0x30
#define oNFMECC0				0x34
#define oNFMECC1				0x38
#define oNFSECC					0x3c
#define oNFMLCBITPT				0x40
#define oNF8ECCERR0				0x44
#define oNF8ECCERR1				0x48
#define oNF8ECCERR2				0x4c
#define oNFM8ECC0				0x50
#define oNFM8ECC1				0x54
#define oNFM8ECC2				0x58
#define oNFM8ECC3				0x5c
#define oNFMLC8BITPT0			0x60
#define oNFMLC8BITPT1			0x64

#define NFCONF					(ELFIN_NAND_BASE + oNFCONF)
#define NFCONT					(ELFIN_NAND_BASE + oNFCONT)
#define NFCMMD					(ELFIN_NAND_BASE + oNFCMMD)
#define NFADDR           		(ELFIN_NAND_BASE + oNFADDR)
#define NFDATA          		(ELFIN_NAND_BASE + oNFDATA)
#define NFMECCDATA0     		(ELFIN_NAND_BASE + oNFMECCDATA0)
#define NFMECCDATA1     		(ELFIN_NAND_BASE + oNFMECCDATA1)
#define NFSECCDATA0      		(ELFIN_NAND_BASE + oNFSECCDATA0)
#define NFSBLK          		(ELFIN_NAND_BASE + oNFSBLK)
#define NFEBLK           		(ELFIN_NAND_BASE + oNFEBLK)
#define NFSTAT           		(ELFIN_NAND_BASE + oNFSTAT)
#define NFESTAT0         		(ELFIN_NAND_BASE + oNFESTAT0)
#define NFESTAT1         		(ELFIN_NAND_BASE + oNFESTAT1)
#define NFMECC0          		(ELFIN_NAND_BASE + oNFMECC0)
#define NFMECC1          		(ELFIN_NAND_BASE + oNFMECC1)
#define NFSECC           		(ELFIN_NAND_BASE + oNFSECC)
#define NFMLCBITPT           	(ELFIN_NAND_BASE + oNFMLCBITPT)
#define NF8ECCERR0				(ELFIN_NAND_BASE + oNF8ECCERR0)
#define NF8ECCERR1				(ELFIN_NAND_BASE + oNF8ECCERR1)
#define NF8ECCERR2				(ELFIN_NAND_BASE + oNF8ECCERR2)
#define NFM8ECC0				(ELFIN_NAND_BASE + oNFM8ECC0)
#define NFM8ECC1				(ELFIN_NAND_BASE + oNFM8ECC1)
#define NFM8ECC2				(ELFIN_NAND_BASE + oNFM8ECC2)
#define NFM8ECC3				(ELFIN_NAND_BASE + oNFM8ECC3)
#define NFMLC8BITPT0			(ELFIN_NAND_BASE + oNFMLC8BITPT0)
#define NFMLC8BITPT1			(ELFIN_NAND_BASE + oNFMLC8BITPT1)


#define NFCONF_ECC_MLC			(1<<24)

#define NFCONF_ECC_1BIT			(0<<23)
#define NFCONF_ECC_4BIT			(2<<23)
#define NFCONF_ECC_8BIT			(1<<23)

#define NFCONT_ECC_ENC			(1<<18)
#define NFCONT_WP				(1<<16)
#define NFCONT_MECCLOCK			(1<<7)
#define NFCONT_SECCLOCK			(1<<6)
#define NFCONT_INITMECC			(1<<5)
#define NFCONT_INITSECC			(1<<4)
#define NFCONT_INITECC			(NFCONT_INITMECC | NFCONT_INITSECC)
#define NFCONT_CS_ALT			(1<<1)
#define NFCONT_CS				(1<<1)
#define NFSTAT_ECCENCDONE		(1<<7)
#define NFSTAT_ECCDECDONE		(1<<6)
#define NFSTAT_RnB				(1<<0)
#define NFESTAT0_ECCBUSY		(1<<31)



/*************************************************************
 * OneNAND Controller
 *************************************************************/

/*
 * S3C6400 SFRs
 */
#define ONENAND_REG_MEM_CFG			(0x000)
#define ONENAND_REG_BURST_LEN		(0x010)
#define ONENAND_REG_MEM_RESET		(0x020)
#define ONENAND_REG_INT_ERR_STAT	(0x030)
#define ONENAND_REG_INT_ERR_MASK	(0x040)
#define ONENAND_REG_INT_ERR_ACK		(0x050)
#define ONENAND_REG_ECC_ERR_STAT	(0x060)
#define ONENAND_REG_MANUFACT_ID		(0x070)
#define ONENAND_REG_DEVICE_ID		(0x080)
#define ONENAND_REG_DATA_BUF_SIZE	(0x090)
#define ONENAND_REG_BOOT_BUF_SIZE	(0x0A0)
#define ONENAND_REG_BUF_AMOUNT		(0x0B0)
#define ONENAND_REG_TECH			(0x0C0)
#define ONENAND_REG_FBA_WIDTH		(0x0D0)
#define ONENAND_REG_FPA_WIDTH		(0x0E0)
#define ONENAND_REG_FSA_WIDTH		(0x0F0)
#define ONENAND_REG_REVISION		(0x100)
#define ONENAND_REG_DATARAM0		(0x110)
#define ONENAND_REG_DATARAM1		(0x120)
#define ONENAND_REG_SYNC_MODE		(0x130)
#define ONENAND_REG_TRANS_SPARE		(0x140)
#define ONENAND_REG_LOCK_BIT		(0x150)
#define ONENAND_REG_DBS_DFS_WIDTH	(0x160)
#define ONENAND_REG_PAGE_CNT		(0x170)
#define ONENAND_REG_ERR_PAGE_ADDR	(0x180)
#define ONENAND_REG_BURST_RD_LAT	(0x190)
#define ONENAND_REG_INT_PIN_ENABLE	(0x1A0)
#define ONENAND_REG_INT_MON_CYC		(0x1B0)
#define ONENAND_REG_ACC_CLOCK		(0x1C0)
#define ONENAND_REG_SLOW_RD_PATH	(0x1D0)
#define ONENAND_REG_ERR_BLK_ADDR	(0x1E0)
#define ONENAND_REG_FLASH_VER_ID	(0x1F0)
#define ONENAND_REG_FLASH_AUX_CNTRL	(0x300)

/*
 * S3C6400 SFR values
 */
#define ONENAND_MEM_CFG_SYNC_READ	(1 << 15)
#define ONENAND_MEM_CFG_BRL_7		(7 << 12)
#define ONENAND_MEM_CFG_BRL_6		(6 << 12)
#define ONENAND_MEM_CFG_BRL_5		(5 << 12)
#define ONENAND_MEM_CFG_BRL_4		(4 << 12)
#define ONENAND_MEM_CFG_BRL_3		(3 << 12)
#define ONENAND_MEM_CFG_BRL_10		(2 << 12)
#define ONENAND_MEM_CFG_BRL_9		(1 << 12)
#define ONENAND_MEM_CFG_BRL_8		(0 << 12)
#define ONENAND_MEM_CFG_BRL_SHIFT	(12)
#define ONENAND_MEM_CFG_BL_1K		(5 << 9)
#define ONENAND_MEM_CFG_BL_32		(4 << 9)
#define ONENAND_MEM_CFG_BL_16		(3 << 9)
#define ONENAND_MEM_CFG_BL_8		(2 << 9)
#define ONENAND_MEM_CFG_BL_4		(1 << 9)
#define ONENAND_MEM_CFG_BL_CONT		(0 << 9)
#define ONENAND_MEM_CFG_BL_SHIFT	(9)
#define ONENAND_MEM_CFG_NO_ECC		(1 << 8)
#define ONENAND_MEM_CFG_RDY_HIGH	(1 << 7)
#define ONENAND_MEM_CFG_INT_HIGH	(1 << 6)
#define ONENAND_MEM_CFG_IOBE		(1 << 5)
#define ONENAND_MEM_CFG_RDY_CONF	(1 << 4)
#define ONENAND_MEM_CFG_HF			(1 << 2)
#define ONENAND_MEM_CFG_WM_SYNC		(1 << 1)
#define ONENAND_MEM_CFG_BWPS_UNLOCK	(1 << 0)

#define ONENAND_BURST_LEN_CONT		(0)
#define ONENAND_BURST_LEN_4			(4)
#define ONENAND_BURST_LEN_8			(8)
#define ONENAND_BURST_LEN_16		(16)

#define ONENAND_MEM_RESET_WARM		(0x1)
#define ONENAND_MEM_RESET_COLD		(0x2)
#define ONENAND_MEM_RESET_HOT		(0x3)

#define ONENAND_INT_ERR_CACHE_OP_ERR	(1 << 13)
#define ONENAND_INT_ERR_RST_CMP		(1 << 12)
#define ONENAND_INT_ERR_RDY_ACT		(1 << 11)
#define ONENAND_INT_ERR_INT_ACT		(1 << 10)
#define ONENAND_INT_ERR_UNSUP_CMD	(1 << 9)
#define ONENAND_INT_ERR_LOCKED_BLK	(1 << 8)
#define ONENAND_INT_ERR_BLK_RW_CMP	(1 << 7)
#define ONENAND_INT_ERR_ERS_CMP		(1 << 6)
#define ONENAND_INT_ERR_PGM_CMP		(1 << 5)
#define ONENAND_INT_ERR_LOAD_CMP	(1 << 4)
#define ONENAND_INT_ERR_ERS_FAIL	(1 << 3)
#define ONENAND_INT_ERR_PGM_FAIL	(1 << 2)
#define ONENAND_INT_ERR_INT_TO		(1 << 1)
#define ONENAND_INT_ERR_LD_FAIL_ECC_ERR	(1 << 0)

#define ONENAND_DEVICE_DENSITY_SHIFT	(4)
#define ONENAND_DEVICE_IS_DDP		(1 << 3)
#define ONENAND_DEVICE_IS_DEMUX		(1 << 2)
#define ONENAND_DEVICE_VCC_MASK		(0x3)
#define ONENAND_DEVICE_DENSITY_128Mb	(0x000)
#define ONENAND_DEVICE_DENSITY_256Mb	(0x001)
#define ONENAND_DEVICE_DENSITY_512Mb	(0x002)
#define ONENAND_DEVICE_DENSITY_1Gb	(0x003)
#define ONENAND_DEVICE_DENSITY_2Gb	(0x004)
#define ONENAND_DEVICE_DENSITY_4Gb	(0x005)

#define ONENAND_SYNC_MODE_RM_SYNC	(1 << 1)
#define ONENAND_SYNC_MODE_WM_SYNC	(1 << 0)

#define ONENAND_TRANS_SPARE_TSRF_INC	(1 << 0)

#define ONENAND_INT_PIN_ENABLE		(1 << 0)

#define ONENAND_ACC_CLOCK_266_133	(0x5)
#define ONENAND_ACC_CLOCK_166_83	(0x3)
#define ONENAND_ACC_CLOCK_134_67	(0x3)
#define ONENAND_ACC_CLOCK_100_50	(0x2)
#define ONENAND_ACC_CLOCK_60_30		(0x2)

#define ONENAND_FLASH_AUX_WD_DISABLE	(1 << 0)

/*
 * Datain values for mapped commands
 */
#define ONENAND_DATAIN_ERASE_STATUS	(0x00)
#define ONENAND_DATAIN_ERASE_MULTI	(0x01)
#define ONENAND_DATAIN_ERASE_SINGLE	(0x03)
#define ONENAND_DATAIN_ERASE_VERIFY	(0x15)
#define ONENAND_DATAIN_UNLOCK_START	(0x08)
#define ONENAND_DATAIN_UNLOCK_END	(0x09)
#define ONENAND_DATAIN_LOCK_START	(0x0A)
#define ONENAND_DATAIN_LOCK_END		(0x0B)
#define ONENAND_DATAIN_LOCKTIGHT_START	(0x0C)
#define ONENAND_DATAIN_LOCKTIGHT_END	(0x0D)
#define ONENAND_DATAIN_UNLOCK_ALL	(0x0E)
#define ONENAND_DATAIN_COPYBACK_SRC	(0x1000)
#define ONENAND_DATAIN_COPYBACK_DST	(0x2000)
#define ONENAND_DATAIN_ACCESS_OTP	(0x12)
#define ONENAND_DATAIN_ACCESS_MAIN	(0x14)
#define ONENAND_DATAIN_PIPELINE_READ	(0x4000)
#define ONENAND_DATAIN_PIPELINE_WRITE	(0x4100)
#define ONENAND_DATAIN_RMW_LOAD		(0x10)
#define ONENAND_DATAIN_RMW_MODIFY	(0x11)

/*
 * Device ID Register F001h (R)
 */
#define ONENAND_DEVICE_DENSITY_SHIFT	(4)
#define ONENAND_DEVICE_IS_DDP		(1 << 3)
#define ONENAND_DEVICE_IS_DEMUX		(1 << 2)
#define ONENAND_DEVICE_VCC_MASK		(0x3)

/*
 * Version ID Register F002h (R)
 */
#define ONENAND_VERSION_PROCESS_SHIFT	(8)

/*
 * Start Address 1 F100h (R/W)
 */
#define ONENAND_DDP_SHIFT		(15)
#define ONENAND_DDP_CHIP0		(0)
#define ONENAND_DDP_CHIP1		(1 << ONENAND_DDP_SHIFT)

/*
 * Start Buffer Register F200h (R/W)
 */
#define ONENAND_BSA_MASK		(0x03)
#define ONENAND_BSA_SHIFT		(8)
#define ONENAND_BSA_BOOTRAM		(0 << 2)
#define ONENAND_BSA_DATARAM0	(2 << 2)
#define ONENAND_BSA_DATARAM1	(3 << 2)
#define ONENAND_BSC_MASK		(0x03)

/*
 * Command Register F220h (R/W)
 */
#define ONENAND_CMD_READ		(0x00)
#define ONENAND_CMD_READOOB		(0x13)
#define ONENAND_CMD_PROG		(0x80)
#define ONENAND_CMD_PROGOOB		(0x1A)
#define ONENAND_CMD_UNLOCK		(0x23)
#define ONENAND_CMD_LOCK		(0x2A)
#define ONENAND_CMD_LOCK_TIGHT	(0x2C)
#define ONENAND_CMD_UNLOCK_ALL	(0x27)
#define ONENAND_CMD_ERASE		(0x94)
#define ONENAND_CMD_RESET		(0xF0)
#define ONENAND_CMD_OTP_ACCESS	(0x65)
#define ONENAND_CMD_READID		(0x90)
#define ONENAND_CMD_STARTADDR1	(0xE0)
#define ONENAND_CMD_WP_STATUS	(0xE1)
#define ONENAND_CMD_PIPELINE_READ	(0x01)
#define ONENAND_CMD_PIPELINE_WRITE	(0x81)

/*
 * Command Mapping for S3C6400 OneNAND Controller
 */
#define ONENAND_AHB_ADDR		(0x20000000)
#define ONENAND_DUMMY_ADDR		(0x20400000)
#define ONENAND_CMD_SHIFT		(24)
#define ONENAND_CMD_MAP_00		(0x0)
#define ONENAND_CMD_MAP_01		(0x1)
#define ONENAND_CMD_MAP_10		(0x2)
#define ONENAND_CMD_MAP_11		(0x3)
#define ONENAND_CMD_MAP_FF		(0xF)

/*
 * Mask for Mapping table
 */
#define ONENAND_MEM_ADDR_MASK	(0xffffff)
#define ONENAND_DDP_SHIFT_1Gb	(21)
#define ONENAND_DDP_SHIFT_2Gb	(22)
#define ONENAND_DDP_SHIFT_4Gb	(23)
#define ONENAND_FBA_SHIFT		(12)
#define ONENAND_FPA_SHIFT		(6)
#define ONENAND_FSA_SHIFT		(4)
#define ONENAND_FBA_MASK_128Mb	(0xff)
#define ONENAND_FBA_MASK_256Mb	(0x1ff)
#define ONENAND_FBA_MASK_512Mb	(0x1ff)
#define ONENAND_FBA_MASK_1Gb_DDP	(0x1ff)
#define ONENAND_FBA_MASK_1Gb		(0x3ff)
#define ONENAND_FBA_MASK_2Gb_DDP	(0x3ff)
#define ONENAND_FBA_MASK_2Gb		(0x7ff)
#define ONENAND_FBA_MASK_4Gb_DDP	(0x7ff)
#define ONENAND_FBA_MASK_4Gb		(0xfff)
#define ONENAND_FPA_MASK		(0x3f)
#define ONENAND_FSA_MASK		(0x3)

/*
 * System Configuration 1 Register F221h (R, R/W)
 */
#define ONENAND_SYS_CFG1_SYNC_READ	(1 << 15)
#define ONENAND_SYS_CFG1_BRL_7		(7 << 12)
#define ONENAND_SYS_CFG1_BRL_6		(6 << 12)
#define ONENAND_SYS_CFG1_BRL_5		(5 << 12)
#define ONENAND_SYS_CFG1_BRL_4		(4 << 12)
#define ONENAND_SYS_CFG1_BRL_3		(3 << 12)
#define ONENAND_SYS_CFG1_BRL_10		(2 << 12)
#define ONENAND_SYS_CFG1_BRL_9		(1 << 12)
#define ONENAND_SYS_CFG1_BRL_8		(0 << 12)
#define ONENAND_SYS_CFG1_BRL_SHIFT	(12)
#define ONENAND_SYS_CFG1_BL_32		(4 << 9)
#define ONENAND_SYS_CFG1_BL_16		(3 << 9)
#define ONENAND_SYS_CFG1_BL_8		(2 << 9)
#define ONENAND_SYS_CFG1_BL_4		(1 << 9)
#define ONENAND_SYS_CFG1_BL_CONT	(0 << 9)
#define ONENAND_SYS_CFG1_BL_SHIFT	(9)
#define ONENAND_SYS_CFG1_NO_ECC		(1 << 8)
#define ONENAND_SYS_CFG1_RDY		(1 << 7)
#define ONENAND_SYS_CFG1_INT		(1 << 6)
#define ONENAND_SYS_CFG1_IOBE		(1 << 5)
#define ONENAND_SYS_CFG1_RDY_CONF	(1 << 4)

/*
 * Controller Status Register F240h (R)
 */
#define ONENAND_CTRL_ONGO		(1 << 15)
#define ONENAND_CTRL_LOCK		(1 << 14)
#define ONENAND_CTRL_LOAD		(1 << 13)
#define ONENAND_CTRL_PROGRAM	(1 << 12)
#define ONENAND_CTRL_ERASE		(1 << 11)
#define ONENAND_CTRL_ERROR		(1 << 10)
#define ONENAND_CTRL_RSTB		(1 << 7)
#define ONENAND_CTRL_OTP_L		(1 << 6)
#define ONENAND_CTRL_OTP_BL		(1 << 5)

/*
 * Interrupt Status Register F241h (R)
 */
#define ONENAND_INT_MASTER		(1 << 15)
#define ONENAND_INT_READ		(1 << 7)
#define ONENAND_INT_WRITE		(1 << 6)
#define ONENAND_INT_ERASE		(1 << 5)
#define ONENAND_INT_RESET		(1 << 4)
#define ONENAND_INT_CLEAR		(0 << 0)

/*
 * NAND Flash Write Protection Status Register F24Eh (R)
 */
#define ONENAND_WP_US			(1 << 2)
#define ONENAND_WP_LS			(1 << 1)
#define ONENAND_WP_LTS			(1 << 0)

/*
 * ECC Status Register FF00h (R)
 */
#define ONENAND_ECC_1BIT		(1 << 0)
#define ONENAND_ECC_1BIT_ALL	(0x5555)
#define ONENAND_ECC_2BIT		(1 << 1)
#define ONENAND_ECC_2BIT_ALL	(0xAAAA)

/*
 * One-Time Programmable (OTP)
 */
#define ONENAND_OTP_LOCK_OFFSET	(14)

/*************************************************************
 * End of OneNAND Controller
 *************************************************************/

/*
 * Watchdog timer
 */
#define ELFIN_WATCHDOG_BASE		0x7E004000

#define oWTCON					0x04
#define oWTDAT					0x08
#define oWTCNT					0x0c

#define WTCON					(ELFIN_WATCHDOG_BASE + oWTCON)
#define WTDAT					(ELFIN_WATCHDOG_BASE + oWTDAT)
#define WTCNT					(ELFIN_WATCHDOG_BASE + oWTCNT)



/*
 * UART
 */
#define ELFIN_UART_BASE			0x7F005000

#define oULCON					0x00
#define oUCON					0x04
#define oUFCON					0x08
#define oUMCON					0x0C
#define oUTRSTAT				0x10
#define oUERSTAT				0x14
#define oUFSTAT					0x18
#define oUMSTAT					0x1C
#define oUTXH					0x20
#define oURXH					0x24
#define oUBRDIV					0x28
#define oUDIVSLOT				0x2C
#define oUINTP					0x30
#define oUINTSP					0x34
#define oUINTM					0x38

#ifdef CONFIG_SERIAL1
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + 0x0000)
#elif defined(CONFIG_SERIAL2)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + 0x0400)
#else
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + 0x0000)
#endif

#define ELFIN_UART0_BASE 		(ELFIN_UART_BASE + 0x0000)

#define ULCON0					(ELFIN_UART0_BASE + oULCON)
#define UCON0					(ELFIN_UART0_BASE + oUCON)
#define UFCON0					(ELFIN_UART0_BASE + oUFCON)
#define UMCON0					(ELFIN_UART0_BASE + oUMCON)
#define UTRSTAT0				(ELFIN_UART0_BASE + oUTRSTAT)
#define UERSTAT0				(ELFIN_UART0_BASE + oUERSTAT)
#define UFSTAT0					(ELFIN_UART0_BASE + oUFSTAT)
#define UMSTAT0					(ELFIN_UART0_BASE + oUMSTAT)
#define UTXH0					(ELFIN_UART0_BASE + oUTXH)
#define URXH0					(ELFIN_UART0_BASE + oURXH)
#define UBRDIV0					(ELFIN_UART0_BASE + oUBRDIV)
#define UDIVSLOT0				(ELFIN_UART0_BASE + oUDIVSLOT)
#define UINTP0					(ELFIN_UART0_BASE + oUINTP)
#define UINTSP0					(ELFIN_UART0_BASE + oUINTSP)
#define UINTM0					(ELFIN_UART0_BASE + oUINTM)

#define ELFIN_UART1_BASE 		(ELFIN_UART_BASE + 0x0400)

#define ULCON1					(ELFIN_UART1_BASE + oULCON)
#define UCON1					(ELFIN_UART1_BASE + oUCON)
#define UFCON1					(ELFIN_UART1_BASE + oUFCON)
#define UMCON1					(ELFIN_UART1_BASE + oUMCON)
#define UTRSTAT1				(ELFIN_UART1_BASE + oUTRSTAT)
#define UERSTAT1				(ELFIN_UART1_BASE + oUERSTAT)
#define UFSTAT1					(ELFIN_UART1_BASE + oUFSTAT)
#define UMSTAT1					(ELFIN_UART1_BASE + oUMSTAT)
#define UTXH1					(ELFIN_UART1_BASE + oUTXH)
#define URXH1					(ELFIN_UART1_BASE + oURXH)
#define UBRDIV1					(ELFIN_UART1_BASE + oUBRDIV)
#define UDIVSLOT1				(ELFIN_UART1_BASE + oUDIVSLOT)
#define UINTP1					(ELFIN_UART1_BASE + oUINTP)
#define UINTSP1					(ELFIN_UART1_BASE + oUINTSP)
#define UINTM1					(ELFIN_UART1_BASE + oUINTM)

#define UTRSTAT_TX_EMPTY		BIT(2)
#define UTRSTAT_RX_READY		BIT(0)
#define UART_ERR_MASK			0xF


/*
 * PWM timer
 */
#define ELFIN_TIMER_BASE		0x7F006000

#define oTCFG0					0x00
#define oTCFG1					0x04
#define oTCON					0x08
#define oTCNTB0					0x0c
#define oTCMPB0					0x10
#define oTCNTO0					0x14
#define oTCNTB1					0x18
#define oTCMPB1					0x1c
#define oTCNTO1					0x20
#define oTCNTB2					0x24
#define oTCMPB2					0x28
#define oTCNTO2					0x2c
#define oTCNTB3					0x30
#define oTCMPB3					0x34
#define oTCNTO3					0x38
#define oTCNTB4					0x3c
#define oTCNTO4					0x40
#define oTINT_CSTAT				0x44

#define TCFG0					(ELFIN_TIMER_BASE + oTCFG0)
#define TCFG1					(ELFIN_TIMER_BASE + oTCFG1)
#define TCON					(ELFIN_TIMER_BASE + oTCON)
#define TCNTB0					(ELFIN_TIMER_BASE + oTCNTB0)
#define TCMPB0					(ELFIN_TIMER_BASE + oTCMPB0)
#define TCNTO0					(ELFIN_TIMER_BASE + oTCNTO0)
#define TCNTB1					(ELFIN_TIMER_BASE + oTCNTB1)
#define TCMPB1					(ELFIN_TIMER_BASE + oTCMPB1)
#define TCNTO1					(ELFIN_TIMER_BASE + oTCNTO1)
#define TCNTB2					(ELFIN_TIMER_BASE + oTCNTB2)
#define TCMPB2					(ELFIN_TIMER_BASE + oTCMPB2)
#define TCNTO2					(ELFIN_TIMER_BASE + oTCNTO2)
#define TCNTB3					(ELFIN_TIMER_BASE + oTCNTB3)
#define TCMPB3					(ELFIN_TIMER_BASE + oTCMPB3)
#define TCNTO3					(ELFIN_TIMER_BASE + oTCNTO3)
#define TCNTB4					(ELFIN_TIMER_BASE + oTCNTB4)
#define TCNTO4					(ELFIN_TIMER_BASE + oTCNTO4)
#define TINT_CSTAT				(ELFIN_TIMER_BASE + oTINT_CSTAT)

/* Fields */
#define fTCFG0_DZONE			Fld(8,16)       /* the dead zone length (= timer 0) */
#define fTCFG0_PRE1				Fld(8,8)        /* prescaler value for time 2,3,4 */
#define fTCFG0_PRE0				Fld(8,0)        /* prescaler value for time 0,1 */
#define fTCFG1_MUX4				Fld(4,16)
/* bits */
#define TCFG0_DZONE(x)			FInsrt((x), fTCFG0_DZONE)
#define TCFG0_PRE1(x)			FInsrt((x), fTCFG0_PRE1)
#define TCFG0_PRE0(x)			FInsrt((x), fTCFG0_PRE0)
#define TCON_4_AUTO				(1 << 22)       /* auto reload on/off for Timer 4 */
#define TCON_4_UPDATE			(1 << 21)       /* manual Update TCNTB4 */
#define TCON_4_ONOFF			(1 << 20)       /* 0: Stop, 1: start Timer 4 */
#define COUNT_4_ON				(TCON_4_ONOFF*1)
#define COUNT_4_OFF				(TCON_4_ONOFF*0)
#define TCON_3_AUTO				(1 << 19)       /* auto reload on/off for Timer 3 */
#define TIMER3_ATLOAD_ON		(TCON_3_AUTO*1)
#define TIMER3_ATLAOD_OFF		FClrBit(TCON, TCON_3_AUTO)
#define TCON_3_INVERT			(1 << 18)       /* 1: Inverter on for TOUT3 */
#define TIMER3_IVT_ON			(TCON_3_INVERT*1)
#define TIMER3_IVT_OFF			(FClrBit(TCON, TCON_3_INVERT))
#define TCON_3_MAN				(1 << 17)       /* manual Update TCNTB3,TCMPB3 */
#define TIMER3_MANUP			(TCON_3_MAN*1)
#define TIMER3_NOP				(FClrBit(TCON, TCON_3_MAN))
#define TCON_3_ONOFF			(1 << 16)       /* 0: Stop, 1: start Timer 3 */
#define TIMER3_ON				(TCON_3_ONOFF*1)
#define TIMER3_OFF				(FClrBit(TCON, TCON_3_ONOFF))
/* macros */
#define GET_PRESCALE_TIMER4(x)	FExtr((x), fTCFG0_PRE1)
#define GET_DIVIDER_TIMER4(x)	FExtr((x), fTCFG1_MUX4)

/*
 * RTC Controller
 */
#define ELFIN_RTC_BASE			0x7e005000

#define oRTCCON					0x40
#define oTICNT					0x44
#define oRTCALM					0x50
#define oALMSEC					0x54
#define oALMMIN					0x58
#define oALMHOUR				0x5c
#define oALMDATE				0x60
#define oALMMON					0x64
#define oALMYEAR				0x68
#define oBCDSEC					0x70
#define oBCDMIN					0x74
#define oBCDHOUR				0x78
#define oBCDDATE				0x7c
#define oBCDDAY					0x80
#define oBCDMON					0x84
#define oBCDYEAR				0x88

#define RTCCON					(ELFIN_RTC_BASE + oRTCCON)
#define TICNT					(ELFIN_RTC_BASE + oTICNT)
#define RTCALM					(ELFIN_RTC_BASE + oRTCALM)
#define ALMSEC					(ELFIN_RTC_BASE + oALMSEC)
#define ALMMIN					(ELFIN_RTC_BASE + oALMMIN)
#define ALMHOUR					(ELFIN_RTC_BASE + oALMHOUR)
#define ALMDATE					(ELFIN_RTC_BASE + oALMDATE)
#define ALMMON					(ELFIN_RTC_BASE + oALMMON)
#define ALMYEAR					(ELFIN_RTC_BASE + oALMYEAR)
#define BCDSEC					(ELFIN_RTC_BASE + oBCDSEC)
#define BCDMIN					(ELFIN_RTC_BASE + oBCDMIN)
#define BCDHOUR					(ELFIN_RTC_BASE + oBCDHOUR)
#define BCDDATE					(ELFIN_RTC_BASE + oBCDDATE)
#define BCDDAY					(ELFIN_RTC_BASE + oBCDDAY)
#define BCDMON					(ELFIN_RTC_BASE + oBCDMON)
#define BCDYEAR					(ELFIN_RTC_BASE + oBCDYEAR)

/*
 * USB2.0 HS OTG (Chapter 26)
 */
#define USBOTG_LINK_BASE		(0x7C000000)
#define USBOTG_PHY_BASE			(0x7C100000)

/* Core Global Registers */
#define S3C_OTG_GOTGCTL			(USBOTG_LINK_BASE + 0x000)	/* OTG Control & Status */
#define S3C_OTG_GOTGINT			(USBOTG_LINK_BASE + 0x004)	/* OTG Interrupt */
#define S3C_OTG_GAHBCFG			(USBOTG_LINK_BASE + 0x008)	/* Core AHB Configuration */
#define S3C_OTG_GUSBCFG			(USBOTG_LINK_BASE + 0x00C)	/* Core USB Configuration */
#define S3C_OTG_GRSTCTL			(USBOTG_LINK_BASE + 0x010)	/* Core Reset */
#define S3C_OTG_GINTSTS			(USBOTG_LINK_BASE + 0x014)	/* Core Interrupt */
#define S3C_OTG_GINTMSK			(USBOTG_LINK_BASE + 0x018)	/* Core Interrupt Mask */
#define S3C_OTG_GRXSTSR			(USBOTG_LINK_BASE + 0x01C)	/* Receive Status Debug Read/Status Read */
#define S3C_OTG_GRXSTSP			(USBOTG_LINK_BASE + 0x020)	/* Receive Status Debug Pop/Status Pop */
#define S3C_OTG_GRXFSIZ			(USBOTG_LINK_BASE + 0x024)	/* Receive FIFO Size */
#define S3C_OTG_GNPTXFSIZ		(USBOTG_LINK_BASE + 0x028)	/* Non-Periodic Transmit FIFO Size */
#define S3C_OTG_GNPTXSTS		(USBOTG_LINK_BASE + 0x02C)	/* Non-Periodic Transmit FIFO/Queue Status */

#define S3C_OTG_HPTXFSIZ		(USBOTG_LINK_BASE + 0x100)	/* Host Periodic Transmit FIFO Size */
#define S3C_OTG_DPTXFSIZ1		(USBOTG_LINK_BASE + 0x104)	/* Device Periodic Transmit FIFO-1 Size */
#define S3C_OTG_DPTXFSIZ2		(USBOTG_LINK_BASE + 0x108)	/* Device Periodic Transmit FIFO-2 Size */
#define S3C_OTG_DPTXFSIZ3		(USBOTG_LINK_BASE + 0x10C)	/* Device Periodic Transmit FIFO-3 Size */
#define S3C_OTG_DPTXFSIZ4		(USBOTG_LINK_BASE + 0x110)	/* Device Periodic Transmit FIFO-4 Size */
#define S3C_OTG_DPTXFSIZ5		(USBOTG_LINK_BASE + 0x114)	/* Device Periodic Transmit FIFO-5 Size */
#define S3C_OTG_DPTXFSIZ6		(USBOTG_LINK_BASE + 0x118)	/* Device Periodic Transmit FIFO-6 Size */
#define S3C_OTG_DPTXFSIZ7		(USBOTG_LINK_BASE + 0x11C)	/* Device Periodic Transmit FIFO-7 Size */
#define S3C_OTG_DPTXFSIZ8		(USBOTG_LINK_BASE + 0x120)	/* Device Periodic Transmit FIFO-8 Size */
#define S3C_OTG_DPTXFSIZ9		(USBOTG_LINK_BASE + 0x124)	/* Device Periodic Transmit FIFO-9 Size */
#define S3C_OTG_DPTXFSIZ10		(USBOTG_LINK_BASE + 0x128)	/* Device Periodic Transmit FIFO-10 Size */
#define S3C_OTG_DPTXFSIZ11		(USBOTG_LINK_BASE + 0x12C)	/* Device Periodic Transmit FIFO-11 Size */
#define S3C_OTG_DPTXFSIZ12		(USBOTG_LINK_BASE + 0x130)	/* Device Periodic Transmit FIFO-12 Size */
#define S3C_OTG_DPTXFSIZ13		(USBOTG_LINK_BASE + 0x134)	/* Device Periodic Transmit FIFO-13 Size */
#define S3C_OTG_DPTXFSIZ14		(USBOTG_LINK_BASE + 0x138)	/* Device Periodic Transmit FIFO-14 Size */
#define S3C_OTG_DPTXFSIZ15		(USBOTG_LINK_BASE + 0x13C)	/* Device Periodic Transmit FIFO-15 Size */

/* Host Global Registers */
#define S3C_OTG_HCFG			(USBOTG_LINK_BASE + 0x400)	/* Host Configuration */
#define S3C_OTG_HFIR			(USBOTG_LINK_BASE + 0x404)	/* Host Frame Interval */
#define S3C_OTG_HFNUM			(USBOTG_LINK_BASE + 0x408)	/* Host Frame Number/Frame Time Remaining */
#define S3C_OTG_HPTXSTS			(USBOTG_LINK_BASE + 0x410)	/* Host Periodic Transmit FIFO/Queue Status */
#define S3C_OTG_HAINT			(USBOTG_LINK_BASE + 0x414)	/* Host All Channels Interrupt */
#define S3C_OTG_HAINTMSK		(USBOTG_LINK_BASE + 0x418)	/* Host All Channels Interrupt Mask */

/* Host Port Control & Status Registers */
#define S3C_OTG_HPRT			(USBOTG_LINK_BASE + 0x440)	/* Host Port Control & Status */

/* Host Channel-Specific Registers */
#define S3C_OTG_HCCHAR0			(USBOTG_LINK_BASE + 0x500)	/* Host Channel-0 Characteristics */
#define S3C_OTG_HCSPLT0			(USBOTG_LINK_BASE + 0x504)	/* Host Channel-0 Split Control */
#define S3C_OTG_HCINT0			(USBOTG_LINK_BASE + 0x508)	/* Host Channel-0 Interrupt */
#define S3C_OTG_HCINTMSK0		(USBOTG_LINK_BASE + 0x50C)	/* Host Channel-0 Interrupt Mask */
#define S3C_OTG_HCTSIZ0			(USBOTG_LINK_BASE + 0x510)	/* Host Channel-0 Transfer Size */
#define S3C_OTG_HCDMA0			(USBOTG_LINK_BASE + 0x514)	/* Host Channel-0 DMA Address */


/* Device Global Registers */
#define S3C_OTG_DCFG			(USBOTG_LINK_BASE + 0x800)	/* Device Configuration */
#define S3C_OTG_DCTL			(USBOTG_LINK_BASE + 0x804)	/* Device Control */
#define S3C_OTG_DSTS			(USBOTG_LINK_BASE + 0x808)	/* Device Status */
#define S3C_OTG_DIEPMSK 		(USBOTG_LINK_BASE + 0x810)	/* Device IN Endpoint Common Interrupt Mask */
#define S3C_OTG_DOEPMSK 		(USBOTG_LINK_BASE + 0x814)	/* Device OUT Endpoint Common Interrupt Mask */
#define S3C_OTG_DAINT			(USBOTG_LINK_BASE + 0x818)	/* Device All Endpoints Interrupt */
#define S3C_OTG_DAINTMSK		(USBOTG_LINK_BASE + 0x81C)	/* Device All Endpoints Interrupt Mask */
#define S3C_OTG_DTKNQR1 		(USBOTG_LINK_BASE + 0x820)	/* Device IN Token Sequence Learning Queue Read 1 */
#define S3C_OTG_DTKNQR2 		(USBOTG_LINK_BASE + 0x824)	/* Device IN Token Sequence Learning Queue Read 2 */
#define S3C_OTG_DVBUSDIS		(USBOTG_LINK_BASE + 0x828)	/* Device VBUS Discharge Time */
#define S3C_OTG_DVBUSPULSE		(USBOTG_LINK_BASE + 0x82C)	/* Device VBUS Pulsing Time */
#define S3C_OTG_DTKNQR3 		(USBOTG_LINK_BASE + 0x830)	/* Device IN Token Sequence Learning Queue Read 3 */
#define S3C_OTG_DTKNQR4 		(USBOTG_LINK_BASE + 0x834)	/* Device IN Token Sequence Learning Queue Read 4 */

/* Device Logical IN Endpoint-Specific Registers */
#define S3C_OTG_DIEPCTL0		(USBOTG_LINK_BASE + 0x900)	/* Device IN Endpoint 0 Control */
#define S3C_OTG_DIEPINT0		(USBOTG_LINK_BASE + 0x908)	/* Device IN Endpoint 0 Interrupt */
#define S3C_OTG_DIEPTSIZ0		(USBOTG_LINK_BASE + 0x910)	/* Device IN Endpoint 0 Transfer Size */
#define S3C_OTG_DIEPDMA0		(USBOTG_LINK_BASE + 0x914)	/* Device IN Endpoint 0 DMA Address */

/* Device Logical OUT Endpoint-Specific Registers */
#define S3C_OTG_DOEPCTL0		(USBOTG_LINK_BASE + 0xB00)	/* Device OUT Endpoint 0 Control */
#define S3C_OTG_DOEPINT0		(USBOTG_LINK_BASE + 0xB08)	/* Device OUT Endpoint 0 Interrupt */
#define S3C_OTG_DOEPTSIZ0		(USBOTG_LINK_BASE + 0xB10)	/* Device OUT Endpoint 0 Transfer Size */
#define S3C_OTG_DOEPDMA0		(USBOTG_LINK_BASE + 0xB14)	/* Device OUT Endpoint 0 DMA Address */

/* Power & clock gating registers */
#define S3C_OTG_PCGCCTRL		(USBOTG_LINK_BASE + 0xE00)

/* Endpoint FIFO address */
#define S3C_OTG_EP0_FIFO		(USBOTG_LINK_BASE + 0x1000)

/* OTG PHY CORE REGISTERS */
#define S3C_OTG_PHYPWR			(USBOTG_PHY_BASE + 0x00)
#define S3C_OTG_PHYCTRL			(USBOTG_PHY_BASE + 0x04)
#define S3C_OTG_RSTCON			(USBOTG_PHY_BASE + 0x08)



/*
 * Interrupt
 */
#define ELFIN_VIC0_BASE_ADDR	(0x71200000)
#define ELFIN_VIC1_BASE_ADDR	(0x71300000)

#define oIRQSTATUS				0x000
#define oFIQSTATUS				0x004
#define oRAWINTR				0x008
#define oINTSELECT				0x00c
#define oINTENABLE				0x010
#define oINTENCLEAR				0x014
#define oSOFTINT				0x018
#define oSOFTINTCLEAR			0x01c
#define oPROTECTION				0x020
#define oSWPRIORITYMASK			0x024
#define oPRIORITYDAISY			0x028
#define oVECTADDR(X)			(0x100+(X)*4)
#define oVECPRIORITY(X)			(0x200+(X)*4)
#define oVECTADDRESS			0xF00

#define VIC0IRQSTATUS			(ELFIN_VIC0_BASE_ADDR + oIRQSTATUS)
#define VIC0FIQSTATUS			(ELFIN_VIC0_BASE_ADDR + oFIQSTATUS)
#define VIC0RAWINTR				(ELFIN_VIC0_BASE_ADDR + oRAWINTR)
#define VIC0INTSELECT			(ELFIN_VIC0_BASE_ADDR + oINTSELECT)
#define VIC0INTENABLE			(ELFIN_VIC0_BASE_ADDR + oINTENABLE)
#define VIC0INTENCLEAR			(ELFIN_VIC0_BASE_ADDR + oINTENCLEAR)
#define VIC0SOFTINT				(ELFIN_VIC0_BASE_ADDR + oSOFTINT)
#define VIC0SOFTINTCLEAR		(ELFIN_VIC0_BASE_ADDR + oSOFTINTCLEAR)
#define VIC0PROTECTION			(ELFIN_VIC0_BASE_ADDR + oPROTECTION)
#define VIC0SWPRIORITYMASK		(ELFIN_VIC0_BASE_ADDR + oSWPRIORITYMASK)
#define VIC0PRIORITYDAISY		(ELFIN_VIC0_BASE_ADDR + oPRIORITYDAISY)
#define VIC0VECTADDR(X)			(ELFIN_VIC0_BASE_ADDR + oVECTADDR(X))
#define VIC0VECPRIORITY(X)		(ELFIN_VIC0_BASE_ADDR + oVECPRIORITY(X))
#define VIC0VECTADDRESS			(ELFIN_VIC0_BASE_ADDR + oVECTADDRESS)

#define VIC1IRQSTATUS			(ELFIN_VIC1_BASE_ADDR + oIRQSTATUS)
#define VIC1FIQSTATUS			(ELFIN_VIC1_BASE_ADDR + oFIQSTATUS)
#define VIC1RAWINTR				(ELFIN_VIC1_BASE_ADDR + oRAWINTR)
#define VIC1INTSELECT			(ELFIN_VIC1_BASE_ADDR + oINTSELECT)
#define VIC1INTENABLE			(ELFIN_VIC1_BASE_ADDR + oINTENABLE)
#define VIC1INTENCLEAR			(ELFIN_VIC1_BASE_ADDR + oINTENCLEAR)
#define VIC1SOFTINT				(ELFIN_VIC1_BASE_ADDR + oSOFTINT)
#define VIC1SOFTINTCLEAR		(ELFIN_VIC1_BASE_ADDR + oSOFTINTCLEAR)
#define VIC1PROTECTION			(ELFIN_VIC1_BASE_ADDR + oPROTECTION)
#define VIC1SWPRIORITYMASK		(ELFIN_VIC1_BASE_ADDR + oSWPRIORITYMASK)
#define VIC1PRIORITYDAISY		(ELFIN_VIC1_BASE_ADDR + oPRIORITYDAISY)
#define VIC1VECTADDR(X)			(ELFIN_VIC1_BASE_ADDR + oVECTADDR(X))
#define VIC1VECPRIORITY(X)		(ELFIN_VIC1_BASE_ADDR + oVECPRIORITY(X))
#define VIC1VECTADDRESS			(ELFIN_VIC1_BASE_ADDR + oVECTADDRESS)


/* interrupt pending bit */
#define IRQ_VIC0_BASE			0
#define IRQ_VIC1_BASE			32
#define IRQ_VIC0(X)				(IRQ_VIC0_BASE + (X))
#define IRQ_VIC1(X)				(IRQ_VIC1_BASE + (X))
#define IRQ_TOTAL				64

/* VIC0 */
#define IRQ_EINT0_3				IRQ_VIC0(0)
#define IRQ_EINT4_11			IRQ_VIC0(1)
#define IRQ_RTC_TIC				IRQ_VIC0(2)
#define IRQ_CAMIF_C				IRQ_VIC0(3)
#define IRQ_CAMIF_P				IRQ_VIC0(4)
#define IRQ_IIC1				IRQ_VIC0(5)
#define IRQ_IIS					IRQ_VIC0(6)
#define IRQ_UNUSED7				IRQ_VIC0(7)
#define IRQ_3D					IRQ_VIC0(8)
#define IRQ_POST0				IRQ_VIC0(9)
#define IRQ_ROTATOR				IRQ_VIC0(10)
#define IRQ_2D					IRQ_VIC0(11)
#define IRQ_TVENC				IRQ_VIC0(12)
#define IRQ_SCALER				IRQ_VIC0(13)
#define IRQ_BATF				IRQ_VIC0(14)
#define IRQ_JPEG				IRQ_VIC0(15)
#define IRQ_MFC					IRQ_VIC0(16)
#define IRQ_SDMA0				IRQ_VIC0(17)
#define IRQ_SDMA1				IRQ_VIC0(18)
#define IRQ_ARM_DMAERR			IRQ_VIC0(19)
#define IRQ_ARM_DMA				IRQ_VIC0(20)
#define IRQ_ARM_DMAS			IRQ_VIC0(21)
#define IRQ_KEYPAD				IRQ_VIC0(22)
#define IRQ_TIMER0				IRQ_VIC0(23)
#define IRQ_TIMER1				IRQ_VIC0(24)
#define IRQ_TIMER2				IRQ_VIC0(25)
#define IRQ_WDT					IRQ_VIC0(26)
#define IRQ_TIMER3				IRQ_VIC0(27)
#define IRQ_TIMER4				IRQ_VIC0(28)
#define IRQ_LCD_FIFO			IRQ_VIC0(29)
#define IRQ_LCD_VSYNC			IRQ_VIC0(30)
#define IRQ_LCD_SYSTEM			IRQ_VIC0(31)

/* VIC1 */
#define IRQ_EINT12_19			IRQ_VIC1(0)
#define IRQ_EINT20_27			IRQ_VIC1(1)
#define IRQ_PCM0				IRQ_VIC1(2)
#define IRQ_PCM1				IRQ_VIC1(3)
#define IRQ_AC97				IRQ_VIC1(4)
#define IRQ_UART0				IRQ_VIC1(5)
#define IRQ_UART1				IRQ_VIC1(6)
#define IRQ_UART2				IRQ_VIC1(7)
#define IRQ_UART3				IRQ_VIC1(8)
#define IRQ_DMA0				IRQ_VIC1(9)
#define IRQ_DMA1				IRQ_VIC1(10)
#define IRQ_ONENAND0			IRQ_VIC1(11)
#define IRQ_ONENAND1			IRQ_VIC1(12)
#define IRQ_NFC					IRQ_VIC1(13)
#define IRQ_CFC					IRQ_VIC1(14)
#define IRQ_USBH				IRQ_VIC1(15)
#define IRQ_SPI0				IRQ_VIC1(16)
#define IRQ_SPI1				IRQ_VIC1(17)
#define IRQ_IIC0				IRQ_VIC1(18)
#define IRQ_HSItx				IRQ_VIC1(19)
#define IRQ_HSIrx				IRQ_VIC1(20)
#define IRQ_EINT4				IRQ_VIC1(21)
#define IRQ_MSM					IRQ_VIC1(22)
#define IRQ_HOSTIF				IRQ_VIC1(23)
#define IRQ_HSMMC0				IRQ_VIC1(24)
#define IRQ_HSMMC1				IRQ_VIC1(25)
#define IRQ_HSMMC2				IRQ_SPI1	/* shared with SPI1 */
#define IRQ_OTG					IRQ_VIC1(26)
#define IRQ_IRDA				IRQ_VIC1(27)
#define IRQ_RTC_ALARM			IRQ_VIC1(28)
#define IRQ_SEC					IRQ_VIC1(29)
#define IRQ_PENDN				IRQ_VIC1(30)
#define IRQ_TC					IRQ_PENDN
#define IRQ_ADC					IRQ_VIC1(31)

#define IRQ_ALLMSK				(0xFFFFFFFF)


#ifndef __ASSEMBLY__
struct rt_hw_register
{
	rt_uint32_t r0;
	rt_uint32_t r1;
	rt_uint32_t r2;
	rt_uint32_t r3;
	rt_uint32_t r4;
	rt_uint32_t r5;
	rt_uint32_t r6;
	rt_uint32_t r7;
	rt_uint32_t r8;
	rt_uint32_t r9;
	rt_uint32_t r10;
	rt_uint32_t fp;
	rt_uint32_t ip;
	rt_uint32_t sp;
	rt_uint32_t lr;
	rt_uint32_t pc;
	rt_uint32_t cpsr;
	rt_uint32_t ORIG_r0;
};
#endif /* end of __ASSEMBLY__ */

#define CONFIG_STACKSIZE 	512
#define S_FRAME_SIZE 		72

#define S_OLD_R0 			68
#define S_PSR  				64
#define S_PC  				60
#define S_LR  				56
#define S_SP  				52

#define S_IP  					48
#define S_FP  				44
#define S_R10  				40
#define S_R9  				36
#define S_R8  				32
#define S_R7  				28
#define S_R6  				24
#define S_R5  				20
#define S_R4  				16
#define S_R3  				12
#define S_R2  				8
#define S_R1  				4
#define S_R0 				0


/*****************************/
/* CPU Mode                  */
/*****************************/
#define USERMODE				0x10
#define FIQMODE					0x11
#define IRQMODE					0x12
#define SVCMODE					0x13
#define ABORTMODE				0x17
#define UNDEFMODE				0x1b
#define MODEMASK				0x1f
#define NOINT					0xc0

/*****************************/
/* CP15 Mode Bit Definition  */
/*****************************/
#define R1_iA					(1<<31)
#define R1_nF					(1<<30)
#define R1_VE					(1<<24)
#define R1_I					(1<<12)
#define R1_BP					(1<<11)		/* Z bit */
#define R1_C					(1<<2)
#define R1_A					(1<<1)
#define R1_M					(1<<0)

#define RAM_BASE				0x50000000	/*Start address of RAM		*/
#define ROM_BASE				0x00000000	/*Start address of Flash	*/


#ifdef __cplusplus
}
#endif

#endif /*__S3C6410_H__*/
