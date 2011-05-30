/*
 * main.c
 *
 *  Created on: 2011-5-11
 *      Author: sangwei
 */
#include "s3c6410.h"
#include "tiny6410.h"
#include "serial.h"


#define globalBlockSizeHide		*((volatile unsigned int*)(0x0C004000-0x4))

/**
* This Function copies SD/MMC Card Data to memory.
* Always use EPLL source clock.
* @param channel : HSMMC Controller channel number ( Not support. Depend on GPN15, GPN14 and GPN13 )
* @param StartBlkAddress : Source card(SD/MMC) Address.(It must block address.)
* @param blockSize : Number of blocks to copy.
* @param memoryPtr : Buffer to copy from.
* @param with_init : reinitialize or not
* @return bool(unsigend char) - Success or failure.
*/
#define CopyMMCtoMem(z, a, b, c, e) \
	(((int (*)(int, unsigned int, unsigned short, unsigned int *, int)) \
	(*((unsigned int *)0x0C004008)))(z, a, b, c, e))


static void led_init(void);
static void led_display(unsigned int data);
static int ddr_test(void);
static void dead(unsigned int count);

extern void mmu_init(void);
extern void rt_hw_lcd_bl_init(void);


int main()
{
	int retv;
	unsigned int uintv;

    led_init();
    led_display(0x0f);
    serial_init(CONFIG_BAUDRATE);

    retv = ddr_test();
    if (retv < 0)
    {
    	serial_puts("ddr test faild.\n");
    	dead(0x4000000);
    }
    serial_puts("1. ddr init/test successful.\n");

    uintv = *(volatile unsigned int *)(0x0C003FF8);
    if (uintv & 0x04)
    	serial_puts("2. SD card detected, Block size: 0x");
    if (uintv & 0x02)
    	serial_puts("2. MMC card detected, Block size: 0x");
    uintv = globalBlockSizeHide;
    print_dword(uintv);
    uintv = *(volatile unsigned int *)(0x0C003FEC);
    serial_puts("   SD/MMC Controller Base Address: 0x");
    print_dword(uintv);

    serial_puts("3. Copy BL2 to DDR.\n");
    serial_puts("   From SD/MMC block: 0x");
    uintv = globalBlockSizeHide;
    uintv -= BL2_BLOCK_OFFSET;
    print_dword(uintv);
    serial_puts("   To DDR Address (Physical): 0x");
    print_dword(BL2_ADDR+MEMORY_BASE_ADDRESS);
    serial_puts("   Size: 0x");
    print_dword(BL2_SIZE);
    retv = CopyMMCtoMem(1, uintv, (unsigned short)(BL2_SIZE/BLOCK_SIZE),
    					(unsigned int *)(BL2_ADDR+MEMORY_BASE_ADDRESS), 0);
    serial_puts("   Done, result: ");
    print_dword(retv);

    rt_hw_lcd_bl_init();

    mmu_init();
    serial_puts("4. Jump to BL2, Virtual Address: 0x");
    print_dword(BL2_ADDR);
    serial_puts("\n\n");

    return 0;
}

static int ddr_test(void)
{
	int i;
	unsigned int val;

	for (i=0; i<0x1000; i++)
	{
		s3c_writel(0x55aaaa55, (MEMORY_BASE_ADDRESS + i*0x100));
		val = s3c_readl(MEMORY_BASE_ADDRESS + i*0x100);
		if (val != 0x55aaaa55)
			return -1;

		s3c_writel(0xaa5555aa, (MEMORY_BASE_ADDRESS + i*0x100));
		val = s3c_readl(MEMORY_BASE_ADDRESS + i*0x100);
		if (val != 0xaa5555aa)
			return -1;
	}

	return i;
}

static void led_init(void)
{
	unsigned int regv;

	regv = s3c_readl(GPKCON0);
	regv = (regv & ~(0xffffU<<16))|(0x1111U<<16);
	s3c_writel(regv, GPKCON0);

	regv = s3c_readl(GPKPUD);
	regv  = (regv & ~(0xffU << 8))|(0x00U<<8);
	s3c_writel(regv, GPKPUD);
}

static void led_display(unsigned int data)
{
	unsigned int regv;

	regv = s3c_readl(GPKDAT);
	regv = (regv & ~(0xf<<4)) | ((data & 0xf)<<4);
    s3c_writel(regv, GPKDAT);
}

static void led_delay(unsigned int count)
{
	volatile unsigned int k;
	for (k = 0; k < count; k++);
}

static void dead(unsigned int count)
{
    for (;;)
    {
    	led_display(0x9); // 1001
    	led_delay(count);
    	led_display(0x6); // 0110
    	led_delay(count);
    }
}
