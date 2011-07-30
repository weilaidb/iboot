/*
 * File      : dma.c
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
#include "dma.h"


/* dma channel state information */
struct s3c_dma_chan s3c6410_chans[S3C_DMA_CHANNELS];
struct s3c_dma_chan *s3c_dma_chan_map[DMACH_MAX];

/* s3c_dma_lookup_channel
 *
 * change the dma channel number given into a real dma channel id
*/
struct s3c_dma_chan *s3c_dma_lookup_channel(unsigned int channel)
{
	if (channel & DMACH_LOW_LEVEL)
		return &s3c6410_chans[channel & ~DMACH_LOW_LEVEL];
	else
		return s3c_dma_chan_map[channel];
}

/* do we need to protect the settings of the fields from
 * irq?
*/
int s3c_dma_set_opfn(rt_uint32_t channel, s3c_dma_opfn_t rtn)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);

	if (chan == RT_NULL)
		return -1;

	rt_kprintf("%s: chan=%p, op rtn=%p\n", __func__, chan, rtn);
	chan->op_fn = rtn;
	return 0;
}

int s3c_dma_set_buffdone_fn(rt_uint32_t channel, s3c_dma_cbfn_t rtn)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);

	if (chan == RT_NULL)
		return -1;

	rt_kprintf("%s: chan=%p, callback rtn=%p\n", __func__, chan, rtn);
	chan->callback_fn = rtn;
	return 0;
}

int s3c_dma_setflags(rt_uint32_t channel, rt_uint32_t flags)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);

	if (chan == RT_NULL)
		return -1;

	chan->flags = flags;
	return 0;
}


static void dbg_showchan(struct s3c_dma_chan *chan)
{
	rt_kprintf("DMA%d: %08x->%08x L %08x C %08x,%08x S %08x\n",
		 chan->number,
		 s3c_readl(chan->regs + PL080_CH_SRC_ADDR),
		 s3c_readl(chan->regs + PL080_CH_DST_ADDR),
		 s3c_readl(chan->regs + PL080_CH_LLI),
		 s3c_readl(chan->regs + PL080_CH_CONTROL),
		 s3c_readl(chan->regs + PL080S_CH_CONTROL2),
		 s3c_readl(chan->regs + PL080S_CH_CONFIG));
}

static void show_lli(struct pl080s_lli *lli)
{
	rt_kprintf("LLI[%p] %08x->%08x, NL %08x C %08x,%08x\n",
		 lli, lli->src_addr, lli->dst_addr, lli->next_lli,
		 lli->control0, lli->control1);
}

static void dbg_showbuffs(struct s3c_dma_chan *chan)
{
	struct s3c_dma_buff *ptr;
	struct s3c_dma_buff *end;

	rt_kprintf("DMA%d: buffs next %p, curr %p, end %p\n",
		 chan->number, chan->next, chan->curr, chan->end);

	ptr = chan->next;
	end = chan->end;

	for (; ptr != RT_NULL; ptr = ptr->next)
	{
		rt_kprintf("DMA%d: %08x ", chan->number, ptr->lli_dma);
		show_lli(ptr->lli);
	}
}

static struct s3c_dma_chan *s3c64xx_dma_map_channel(rt_uint32_t channel)
{
	struct s3c_dma_chan *chan;
	rt_uint32_t start, offs;

	start = 0;
	if (channel >= DMACH_PCM1_TX)
		start = 8;

	for (offs = 0; offs < 8; offs++)
	{
		chan = &s3c6410_chans[start + offs];
		if (!chan->in_use)
			goto found;
	}

	return RT_NULL;

found:
	s3c_dma_chan_map[channel] = chan;
	return chan;
}

int s3c_dma_config(rt_uint32_t channel, int xferunit)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);

	if (chan == RT_NULL)
		return -1;

	switch (xferunit)
	{
	case 1:
		chan->hw_width = 0;
		break;
	case 2:
		chan->hw_width = 1;
		break;
	case 4:
		chan->hw_width = 2;
		break;
	default:
		rt_kprintf("%s: illegal width %d\n", __func__, xferunit);
		return -2;
	}

	return 0;
}

static void s3c64xx_dma_fill_lli(struct s3c_dma_chan *chan,
				 struct pl080s_lli *lli, dma_addr_t data, int size)
{
	dma_addr_t src, dst;
	rt_uint32_t control0, control1;

