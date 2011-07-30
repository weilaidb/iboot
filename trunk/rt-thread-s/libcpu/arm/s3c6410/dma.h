/*
 * File      : dma.h
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
#ifndef __DMA_H__
#define __DMA_H__

#include <rtdef.h>

typedef rt_ubase_t dma_addr_t;


#define PL080_INT_STATUS			(0x00)
#define PL080_TC_STATUS				(0x04)
#define PL080_TC_CLEAR				(0x08)
#define PL080_ERR_STATUS			(0x0C)
#define PL080_ERR_CLEAR				(0x10)
#define PL080_RAW_TC_STATUS			(0x14)
#define PL080_RAW_ERR_STATUS		(0x18)
#define PL080_EN_CHAN				(0x1c)
#define PL080_SOFT_BREQ				(0x20)
#define PL080_SOFT_SREQ				(0x24)
#define PL080_SOFT_LBREQ			(0x28)
#define PL080_SOFT_LSREQ			(0x2C)

#define PL080_CONFIG				(0x30)
#define PL080_CONFIG_M2_BE			(1 << 2)
#define PL080_CONFIG_M1_BE			(1 << 1)
#define PL080_CONFIG_ENABLE			(1 << 0)

#define PL080_SYNC					(0x34)

/* Per channel configuration registers */

#define PL080_Cx_STRIDE				(0x20)
#define PL080_Cx_BASE(x)			((0x100 + (x * 0x20)))
#define PL080_Cx_SRC_ADDR(x)		((0x100 + (x * 0x20)))
#define PL080_Cx_DST_ADDR(x)		((0x104 + (x * 0x20)))
#define PL080_Cx_LLI(x)				((0x108 + (x * 0x20)))
#define PL080_Cx_CONTROL(x)			((0x10C + (x * 0x20)))
#define PL080_Cx_CONFIG(x)			((0x110 + (x * 0x20)))
#define PL080S_Cx_CONTROL2(x)		((0x110 + (x * 0x20)))
#define PL080S_Cx_CONFIG(x)			((0x114 + (x * 0x20)))

#define PL080_CH_SRC_ADDR			(0x00)
#define PL080_CH_DST_ADDR			(0x04)
#define PL080_CH_LLI				(0x08)
#define PL080_CH_CONTROL			(0x0C)
#define PL080_CH_CONFIG				(0x10)
#define PL080S_CH_CONTROL2			(0x10)
#define PL080S_CH_CONFIG			(0x14)

#define PL080_LLI_ADDR_MASK					(0x3fffffff << 2)
#define PL080_LLI_ADDR_SHIFT				(2)
#define PL080_LLI_LM_AHB2					(1 << 0)

#define PL080_CONTROL_TC_IRQ_EN				(1 << 31)
#define PL080_CONTROL_PROT_MASK				(0x7 << 28)
#define PL080_CONTROL_PROT_SHIFT			(28)
#define PL080_CONTROL_PROT_CACHE			(1 << 30)
#define PL080_CONTROL_PROT_BUFF				(1 << 29)
#define PL080_CONTROL_PROT_SYS				(1 << 28)
#define PL080_CONTROL_DST_INCR				(1 << 27)
#define PL080_CONTROL_SRC_INCR				(1 << 26)
#define PL080_CONTROL_DST_AHB2				(1 << 25)
#define PL080_CONTROL_SRC_AHB2				(1 << 24)
#define PL080_CONTROL_DWIDTH_MASK			(0x7 << 21)
#define PL080_CONTROL_DWIDTH_SHIFT			(21)
#define PL080_CONTROL_SWIDTH_MASK			(0x7 << 18)
#define PL080_CONTROL_SWIDTH_SHIFT			(18)
#define PL080_CONTROL_DB_SIZE_MASK			(0x7 << 15)
#define PL080_CONTROL_DB_SIZE_SHIFT			(15)
#define PL080_CONTROL_SB_SIZE_MASK			(0x7 << 12)
#define PL080_CONTROL_SB_SIZE_SHIFT			(12)
#define PL080_CONTROL_TRANSFER_SIZE_MASK	(0xfff << 0)
#define PL080_CONTROL_TRANSFER_SIZE_SHIFT	(0)

#define PL080_BSIZE_1						(0x0)
#define PL080_BSIZE_4						(0x1)
#define PL080_BSIZE_8						(0x2)
#define PL080_BSIZE_16						(0x3)
#define PL080_BSIZE_32						(0x4)
#define PL080_BSIZE_64						(0x5)
#define PL080_BSIZE_128						(0x6)
#define PL080_BSIZE_256						(0x7)

