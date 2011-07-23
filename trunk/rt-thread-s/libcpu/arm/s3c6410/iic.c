/*
 * File      : cpu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-05-29     swkyer       first version
 */
#include <rtthread.h>
#include "s3c6410.h"
#include "tiny6410.h"
#include "clock.h"
#include "iic.h"



#define	I2C_WRITE				0
#define I2C_READ				1

#define I2C_OK					0
#define I2C_NOK					1
#define I2C_NACK				2
#define I2C_NOK_LA				3			/* Lost arbitration */
#define I2C_NOK_TOUT			4			/* time out */

#define I2CSTAT_BSY				0x20		/* Busy bit */
#define I2CSTAT_NACK			0x01		/* Nack bit */
#define I2CCON_IRPND			0x10		/* Interrupt pending bit */
#define I2C_MODE_MT				0xC0		/* Master Transmit Mode */
#define I2C_MODE_MR				0x80		/* Master Receive Mode */
#define I2C_START_STOP			0x20		/* START / STOP */
#define I2C_TXRX_ENA			0x10		/* I2C Tx/Rx enable */

#define I2C_TIMEOUT_TICKS 		RT_TICK_PER_SECOND			/* 1 second */


static rt_uint32_t wait_for_xfer(rt_iic_t *pi2c)
{
	rt_uint32_t i, status;

	i = I2C_TIMEOUT_TICKS;
	status = s3c_readl(pi2c->regbase + oIICCON);
	while ((i > 0) && !(status & I2CCON_IRPND))
	{
		rt_thread_delay(1);
		status = s3c_readl(pi2c->regbase + oIICCON);
		i--;
	}

	return (status & I2CCON_IRPND) ? I2C_OK : I2C_NOK_TOUT;
}

static inline int is_ack(rt_iic_t *pi2c)
{
	return (!(s3c_readl(pi2c->regbase + oIICSTAT) & I2CSTAT_NACK));
}

static void read_write_byte(rt_iic_t *pi2c)
{
	rt_uint32_t regv;

	regv = s3c_readl(pi2c->regbase + oIICCON);
	regv &= ~I2CCON_IRPND;
	s3c_writel(regv, pi2c->regbase + oIICCON);
}


/*
 * cmd_type is 0 for write, 1 for read.
 *
 * addr_len can take any value from 0-255, it is only limited
 * by the char, we could make it larger if needed. If it is
 * 0 we skip the address write cycle.
 */