	switch (chan->source)
	{
	case S3C_DMASRC_HW:
		src = chan->dev_addr;
		dst = data;
		control0 = PL080_CONTROL_SRC_AHB2;
		control0 |= PL080_CONTROL_DST_INCR;
		break;

	case S3C_DMASRC_MEM:
		src = data;
		dst = chan->dev_addr;
		control0 = PL080_CONTROL_DST_AHB2;
		control0 |= PL080_CONTROL_SRC_INCR;
		break;
	default:
		rt_kprintf("there is bug here, %s--%d\n", __FILE__, __LINE__);
		return;
	}

	/* note, we do not currently setup any of the burst controls */

	control1 = size >> chan->hw_width;	/* size in no of xfers */
	control0 |= PL080_CONTROL_PROT_SYS;	/* always in priv. mode */
	control0 |= PL080_CONTROL_TC_IRQ_EN;	/* always fire IRQ */
	control0 |= (u32)chan->hw_width << PL080_CONTROL_DWIDTH_SHIFT;
	control0 |= (u32)chan->hw_width << PL080_CONTROL_SWIDTH_SHIFT;

	lli->src_addr = src;
	lli->dst_addr = dst;
	lli->next_lli = 0;
	lli->control0 = control0;
	lli->control1 = control1;
}

static void s3c64xx_lli_to_regs(struct s3c_dma_chan *chan, struct pl080s_lli *lli)
{
	void *regs = chan->regs;

	rt_kprintf("%s: LLI %p => regs\n", __func__, lli);
	show_lli(lli);

	s3c_writel(lli->src_addr, regs + PL080_CH_SRC_ADDR);
	s3c_writel(lli->dst_addr, regs + PL080_CH_DST_ADDR);
	s3c_writel(lli->next_lli, regs + PL080_CH_LLI);
	s3c_writel(lli->control0, regs + PL080_CH_CONTROL);
	s3c_writel(lli->control1, regs + PL080S_CH_CONTROL2);
}

static int s3c64xx_dma_start(struct s3c_dma_chan *chan)
{
	struct s3c64xx_dmac *dmac = chan->dmac;
	rt_uint32_t config;
	rt_uint32_t bit = chan->bit;

	dbg_showchan(chan);

	rt_kprintf("%s: clearing interrupts\n", __func__);

	/* clear interrupts */
	s3c_writel(bit, dmac->regs + PL080_TC_CLEAR);
	s3c_writel(bit, dmac->regs + PL080_ERR_CLEAR);

	rt_kprintf("%s: starting channel\n", __func__);

	config = readl(chan->regs + PL080S_CH_CONFIG);
	config |= PL080_CONFIG_ENABLE;
	config &= ~PL080_CONFIG_HALT;

	rt_kprintf("%s: writing config %08x\n", __func__, config);
	s3c_writel(config, chan->regs + PL080S_CH_CONFIG);

	return 0;
}

static int s3c64xx_dma_stop(struct s3c_dma_chan *chan)
{
	rt_uint32_t config;
	int timeout;

	rt_kprintf("%s: stopping channel\n", __func__);

	dbg_showchan(chan);

	config = s3c_readl(chan->regs + PL080S_CH_CONFIG);
	config |= PL080_CONFIG_HALT;
	s3c_writel(config, chan->regs + PL080S_CH_CONFIG);

	timeout = 1000;
	do {
		config = s3c_readl(chan->regs + PL080S_CH_CONFIG);
		rt_kprintf("%s: %d - config %08x\n", __func__, timeout, config);
		if (config & PL080_CONFIG_ACTIVE)
			udelay(10);
		else
			break;
	} while (--timeout > 0);

	if (config & PL080_CONFIG_ACTIVE)
	{
		rt_kprintf("%s: channel still active\n", __func__);
		return -1;
	}

	config = s3c_readl(chan->regs + PL080S_CH_CONFIG);
	config &= ~PL080_CONFIG_ENABLE;
	s3c_writel(config, chan->regs + PL080S_CH_CONFIG);

	return 0;
}

static inline void s3c64xx_dma_bufffdone(struct s3c_dma_chan *chan,
					 struct s3c_dma_buff *buf, enum s3c_dma_buffresult result)
{
	if (chan->callback_fn != RT_NULL)
		(chan->callback_fn)(chan, buf->pw, 0, result);
}