#define PL080_WIDTH_8BIT					(0x0)
#define PL080_WIDTH_16BIT					(0x1)
#define PL080_WIDTH_32BIT					(0x2)

#define PL080_CONFIG_HALT					(1 << 18)
#define PL080_CONFIG_ACTIVE					(1 << 17)  /* RO */
#define PL080_CONFIG_LOCK					(1 << 16)
#define PL080_CONFIG_TC_IRQ_MASK			(1 << 15)
#define PL080_CONFIG_ERR_IRQ_MASK			(1 << 14)
#define PL080_CONFIG_FLOW_CONTROL_MASK		(0x7 << 11)
#define PL080_CONFIG_FLOW_CONTROL_SHIFT		(11)
#define PL080_CONFIG_DST_SEL_MASK			(0xf << 6)
#define PL080_CONFIG_DST_SEL_SHIFT			(6)
#define PL080_CONFIG_SRC_SEL_MASK			(0xf << 1)
#define PL080_CONFIG_SRC_SEL_SHIFT			(1)
#define PL080_CONFIG_ENABLE					(1 << 0)

#define PL080_FLOW_MEM2MEM					(0x0)
#define PL080_FLOW_MEM2PER					(0x1)
#define PL080_FLOW_PER2MEM					(0x2)
#define PL080_FLOW_SRC2DST					(0x3)
#define PL080_FLOW_SRC2DST_DST				(0x4)
#define PL080_FLOW_MEM2PER_PER				(0x5)
#define PL080_FLOW_PER2MEM_PER				(0x6)
#define PL080_FLOW_SRC2DST_SRC				(0x7)


/* DMA linked list chain structure */
struct pl080s_lli
{
	rt_uint32_t	src_addr;
	rt_uint32_t	dst_addr;
	rt_uint32_t	next_lli;
	rt_uint32_t	control0;
	rt_uint32_t control1;
};


#define S3C_DMA_CHANNELS	(16)


/* Note, for the S3C64XX architecture we keep the DMACH_
 * defines in the order they are allocated to [S]DMA0/[S]DMA1
 * so that is easy to do DHACH_ -> DMA controller conversion
 */
enum s3c_dma_ch
{
	/* DMA0/SDMA0 */
	DMACH_UART0 = 0,
	DMACH_UART0_SRC2,
	DMACH_UART1,
	DMACH_UART1_SRC2,
	DMACH_UART2,
	DMACH_UART2_SRC2,
	DMACH_UART3,
	DMACH_UART3_SRC2,
	DMACH_PCM0_TX,
	DMACH_PCM0_RX,
	DMACH_I2S0_OUT,
	DMACH_I2S0_IN,
	DMACH_SPI0_TX,
	DMACH_SPI0_RX,
	DMACH_HSI_I2SV40_TX,
	DMACH_HSI_I2SV40_RX,

	/* DMA1/SDMA1 */
	DMACH_PCM1_TX = 16,
	DMACH_PCM1_RX,
	DMACH_I2S1_OUT,
	DMACH_I2S1_IN,
	DMACH_SPI1_TX,
	DMACH_SPI1_RX,
	DMACH_AC97_PCMOUT,
	DMACH_AC97_PCMIN,
	DMACH_AC97_MICIN,
	DMACH_PWM,
	DMACH_IRDA,
	DMACH_EXTERNAL,
	DMACH_RES1,
	DMACH_RES2,
	DMACH_SECURITY_RX,	/* SDMA1 only */
	DMACH_SECURITY_TX,	/* SDMA1 only */
	DMACH_MAX		/* the end */
};

static __inline__ int s3c_dma_has_circular(void)
{
	return 1;
}

enum s3c_dma_buffresult
{
	S3C_RES_OK,
	S3C_RES_ERR,
	S3C_RES_ABORT
};

enum s3c_dmasrc
{
	S3C_DMASRC_HW,		/* source is memory */
	S3C_DMASRC_MEM		/* source is hardware */
};