static int i2c_transfer(rt_iic_t *pi2c, rt_uint8_t cmd_type, rt_uint8_t chip,
					rt_uint8_t addr[], rt_uint8_t addr_len,
					rt_uint8_t data[], rt_uint16_t data_len)
{
	rt_uint32_t i, status, result;

	if (data == 0 || data_len == 0)
	{
		/*Don't support data transfer of no length or to address 0 */
		rt_kprintf("IIC[%d] i2c_transfer: bad call\n", pi2c->index);
		return I2C_NOK;
	}

	/* Check I2C bus idle */
	i = I2C_TIMEOUT_TICKS;
	status = s3c_readl(pi2c->regbase + oIICSTAT);
	while ((i > 0) && (status & I2CSTAT_BSY))
	{
		rt_thread_delay(1);
		status = s3c_readl(pi2c->regbase + oIICSTAT);
		i--;
	}

	if (status & I2CSTAT_BSY)
		return I2C_NOK_TOUT;

	status = s3c_readl(pi2c->regbase + oIICCON);
	status |= 0x80;
	s3c_writel(status, pi2c->regbase + oIICCON);
	result = I2C_OK;

	switch (cmd_type)
	{
	case I2C_WRITE:
		if (addr && addr_len)
		{
			s3c_writel(chip, pi2c->regbase + oIICDS);
			/* send START */
			status = I2C_MODE_MT | I2C_TXRX_ENA | I2C_START_STOP;
			s3c_writel(status, pi2c->regbase + oIICSTAT);
			i = 0;
			while ((i < addr_len) && (result == I2C_OK))
			{
				result = wait_for_xfer(pi2c);
				s3c_writel(addr[i], pi2c->regbase + oIICDS);
				read_write_byte(pi2c);
				i++;
			}
			i = 0;
			while ((i < data_len) && (result == I2C_OK))
			{
				result = wait_for_xfer(pi2c);
				s3c_writel(data[i], pi2c->regbase + oIICDS);
				read_write_byte(pi2c);
				i++;
			}
		}
		else
		{
			s3c_writel(chip, pi2c->regbase + oIICDS);
			/* send START */
			status = I2C_MODE_MT | I2C_TXRX_ENA | I2C_START_STOP;
			s3c_writel(status, pi2c->regbase + oIICSTAT);
			i = 0;
			while ((i < data_len) && (result = I2C_OK))
			{
				result = wait_for_xfer(pi2c);
				s3c_writel(data[i], pi2c->regbase + oIICDS);
				read_write_byte(pi2c);
				i++;
			}
		}

		if (result == I2C_OK)
			result = wait_for_xfer(pi2c);

		/* send STOP */
		status = I2C_MODE_MR | I2C_TXRX_ENA;
		s3c_writel(status, pi2c->regbase + oIICSTAT);
		read_write_byte(pi2c);
		break;

	case I2C_READ:
		if (addr && addr_len)
		{
			status = I2C_MODE_MT | I2C_TXRX_ENA;
			s3c_writel(status, pi2c->regbase + oIICSTAT);
			s3c_writel(chip, pi2c->regbase + oIICDS);

			/* send START */
			status = s3c_readl(pi2c->regbase + oIICSTAT);
			status |= I2C_START_STOP;
			s3c_writel(status, pi2c->regbase + oIICSTAT);
			result = wait_for_xfer(pi2c);
			if (is_ack(pi2c))
			{
				i = 0;
				while ((i < addr_len) && (result == I2C_OK))
				{
					s3c_writel(addr[i], pi2c->regbase + oIICDS);
					read_write_byte(pi2c);
					result = wait_for_xfer(pi2c);
					i++;
				}

				s3c_writel(chip, pi2c->regbase + oIICDS);
				/* resend START */
				status = I2C_MODE_MR | I2C_TXRX_ENA | I2C_START_STOP;
				s3c_writel(status, pi2c->regbase + oIICSTAT);
				read_write_byte(pi2c);
				result = wait_for_xfer(pi2c);
				i = 0;
				while ((i < data_len) && (result == I2C_OK))
				{
					/* disable ACK for final READ */
					if (i == data_len - 1)
					{
						status = s3c_readl(pi2c->regbase + oIICCON);
						status &= ~0x80;
						s3c_writel(status, pi2c->regbase + oIICCON);
					}
					read_write_byte(pi2c);
					result = wait_for_xfer(pi2c);
					data[i] = s3c_readl(pi2c->regbase + oIICDS);
					i++;
				}
			}
			else
			{
				result = I2C_NACK;
			}
		}
		else
		{
			status = I2C_MODE_MR | I2C_TXRX_ENA;
			s3c_writel(status, pi2c->regbase + oIICSTAT);
			s3c_writel(chip, pi2c->regbase + oIICDS);
			/* send START */
			status = s3c_readl(pi2c->regbase + oIICSTAT);
			status |= I2C_START_STOP;
			s3c_writel(status, pi2c->regbase + oIICSTAT);
			result = wait_for_xfer(pi2c);

			if (is_ack(pi2c))
			{
				i = 0;
				while ((i < data_len) && (result == I2C_OK))
				{
					/* disable ACK for final READ */
					if (i == data_len - 1)
					{
						status = s3c_readl(pi2c->regbase + oIICCON);
						status &= ~0x80;
						s3c_writel(status, pi2c->regbase + oIICCON);
					}
					read_write_byte(pi2c);
					result = wait_for_xfer(pi2c);
					data[i] = s3c_readl(pi2c->regbase + oIICDS);
					i++;
				}
			}
			else
			{
				result = I2C_NACK;
			}
		}

		/* send STOP */
		status = I2C_MODE_MR | I2C_TXRX_ENA;
		s3c_writel(status, pi2c->regbase + oIICSTAT);
		read_write_byte(pi2c);
		break;

	default:
		rt_kprintf("IIC[%d] i2c_transfer: bad call\n", pi2c->index);
		result = I2C_NOK;
		break;
	}

	return (result);
}