static void s3c64xx_dma_freebuff(struct s3c_dma_buff *buff)
{
	//dma_pool_free(dma_pool, buff->lli, buff->lli_dma);
	rt_free(buff);
}

static int s3c64xx_dma_flush(struct s3c_dma_chan *chan)
{
	struct s3c_dma_buff *buff, *next;
	rt_uint32_t config;

	dbg_showchan(chan);

	rt_kprintf("%s: flushing channel\n", __func__);

	config = s3c_readl(chan->regs + PL080S_CH_CONFIG);
	config &= ~PL080_CONFIG_ENABLE;
	s3c_writel(config, chan->regs + PL080S_CH_CONFIG);

	/* dump all the buffers associated with this channel */

	for (buff = chan->curr; buff != RT_NULL; buff = next)
	{
		next = buff->next;
		rt_kprintf("%s: buff %p (next %p)\n", __func__, buff, buff->next);

		s3c64xx_dma_bufffdone(chan, buff, S3C_RES_ABORT);
		s3c64xx_dma_freebuff(buff);
	}

	chan->curr = chan->next = chan->end = RT_NULL;

	return 0;
}

int s3c_dma_ctrl(rt_uint32_t channel, enum s3c_chan_op op)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);

	if (!chan)
		return -1;

	switch (op)
	{
	case S3C_DMAOP_START:
		return s3c64xx_dma_start(chan);

	case S3C_DMAOP_STOP:
		return s3c64xx_dma_stop(chan);

	case S3C_DMAOP_FLUSH:
		return s3c64xx_dma_flush(chan);

	/* believe PAUSE/RESUME are no-ops */
	case S3C_DMAOP_PAUSE:
	case S3C_DMAOP_RESUME:
	case S3C_DMAOP_STARTED:
	case S3C_DMAOP_TIMEOUT:
		return 0;
	}

	return -2;
}

/* s3c_dma_enque
 *
 */
int s3c_dma_enqueue(rt_uint32_t channel, void *id, dma_addr_t data, int size)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);
	struct s3c_dma_buff *next;
	struct s3c_dma_buff *buff;
	struct pl080s_lli *lli;
	rt_ubase_t flags;
	int ret;

	if (!chan)
		return -1;

	buff = rt_malloc(sizeof(struct s3c_dma_buff));
	if (!buff)
	{
		rt_kprintf("%s: no memory for buffer\n", __func__);
		return -2;
	}
	rt_memset(buff, 0, sizeof(struct s3c_dma_buff));

/*	lli = dma_pool_alloc(dma_pool, GFP_ATOMIC, &buff->lli_dma);
	if (!lli) {
		printk("%s: no memory for lli\n", __func__);
		ret = -ENOMEM;
		goto err_buff;
	}
*/
	rt_kprintf("%s: buff %p, dp %08x lli (%p, %08x) %d\n",
		 __func__, buff, data, lli, (u32)buff->lli_dma, size);

	buff->lli = lli;
	buff->pw = id;

	s3c64xx_dma_fill_lli(chan, lli, data, size);

	flags = rt_hw_interrupt_disable();

	if ((next = chan->next) != RT_NULL)
	{
		struct s3c_dma_buff *end = chan->end;
		struct pl080s_lli *endlli = end->lli;

		rt_kprintf("enquing onto channel\n");

		end->next = buff;
		endlli->next_lli = buff->lli_dma;

		if (chan->flags & S3C_DMAF_CIRCULAR)
		{
			struct s3c_dma_buff *curr = chan->curr;
			lli->next_lli = curr->lli_dma;
		}

		if (next == chan->curr)
		{
			s3c_writel(buff->lli_dma, chan->regs + PL080_CH_LLI);
			chan->next = buff;
		}

		show_lli(endlli);
		chan->end = buff;
	}
	else
	{
		rt_kprintf("enquing onto empty channel\n");

		chan->curr = buff;
		chan->next = buff;
		chan->end = buff;

		s3c64xx_lli_to_regs(chan, lli);
	}

	rt_hw_interrupt_enable(flags);

	show_lli(lli);

	dbg_showchan(chan);
	dbg_showbuffs(chan);
	return 0;

err_buff:
	kfree(buff);
	return ret;
}