/* enum chan_op
 *
 * operation codes passed to the DMA code by the user, and also used
 * to inform the current channel owner of any changes to the system state
*/
enum s3c_chan_op
{
	S3C_DMAOP_START,
	S3C_DMAOP_STOP,
	S3C_DMAOP_PAUSE,
	S3C_DMAOP_RESUME,
	S3C_DMAOP_FLUSH,
	S3C_DMAOP_TIMEOUT,		/* internal signal to handler */
	S3C_DMAOP_STARTED,		/* indicate channel started */
};

struct s3c_dma_chan;

/* s3c_dma_cbfn_t
 *
 * buffer callback routine type
*/
typedef void (*s3c_dma_cbfn_t)(struct s3c_dma_chan *,
				   void *buf, int size,
				   enum s3c_dma_buffresult result);

typedef int (*s3c_dma_opfn_t)(struct s3c_dma_chan *, enum chan_op);

#define S3C_DMAF_CIRCULAR			(1 << 0)
#define DMACH_LOW_LEVEL				(1<<28) /* use this to specifiy hardware ch no */

/** s3c_dma_buff - S3C64XX DMA buffer descriptor
 * @next: Pointer to next buffer in queue or ring.
 * @pw: Client provided identifier
 * @lli: Pointer to hardware descriptor this buffer is associated with.
 * @lli_dma: Hardare address of the descriptor.
 */
struct s3c_dma_buff
{
	struct s3c_dma_buff *next;

	void *pw;
	struct pl080s_lli *lli;
	dma_addr_t lli_dma;
};

struct s3c64xx_dmac
{
	void *regs;
	struct s3c_dma_chan *channels;
	enum s3c_dma_ch chanbase;
};

struct s3c_dma_chan
{
	rt_uint8_t number;      /* number of this dma channel */
	rt_uint8_t in_use;      /* channel allocated */
	rt_uint8_t bit;	      /* bit for enable/disable/etc */
	rt_uint8_t hw_width;
	rt_uint8_t peripheral;

	rt_uint32_t flags;
	enum s3c_dmasrc source;

	dma_addr_t dev_addr;

	struct s3c64xx_dmac	*dmac;		/* pointer to controller */

	void *regs;

	/* cdriver callbacks */
	s3c_dma_cbfn_t callback_fn;	/* buffer done callback */
	s3c_dma_opfn_t op_fn;		/* channel op callback */

	/* buffer list and information */
	struct s3c_dma_buff	*curr;		/* current dma buffer */
	struct s3c_dma_buff	*next;		/* next buffer to load */
	struct s3c_dma_buff	*end;		/* end of queue */

	/* note, when channel is running in circular mode, curr is the
	 * first buffer enqueued, end is the last and curr is where the
	 * last buffer-done event is set-at. The buffers are not freed
	 * and the last buffer hardware descriptor points back to the
	 * first.
	 */
};


/* s3c_dma_request
 *
 * request a dma channel exclusivley
*/
int s3c_dma_request(rt_uint32_t channel, void *dev);

/* s3c_dma_ctrl
 *
 * change the state of the dma channel
*/
int s3c_dma_ctrl(rt_uint32_t channel, enum s3c_chan_op op);

/* s3c_dma_setflags
 *
 * set the channel's flags to a given state
*/
int s3c_dma_setflags(rt_uint32_t channel, rt_uint32_t flags);

/* s3c_dma_free
 *
 * free the dma channel (will also abort any outstanding operations)
*/
int s3c_dma_free(rt_uint32_t channel);

/* s3c_dma_enqueue
 *
 * place the given buffer onto the queue of operations for the channel.
 * The buffer must be allocated from dma coherent memory, or the Dcache/WB
 * drained before the buffer is given to the DMA system.
*/
int s3c_dma_enqueue(rt_uint32_t channel, void *id, dma_addr_t data, int size);

/* s3c_dma_config
 *
 * configure the dma channel
*/
int s3c_dma_config(rt_uint32_t channel, int xferunit);

/* s3c_dma_devconfig
 *
 * configure the device we're talking to
*/
int s3c_dma_devconfig(rt_uint32_t channel, enum s3c_dmasrc source, rt_ubase_t devaddr);

/* s3c_dma_getposition
 *
 * get the position that the dma transfer is currently at
*/
int s3c_dma_getposition(rt_uint32_t channel, dma_addr_t *src, dma_addr_t *dest);

int s3c_dma_set_opfn(rt_uint32_t, s3c_dma_opfn_t rtn);
int s3c_dma_set_buffdone_fn(rt_uint32_t, s3c_dma_cbfn_t rtn);


#endif /* end of __DMA_H__ */