void rt_hw_i2c_init(rt_iic_t *pi2c, rt_uint32_t speed, rt_uint32_t slaveadd)
{
	rt_uint32_t freq, pres = 16, div;
	rt_uint32_t i, status;

	/* wait for some time to give previous transfer a chance to finish */
	i = I2C_TIMEOUT_TICKS;
	status = s3c_readl(pi2c->regbase + oIICSTAT);
	while ((i > 0) && (status & I2CSTAT_BSY))
	{
		rt_thread_delay(1);
		status = s3c_readl(pi2c->regbase + oIICSTAT);
		i--;
	}

	/* calculate prescaler and divisor values */
	freq = s3c_get_pclk();
	if (((freq>>4)/speed)>0xf)
	{
		pres = 1;
		div = (freq>>9)/speed;		//	PCLK/512/freq
	}
	else
	{
		pres = 0;
		div = (freq>>4)/speed;		//	PCLK/16/freq
	}

	/* set prescaler, divisor according to freq, also set ACKGEN, IRQ */
	status = s3c_readl(pi2c->regbase + oIICCON);
	status = (pres<<6) | (1<<5) | (div&0xf);
	s3c_writel(status, pi2c->regbase + oIICCON);

	/* init to SLAVE REVEIVE and set slaveaddr */
	s3c_writel(0, pi2c->regbase + oIICSTAT);
	s3c_writel(slaveadd, pi2c->regbase + oIICADD);

	/* program Master Transmit (and implicit STOP) */
	status = s3c_readl(pi2c->regbase + oIICSTAT);
	status = I2C_MODE_MT | I2C_TXRX_ENA;
	s3c_writel(status, pi2c->regbase + oIICSTAT);
}

int rt_hw_i2c_probe(rt_iic_t *pi2c, rt_uint8_t chip)
{
	rt_uint8_t buf[1];

	buf[0] = 0;

	/*
	 * What is needed is to send the chip address and verify that the
	 * address was <ACK>ed (i.e. there was a chip at that address which
	 * drove the data line low).
	 */
	return (i2c_transfer(pi2c, I2C_READ, chip << 1, 0, 0, buf, 1) != I2C_OK);
}

int rt_hw_i2c_read(rt_iic_t *pi2c, rt_uint8_t chip, rt_uint32_t addr, int alen, rt_uint8_t *buffer, int len)
{
	rt_uint8_t xaddr[4];
	int ret;

	if (alen > 4)
	{
		rt_kprintf("IIC[%d] read: addr len %d not supported\n", pi2c->index, alen);
		return 1;
	}

	if (alen > 0)
	{
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
	}

	if ((ret = i2c_transfer(pi2c, I2C_READ, chip << 1, &xaddr[4 - alen], alen, buffer, len)) != 0)
	{
		rt_kprintf("IIC[%d] read: failed %d\n", pi2c->index, ret);
		return 1;
	}
	return 0;
}

int rt_hw_i2c_write(rt_iic_t *pi2c, rt_uint8_t chip, rt_uint32_t addr, int alen, rt_uint8_t *buffer, int len)
{
	rt_uint8_t xaddr[4];

	if (alen > 4)
	{
		rt_kprintf("IIC[%d] write: addr len %d not supported\n", pi2c->index, alen);
		return 1;
	}

	if (alen > 0)
	{
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
	}

	return (i2c_transfer(pi2c, I2C_WRITE, chip << 1, &xaddr[4 - alen], alen, buffer, len) != 0);
}