int s3c_dma_devconfig(rt_uint32_t channel,
			  enum s3c_dmasrc source, rt_ubase_t devaddr)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);
	rt_uint32_t peripheral;
	rt_uint32_t config = 0;

	rt_kprintf("%s: channel %d, source %d, dev %08lx, chan %p\n",
		 __func__, channel, source, devaddr, chan);

	if (!chan)
		return -1;

	peripheral = (chan->peripheral & 0xf);
	chan->source = source;
	chan->dev_addr = devaddr;

	rt_kprintf("%s: peripheral %d\n", __func__, peripheral);

	switch (source)
	{
	case S3C_DMASRC_HW:
		config = 2 << PL080_CONFIG_FLOW_CONTROL_SHIFT;
		config |= peripheral << PL080_CONFIG_SRC_SEL_SHIFT;
		break;
	case S3C_DMASRC_MEM:
		config = 1 << PL080_CONFIG_FLOW_CONTROL_SHIFT;
		config |= peripheral << PL080_CONFIG_DST_SEL_SHIFT;
		break;
	default:
		rt_kprintf("%s: bad source\n", __func__);
		return -2;
	}

	/* allow TC and ERR interrupts */
	config |= PL080_CONFIG_TC_IRQ_MASK;
	config |= PL080_CONFIG_ERR_IRQ_MASK;

	rt_kprintf("%s: config %08x\n", __func__, config);

	s3c_writel(config, chan->regs + PL080S_CH_CONFIG);

	return 0;
}

int s3c_dma_getposition(rt_uint32_t channel, dma_addr_t *src, dma_addr_t *dst)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);

	if (!chan)
		return -1;

	if (src != RT_NULL)
		*src = s3c_readl(chan->regs + PL080_CH_SRC_ADDR);

	if (dst != RT_NULL)
		*dst = s3c_readl(chan->regs + PL080_CH_DST_ADDR);

	return 0;
}

/* s3c_request_dma
 *
 * get control of an dma channel
*/
int s3c_dma_request(rt_uint32_t channel, void *dev)
{
	struct s3c_dma_chan *chan;
	rt_ubase_t flags;

	rt_kprintf("dma%d: s3c2410_request_dma: dev=%p\n", channel, dev);

	flags = rt_hw_interrupt_disable();

	chan = s3c64xx_dma_map_channel(channel);
	if (chan == RT_NULL)
	{
		rt_hw_interrupt_enable(flags);
		return -1;
	}

	dbg_showchan(chan);

	//chan->client = client;
	chan->in_use = 1;
	chan->peripheral = channel;

	rt_hw_interrupt_enable(flags);

	/* need to setup */

	rt_kprintf("%s: channel initialised, %p\n", __func__, chan);

	return chan->number | DMACH_LOW_LEVEL;
}

/* s3c_dma_free
 *
 * release the given channel back to the system, will stop and flush
 * any outstanding transfers, and ensure the channel is ready for the
 * next claimant.
 *
 * Note, although a warning is currently printed if the freeing client
 * info is not the same as the registrant's client info, the free is still
 * allowed to go through.
*/
int s3c_dma_free(rt_uint32_t channel, struct s3c_dma_client *client)
{
	struct s3c_dma_chan *chan = s3c_dma_lookup_channel(channel);
	rt_ubase_t flags;

	if (chan == RT_NULL)
		return -1;

	flags = rt_hw_interrupt_disable();

	/* sort out stopping and freeing the channel */
	chan->in_use = 0;
	if (!(channel & DMACH_LOW_LEVEL))
		s3c_dma_chan_map[channel] = RT_NULL;

	rt_hw_interrupt_enable(flags);

	return 0;
}

static struct s3c64xx_dmac *s_dmacs[2];

