/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-24     Bernard      first implementation
 * 2006-05-05     Bernard      add DATA_COUNT definition
 * 2006-10-05     Alsor.Z      for s3c2410x porting
 * 2007-11-20     Yi.Qiu	   add lcd,touch,console
 * 2011-05-11     swkyer       for tiny6410
 */

#include <rtthread.h>
#include <rthw.h>
#include "clock.h"
#include "led.h"
#include "board.h"

/**
 * @addtogroup tiny6410
 */
/*@{*/

extern void rt_hw_lcd_bl_init(void);
extern void rt_hw_mmu_init(void);

#define UART0	((struct uartport *)ELFIN_UART0_BASE)
struct serial_int_rx uart0_int_rx;
struct serial_device uart0 =
{
	UART0,
	&uart0_int_rx,
	RT_NULL
};
struct rt_device uart0_device;

#define UART2	((struct uartport *)ELFIN_UART1_BASE)
struct serial_int_rx uart2_int_rx;
struct serial_device uart2 =
{
	UART2,
	&uart2_int_rx,
	RT_NULL
};
struct rt_device uart2_device;

/**
 * This function will handle rtos timer
 */
static void rt_timer_handler(int vector)
{
	register rt_base_t regv;

	/* clear timer4 interrupt status bit */
	regv = s3c_readl(TINT_CSTAT);
	regv |= (1 << 9);
	s3c_writel(regv, TINT_CSTAT);

	rt_tick_increase();
}

/**
 * This function will handle serial
 */
static void rt_serial0_handler(int vector)
{

	rt_hw_serial_isr(&uart0_device);

	//SUBSRCPND |= BIT_SUB_RXD0;
}

/**
 * This function will handle serial
 */
static void rt_serial2_handler(int vector)
{

	rt_hw_serial_isr(&uart2_device);

	//SUBSRCPND |= BIT_SUB_RXD2;
}

/**
 * This function will handle init uart
 */
static void rt_hw_uart_init(void)
{
	int i;
	rt_uint32_t pclk;

	pclk = s3c_get_pclk();
	/* UART0 port configure */
	s3c_writel(s3c_readl(GPHCON0) | 0xAA, GPHCON0);
	/* PULLUP is disable */
	s3c_writel(s3c_readl(GPHPUD) | 0xF, GPHPUD);

	/* FIFO enable, Tx/Rx FIFO clear */
	uart0.uart_device->ufcon = 0x0;
	/* disable the flow control */
	uart0.uart_device->umcon = 0x0;
	/* Normal,No parity,1 stop,8 bit */
	uart0.uart_device->ulcon = 0x3;
	/*
	 * tx=level,rx=edge,disable timeout int.,enable rx error int.,
	 * normal,interrupt or polling
	 */
	uart0.uart_device->ucon = 0x245;
	/* Set uart0 bps */
	uart0.uart_device->ubrd = (rt_int32_t)(pclk / (BPS * 16)) - 1;
	/* output PCLK to UART0/1, PWMTIMER */
	//CLKCON |= 0x0D00;

	/* FIFO enable, Tx/Rx FIFO clear */
	uart2.uart_device->ufcon = 0x0;
	/* disable the flow control */
	uart2.uart_device->umcon = 0x0;
	/* Normal,No parity,1 stop,8 bit */
	uart2.uart_device->ulcon = 0x3;
	/*
	 * tx=level,rx=edge,disable timeout int.,enable rx error int.,
	 * normal,interrupt or polling
	 */
	uart2.uart_device->ucon = 0x245;
	/* Set uart0 bps */
	uart2.uart_device->ubrd = (rt_int32_t)(pclk / (BPS * 16)) - 1;
	
	for (i = 0; i < 100; i++);

	/* install uart0/uart2 isr */
	rt_hw_interrupt_install(IRQ_UART0, rt_serial0_handler, RT_NULL);
	rt_hw_interrupt_umask(IRQ_UART0);

	rt_hw_interrupt_install(IRQ_UART2, rt_serial2_handler, RT_NULL);
	rt_hw_interrupt_umask(IRQ_UART2);
}

/**
 * This function will init timer4 for system ticks
 */
static void rt_hw_timer_init()
{
	rt_uint32_t pclk;
	register rt_base_t regv;

	pclk = s3c_get_pclk();

	/* timer4, pre = 15+1 */
	regv = s3c_readl(TCFG0);
	regv &= (0xff << 8);
	regv |= (15 << 8);
	s3c_writel(regv, TCFG0);
	/* all are interrupt mode,set Timer 4 MUX 1/4 */
	regv = s3c_readl(TCFG1);
	regv &= ~(0x0f << 16);
	regv |= (2 << 16);
	s3c_writel(regv, TCFG1);

	regv = (rt_int32_t)(pclk / (4 * 16 * RT_TICK_PER_SECOND)) - 1;
	s3c_writel(regv, TCNTB4);
	/* manual update */
	regv = s3c_readl(TCON);
	regv &= ~(0x0f << 20);
	regv |= (0x02 << 20);
	s3c_writel(regv, TCON);
	/* install interrupt handler */
	rt_hw_interrupt_install(IRQ_TIMER4, rt_timer_handler, RT_NULL);
	rt_hw_interrupt_umask(IRQ_TIMER4);

	/* enable timer4 interrupt */
	regv = s3c_readl(TINT_CSTAT);
	regv |= (1 << 4);
	s3c_writel(regv, TINT_CSTAT);

    /* start timer4, reload */
	regv = s3c_readl(TCON);
	regv &= ~(0x0f << 20);
	regv |= (0x05 << 20);
	s3c_writel(regv, TCON);
}

/**
 * This function will init s3ceb2410 board
 */
void rt_hw_board_init()
{
	/* initialize the system clock */
	rt_hw_clock_init();

	/* initialize led port */
	rt_hw_led_init();

	/* initialize lcd backlight */
	rt_hw_lcd_bl_init();

	/* initialize uart */
	rt_hw_uart_init();

	/* initialize timer4 */
	rt_hw_timer_init();

	/* initialize mmu */
	rt_hw_mmu_init();
}

/*@}*/
