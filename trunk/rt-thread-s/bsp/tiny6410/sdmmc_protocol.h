/*
 * sdmmc_protocol.h
 *
 *  Created on: 2011-6-5
 *      Author: sangwei
 */
#ifndef __SDMMC_PROTOCOL_H__
#define __SDMMC_PROTOCOL_H__

   /* class 1 */
#define	MMC_GO_IDLE_STATE         0   /* bc                          */
#define MMC_SEND_OP_COND          1   /* bcr  [31:0] OCR         R3  */
#define MMC_ALL_SEND_CID          2   /* bcr                     R2  */
#define MMC_SET_RELATIVE_ADDR     3   /* ac   [31:16] RCA        R1  */
#define MMC_SET_DSR               4   /* bc   [31:16] RCA            */
//#ifdef CONFIG_SUPPORT_MMC_PLUS
#define MMC_SWITCH                6   /* ac                      R1b */
//#endif
#define MMC_SELECT_CARD           7   /* ac   [31:16] RCA        R1  */
//#ifdef CONFIG_SUPPORT_MMC_PLUS
#define MMC_SEND_EXT_CSD          8   /* adtc                    R1  */
//#endif
#define MMC_SEND_CSD              9   /* ac   [31:16] RCA        R2  */
#define MMC_SEND_CID             10   /* ac   [31:16] RCA        R2  */
#define MMC_READ_DAT_UNTIL_STOP  11   /* adtc [31:0] dadr        R1  */
#define MMC_STOP_TRANSMISSION    12   /* ac                      R1b */
#define MMC_SEND_STATUS	         13   /* ac   [31:16] RCA        R1  */
//#ifdef CONFIG_SUPPORT_MMC_PLUS
#define MMC_BUSTEST_R            14   /* adtc                    R1  */
//#endif
#define MMC_GO_INACTIVE_STATE    15   /* ac   [31:16] RCA            */
//#ifdef CONFIG_SUPPORT_MMC_PLUS
#define MMC_BUSTEST_W            19   /* adtc                    R1  */
//#endif
  /* class 2 */
#define MMC_SET_BLOCKLEN         16   /* ac   [31:0] block len   R1  */
#define MMC_READ_SINGLE_BLOCK    17   /* adtc [31:0] data addr   R1  */
#define MMC_READ_MULTIPLE_BLOCK  18   /* adtc [31:0] data addr   R1  */

  /* class 3 */
#define MMC_WRITE_DAT_UNTIL_STOP 20   /* adtc [31:0] data addr   R1  */

  /* class 4 */
#define MMC_SET_BLOCK_COUNT      23   /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_BLOCK          24   /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_MULTIPLE_BLOCK 25   /* adtc                    R1  */
#define MMC_PROGRAM_CID          26   /* adtc                    R1  */
#define MMC_PROGRAM_CSD          27   /* adtc                    R1  */

  /* class 6 */
#define MMC_SET_WRITE_PROT       28   /* ac   [31:0] data addr   R1b */
#define MMC_CLR_WRITE_PROT       29   /* ac   [31:0] data addr   R1b */
#define MMC_SEND_WRITE_PROT      30   /* adtc [31:0] wpdata addr R1  */

  /* class 5 */
#define MMC_ERASE_GROUP_START    35   /* ac   [31:0] data addr   R1  */
#define MMC_ERASE_GROUP_END      36   /* ac   [31:0] data addr   R1  */
#define MMC_ERASE                38   /* ac                      R1b */

  /* class 9 */
#define MMC_FAST_IO              39   /* ac   <Complex>          R4  */
#define MMC_GO_IRQ_STATE         40   /* bcr                     R5  */

  /* class 7 */
#define MMC_LOCK_UNLOCK          42   /* adtc                    R1b */

  /* class 8 */
#define MMC_APP_CMD              55   /* ac   [31:16] RCA        R1  */
#define MMC_GEN_CMD              56   /* adtc [0] RD/WR          R1  */

/* SD commands                           type  argument     response */
  /* class 8 */
/* This is basically the same command as for MMC with some quirks. */
#ifdef CONFIG_SUPPORT_MMC_PLUS
#define SD_SEND_RELATIVE_ADDR     3   /* ac                      R6  */
#else
#define SD_SEND_RELATIVE_ADDR     3   /* bcr                     R6  */
#endif

  /* Application commands */
#define SD_APP_SET_BUS_WIDTH      6   /* ac   [1:0] bus width    R1  */
#define SD_APP_OP_COND           41   /* bcr  [31:0] OCR         R3  */
#define SD_APP_SEND_SCR          51   /* adtc                    R1  */

/*
  MMC status in R1
  Type
  	e : error bit
	s : status bit
	r : detected and set for the actual command response
	x : detected and set during command execution. the host must poll
            the card by sending status command in order to read these bits.
  Clear condition
  	a : according to the card state
	b : always related to the previous command. Reception of
            a valid command will clear it (with a delay of one command)
	c : clear by read
 */

#define R1_OUT_OF_RANGE			(1 << 31)	/* er, c */
#define R1_ADDRESS_ERROR		(1 << 30)	/* erx, c */
#define R1_BLOCK_LEN_ERROR		(1 << 29)	/* er, c */
#define R1_ERASE_SEQ_ERROR      (1 << 28)	/* er, c */
#define R1_ERASE_PARAM			(1 << 27)	/* ex, c */
#define R1_WP_VIOLATION			(1 << 26)	/* erx, c */
#define R1_CARD_IS_LOCKED		(1 << 25)	/* sx, a */
#define R1_LOCK_UNLOCK_FAILED	(1 << 24)	/* erx, c */
#define R1_COM_CRC_ERROR		(1 << 23)	/* er, b */
#define R1_ILLEGAL_COMMAND		(1 << 22)	/* er, b */
#define R1_CARD_ECC_FAILED		(1 << 21)	/* ex, c */
#define R1_CC_ERROR				(1 << 20)	/* erx, c */
#define R1_ERROR				(1 << 19)	/* erx, c */
#define R1_UNDERRUN				(1 << 18)	/* ex, c */
#define R1_OVERRUN				(1 << 17)	/* ex, c */
#define R1_CID_CSD_OVERWRITE	(1 << 16)	/* erx, c, CID/CSD overwrite */
#define R1_WP_ERASE_SKIP		(1 << 15)	/* sx, c */
#define R1_CARD_ECC_DISABLED	(1 << 14)	/* sx, a */
#define R1_ERASE_RESET			(1 << 13)	/* sr, c */
#define R1_STATUS(x)            (x & 0xFFFFE000)
#define R1_CURRENT_STATE(x)    	((x & 0x00001E00) >> 9)	/* sx, b (4 bits) */
#define R1_READY_FOR_DATA		(1 << 8)	/* sx, a */
#define R1_APP_CMD				(1 << 5)	/* sr, c */


#endif /* __SDMMC_PROTOCOL_H__ */