static void s3c64xx_dma_irq(int irq)
{
	struct s3c64xx_dmac *dmac;
	struct s3c_dma_chan *chan;
	enum s3c_dma_buffresult res;
	rt_uint32_t tcstat, errstat;
	rt_uint32_t bit;
	int offs;

	if (irq == IRQ_DMA0)
		dmac = s_dmacs[0];
	else
		dmac = s_dmacs[1];

	tcstat = s3c_readl(dmac->regs + PL080_TC_STATUS);
	errstat = s3c_readl(dmac->regs + PL080_ERR_STATUS);

	for (offs = 0, bit = 1; offs < 8; offs++, bit <<= 1)
	{
		struct s3c_dma_buff *buff;

		if (!(errstat & bit) && !(tcstat & bit))
			continue;

		chan = dmac->channels + offs;
		res = S3C_RES_ERR;

		if (tcstat & bit)
		{
			s3c_writel(bit, dmac->regs + PL080_TC_CLEAR);
			res = S3C_RES_OK;
		}

		if (errstat & bit)
			s3c_writel(bit, dmac->regs + PL080_ERR_CLEAR);

		/* 'next' points to the buffer that is next to the
		 * currently active buffer.
		 * For CIRCULAR queues, 'next' will be same as 'curr'
		 * when 'end' is the active buffer.
		 */
		buff = chan->curr;
		while (buff && buff != chan->next
				&& buff->next != chan->next)
			buff = buff->next;

		if (!buff)
		{
			rt_kprintf("bug found here, %s--%d\n", __FILE__, __LINE__);
			return;
		}

		if (buff == chan->next)
			buff = chan->end;

		s3c64xx_dma_bufffdone(chan, buff, res);

		/* Free the node and update curr, if non-circular queue */
		if (!(chan->flags & S3C_DMAF_CIRCULAR))
		{
			chan->curr = buff->next;
			s3c64xx_dma_freebuff(buff);
		}

		/* Update 'next' */
		buff = chan->next;
		if (chan->next == chan->end)
		{
			chan->next = chan->curr;
			if (!(chan->flags & S3C_DMAF_CIRCULAR))
				chan->end = RT_NULL;
		}
		else
		{
			chan->next = buff->next;
		}
	}
}


static int s3c64xx_dma_init(int chno, enum s3c_dma_ch chbase,
			     int irq, rt_uint32_t base)
{
	struct s3c_dma_chan *chptr = &s3c6410_chans[chno];
	struct s3c64xx_dmac *dmac;
	char clkname[16];
	void *regs;
	void *regptr;
	int err, ch;

	dmac = rt_malloc(sizeof(struct s3c64xx_dmac));
	if (!dmac)
	{
		printk("%s: failed to alloc mem\n", __func__);
		return -1;
	}
	rt_memset(dmac, 0, sizeof(struct s3c64xx_dmac));

	dmac->regs = regs;
	dmac->chanbase = chbase;
	dmac->channels = chptr;

/*	err = request_irq(irq, s3c64xx_dma_irq, 0, "DMA", dmac);
	if (err < 0)
	{
		rt_kprintf("%s: failed to get irq\n", __func__);
		rt_free(dmac);
		return -2;
	}
*/	rt_hw_interrupt_install(irq, s3c64xx_dma_irq);

	regptr = regs + PL080_Cx_BASE(0);
	for (ch = 0; ch < 8; ch++, chptr++)
	{
		rt_kprintf("%s: registering DMA %d (%p)\n",
			 __func__, chno + ch, regptr);

		chptr->bit = 1 << ch;
		chptr->number = chno + ch;
		chptr->dmac = dmac;
		chptr->regs = regptr;
		regptr += PL080_Cx_STRIDE;
	}

	/* for the moment, permanently enable the controller */
	s3c_writel(PL080_CONFIG_ENABLE, regs + PL080_CONFIG);

	if (irq == IRQ_DMA0)
		s_dmacs[0] = dmac;
	else
		s_dmacs[1] = dmac;

	rt_kprintf("PL080: IRQ %d, at %p, channels %d..%d\n",
	       irq, regs, chno, chno+8);

	return 0;
}

int rt_hw_dma_init(void)
{
	int ret;

	rt_kprintf("%s: Registering DMA channels\n", __func__);
/*
	dma_pool = dma_pool_create("DMA-LLI", NULL, sizeof(struct pl080s_lli), 16, 0);
	if (!dma_pool) {
		printk(KERN_ERR "%s: failed to create pool\n", __func__);
		return -ENOMEM;
	}
*/
	s_dmacs[0] = s_dmacs[1] = RT_NULL;

	/* Set all DMA configuration to be DMA, not SDMA */
	s3c_writel(0xffffff, S3C_SYSREG(0x110));

	/* Register standard DMA controllers */
	s3c64xx_dma_init(0, DMACH_UART0, IRQ_DMA0, 0x75000000);
	s3c64xx_dma_init(8, DMACH_PCM1_TX, IRQ_DMA1, 0x75100000);

	return 0;
}

