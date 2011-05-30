
ARCH = arm
CPU  = s3c6410
BSP  = tiny6410
TEXTBASE = 0x00008000

# cross compile
CROSS_COMPILE  = arm-none-eabi
TOOLCHAIN_PATH = /e/yagarto-tools

CC      = $(CROSS_COMPILE)-gcc
LD      = $(CROSS_COMPILE)-ld
AR      = $(CROSS_COMPILE)-ar
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP = $(CROSS_COMPILE)-objdump
STRIP   = $(CROSS_COMPILE)-strip
READELF = $(CROSS_COMPILE)-readelf

# platform depent flags
CPUFLAGS = -mcpu=arm1176jzf-s
PLATFORM_CFLAGS = $(CPUFLAGS) -fno-strict-aliasing -fno-common -ffixed-r8 -msoft-float

# rtt-elements
P_ARCH    = $(TOPDIR)/libcpu/$(ARCH)/$(CPU)
P_BSP     = $(TOPDIR)/bsp/$(BSP)
P_SYS     = $(TOPDIR)/src
P_LIBC    = $(TOPDIR)/components/libc/minilibc
P_PTHREAD = $(TOPDIR)/components/pthread
P_FINSH   = $(TOPDIR)/components/finsh
P_DFS     = $(TOPDIR)/components/dfs
P_GUI     = $(TOPDIR)/components/rtgui

INCARCH   = $(P_ARCH)
INCBSP    = $(P_BSP)
INCSYS    = $(TOPDIR)/include
INCLIBC   = $(P_LIBC)
INCPTHREAD= $(P_PTHREAD)
INCFINSH  = $(P_FINSH)
INCDFS    = $(P_DFS)/include
INCGUI    = $(P_GUI)/include

LIBARCH   = $(TOPDIR)/lib$(CPU).a
LIBBSP    = $(TOPDIR)/lib$(BSP).a
LIBSYS    = $(TOPDIR)/libsys.a
LIBC      = $(TOPDIR)/libc.a
LIBPTHREAD= $(TOPDIR)/libpthread.a
LIBFINSH  = $(TOPDIR)/libfinsh.a
LIBDFS    = $(TOPDIR)/libdfs.a
LIBGUI    = $(TOPDIR)/librtgui.a
TARGET    = $(TOPDIR)/rtt-$(BSP)

# compile flags
ENDIAN  = 
CFLAGS  = -g -O2 -Wall -Wno-unknown-pragmas -nostdinc -nostdlib -fno-builtin $(PLATFORM_CFLAGS)
AFLAGS  = $(CFLAGS) -D__ASSEMBLY__ -DTEXT_BASE=$(TEXTBASE)

LIB_PATH = $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)/lib
LDSCRIPT = $(INCBSP)/link.lds
LDFLAGS  = $(CPUFLAGS) -nostartfiles -T $(LDSCRIPT) -Ttext $(TEXTBASE) -L$(LIB_PATH)
