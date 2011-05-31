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
     * close lcd backlight
     */
    .globl lcd_bl_close
lcd_bl_close:
    /* LED on only #8 */
    ldr     r0, =ELFIN_GPIO_BASE

    ldr     r1, =0x55540000
    str     r1, [r0, #oGPNCON]
    ldr     r1, =0x55555555
    str     r1, [r0, #oGPNPUD]
    ldr     r1, =0xf000
    str     r1, [r0, #oGPNDAT]

    ldr     r1, =0x01
    str     r1, [r0, #oGPECON]
    ldr     r1, =0x00
    str     r1, [r0, #oGPEDAT]

    ldr     r1, =0x2A5AAAAA
    str     r1, [r0, #oGPPCON]
    ldr     r1, =0x0
    str     r1, [r0, #oGPPDAT]

    mov     pc, lr


    /*
     * clock setup
     */
    .globl clock_setup
clock_setup:
    // Prepare to Change PLL
    ldr     r4, =CLK_DIV0
    ldr     r5, [r4]
    bic     r5, r5, #0xff00
    bic     r5, r5, #0xff
    ldr     r6, =CLK_DIV_VAL
    orr     r5, r5, r6
    str     r5, [r4]

    // Change PLL Value
    ldr     r5, =0xffff                 // Lock Time : 0x4b1 (100us @Fin12MHz) for APLL/MPLL
    ldr     r6, =0xe13                  // Lock Time : 0xe13 (300us @Fin12MHz) for EPLL
    ldr     r4, =APLL_LOCK
    str     r5, [r4]                    // APLL Lock Time
    str     r5, [r4, #0x4]              // MPLL Lock Time
    str     r6, [r4, #0x8]              // EPLL Lock Time

    // Set System Clock Divider
    ldr     r4, =APLL_CON
    ldr     r5, =APLL_VAL
    str     r5, [r4]

    ldr     r4, =MPLL_CON
    ldr     r5, =MPLL_VAL
    str     r5, [r4]

    ldr     r4, =EPLL_CON1
    ldr     r5, =EPLL_KVAL
    str     r5, [r4]

    ldr     r4, =EPLL_CON0
    ldr     r5, =EPLL_VAL
    str     r5, [r4]

    // Enable PLL Clock Out
    ldr     r4, =CLK_SRC
    ldr     r5, [r4]
    orr     r5, r5, #0x7            // PLL  Clockout
    str     r5, [r4]                // System will be waiting for PLL unlocked after this instruction

    mov     pc, lr


    /*
     * Set to Synchronous Mode
     * hclk & pclk from APLL
     */
    .globl  set_sync_mode
set_sync_mode:
    ldr     r4, =OTHERS
    ldr     r5, [r4]
    orr     r5, r5, #0x40           // SyncMUXSEL = DOUT_APLL
    str     r5, [r4]
    nop
    nop
    nop
    nop
    nop
    ldr     r5, [r4]
    orr     r5, r5, #0x80           // SyncReq = request Sync
    str     r5, [r4]
_wait_for_sync:
    ldr     r5, [r4]                // Read OTHERS
    and     r5, r5, #0xF00          // Wait SYNCMODEACK = 0xF
    cmp     r5, #0xF00
    bne     _wait_for_sync

    mov     pc, lr


    /*
     * Set to Asynchronous Mode
     * hclk & pclk from MPLL
     */
    .globl  set_async_mode
set_async_mode:
    ldr     r4, =OTHERS
    ldr     r5, [r4]
    bic     r5, r5, #0xC0
    orr     r5, r5, #0x40           // SyncReq = Async, SyncMUX = Sync
    str     r5, [r4]
_wait_for_async:
    ldr     r5, [r4]                // Read OTHERS
    and     r5, r5, #0xF00          // Wait SYNCMODEACK = 0x0
    cmp     r5, #0x0
    bne     _wait_for_async

    ldr     r4, =OTHERS
    ldr     r5, [r4]
    bic     r5, r5, #0x40           // SyncMUX = Async
    str     r5, [r4]

    nop
    nop
    nop
    nop
    nop
    mov     pc, lr


    /* ddr init */
    .globl  mem_ctrl_asm_init
mem_ctrl_asm_init:
    ldr     r4, =ELFIN_MEM_SYS_CFG      // Memory sussystem address 0x7e00f120
    mov     r5, #0xd                    // Xm0CSn2 = NFCON CS0, Xm0CSn3 = NFCON CS1
    str     r5, [r4]

    ldr     r4, =ELFIN_DMC1_BASE        // DMC1 base address 0x7e001000

    @Enter Config State
    ldr     r5, =0x04
    str     r5, [r4, #INDEX_DMC_MEMC_CMD]

    ldr     r5, =DMC_DDR_REFRESH_PRD
    str     r5, [r4, #INDEX_DMC_REFRESH_PRD]

    ldr     r5, =DMC_DDR_CAS_LATENCY
    str     r5, [r4, #INDEX_DMC_CAS_LATENCY]

    ldr     r5, =DMC_DDR_t_DQSS
    str     r5, [r4, #INDEX_DMC_T_DQSS]

    ldr     r5, =DMC_DDR_t_MRD
    str     r5, [r4, #INDEX_DMC_T_MRD]

    ldr     r5, =DMC_DDR_t_RAS
    str     r5, [r4, #INDEX_DMC_T_RAS]

    ldr     r5, =DMC_DDR_t_RC
    str     r5, [r4, #INDEX_DMC_T_RC]

    ldr     r5, =DMC_DDR_t_RCD
    ldr     r6, =DMC_DDR_schedule_RCD
    orr     r5, r5, r6
    str     r5, [r4, #INDEX_DMC_T_RCD]

    ldr     r5, =DMC_DDR_t_RFC
    ldr     r6, =DMC_DDR_schedule_RFC
    orr     r5, r5, r6
    str     r5, [r4, #INDEX_DMC_T_RFC]

    ldr     r5, =DMC_DDR_t_RP
    ldr     r6, =DMC_DDR_schedule_RP
    orr     r5, r5, r6
    str     r5, [r4, #INDEX_DMC_T_RP]

    ldr     r5, =DMC_DDR_t_RRD
    str     r5, [r4, #INDEX_DMC_T_RRD]

    ldr     r5, =DMC_DDR_t_WR
    str     r5, [r4, #INDEX_DMC_T_WR]

    ldr     r5, =DMC_DDR_t_WTR
    str     r5, [r4, #INDEX_DMC_T_WTR]

    ldr     r5, =DMC_DDR_t_XP
    str     r5, [r4, #INDEX_DMC_T_XP]

    ldr     r5, =DMC_DDR_t_XSR
    str     r5, [r4, #INDEX_DMC_T_XSR]

    ldr     r5, =DMC_DDR_t_ESR
    str     r5, [r4, #INDEX_DMC_T_ESR]

    ldr     r5, =DMC1_MEM_CFG
    str     r5, [r4, #INDEX_DMC_MEMORY_CFG]

    ldr     r5, =DMC1_MEM_CFG2
    str     r5, [r4, #INDEX_DMC_MEMORY_CFG2]

    ldr     r5, =DMC1_CHIP0_CFG
    str     r5, [r4, #INDEX_DMC_CHIP_0_CFG]

    ldr     r5, =DMC_DDR_32_CFG
    str     r5, [r4, #INDEX_DMC_USER_CONFIG]

    @DMC0 DDR Chip 0 configuration direct command reg
    ldr     r5, =DMC_NOP0
    str     r5, [r4, #INDEX_DMC_DIRECT_CMD]

    @Precharge All
    ldr     r5, =DMC_PA0
    str     r5, [r4, #INDEX_DMC_DIRECT_CMD]

    @Auto Refresh   2 time
    ldr     r5, =DMC_AR0
    str     r5, [r4, #INDEX_DMC_DIRECT_CMD]
    str     r5, [r4, #INDEX_DMC_DIRECT_CMD]

    @MRS
    ldr     r5, =DMC_mDDR_EMR0
    str     r5, [r4, #INDEX_DMC_DIRECT_CMD]

    @Mode Reg
    ldr     r5, =DMC_mDDR_MR0
    str     r5, [r4, #INDEX_DMC_DIRECT_CMD]

    @Enable DMC1, Enter Ready State
    mov     r5, #0x0
    str     r5, [r4, #INDEX_DMC_MEMC_CMD]

    @Wait for Ready
_check_dmc1_ready:
    ldr     r5, [r4, #INDEX_DMC_MEMC_STATUS]
    mov     r6, #0x3
    and     r5, r5, r6
    cmp     r5, #0x1
    bne     _check_dmc1_ready
    nop

    mov     pc, lr
