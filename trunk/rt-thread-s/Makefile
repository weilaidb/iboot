#/*
# * File      : cpu.c
# * This file is part of RT-Thread RTOS
# * COPYRIGHT (C) 2006, RT-Thread Develop Team
# *
# * The license and distribution terms for this file may be
# * found in the file LICENSE in this distribution or at
# * http://openlab.rt-thread.com/license/LICENSE
# *
# * Change Logs:
# * Date           Author       Notes
# * 2011-05-29     swkyer       first version
# */

CURDIR := $(shell pwd)
TOPDIR  = $(CURDIR)
include $(TOPDIR)/rules.mk

INCDIR  = . $(INCARCH) $(INCSYS) $(INCBSP) $(INCLIBC) $(INCPTHREAD) $(INCFINSH) $(INCDFS) $(INCGUI) \
          $(INCDFS)/../filesystems/romfs
INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))

CFLAGS += $(INCLUDE)
AFLAGS += $(INCLUDE)

SRC_COM = 

SRC_ARCH_C   = $(wildcard $(P_ARCH)/*.c $(P_ARCH)/../common/*.c)
SRC_ARCH_S   = $(wildcard $(P_ARCH)/*.S)
OBJ_ARCH_C   = $(SRC_ARCH_C:.c=.o)
OBJ_ARCH_S   = $(SRC_ARCH_S:.S=.o)

SRC_BSP_C   = $(wildcard $(P_BSP)/*.c)
SRC_BSP_S   = $(wildcard $(P_BSP)/*.S)
OBJ_BSP_C   = $(SRC_BSP_C:.c=.o)
OBJ_BSP_S   = $(SRC_BSP_S:.S=.o)

SRC_SYS_C   = $(wildcard $(P_SYS)/*.c)
SRC_SYS_S   = $(wildcard $(P_SYS)/*.S)
OBJ_SYS_C   = $(SRC_SYS_C:.c=.o)
OBJ_SYS_S   = $(SRC_SYS_S:.S=.o)

SRC_LIBC_C   = $(wildcard $(P_LIBC)/*.c)
SRC_LIBC_S   = $(wildcard $(P_LIBC)/*.S)
OBJ_LIBC_C   = $(SRC_LIBC_C:.c=.o)
OBJ_LIBC_S   = $(SRC_LIBC_S:.S=.o)

SRC_PTHREAD_C   = $(wildcard $(P_PTHREAD)/*.c)
SRC_PTHREAD_S   = $(wildcard $(P_PTHREAD)/*.S)
OBJ_PTHREAD_C   = $(SRC_PTHREAD_C:.c=.o)
OBJ_PTHREAD_S   = $(SRC_PTHREAD_S:.S=.o)

SRC_FINSH_C   = $(wildcard $(P_FINSH)/*.c)
SRC_FINSH_S   = $(wildcard $(P_FINSH)/*.S)
OBJ_FINSH_C   = $(SRC_FINSH_C:.c=.o)
OBJ_FINSH_S   = $(SRC_FINSH_S:.S=.o)

SRC_DFS_C   = $(wildcard $(P_DFS)/src/*.c) $(wildcard $(P_DFS)/filesystems/devfs/*.c) \
                $(wildcard $(P_DFS)/filesystems/elmfat/*.c) $(wildcard $(P_DFS)/filesystems/romfs/*.c) 
SRC_DFS_S   = $(wildcard $(P_DFS)/src/*.S) $(wildcard $(P_DFS)/filesystems/devfs/*.S) \
                $(wildcard $(P_DFS)/filesystems/elmfat/*.S) $(wildcard $(P_DFS)/filesystems/romfs/*.S)
OBJ_DFS_C   = $(SRC_DFS_C:.c=.o)
OBJ_DFS_S   = $(SRC_DFS_S:.S=.o)

SRC_GUI_C   = $(wildcard $(P_GUI)/common/*.c) $(wildcard $(P_GUI)/server/*.c) \
                $(wildcard $(P_GUI)/widgets/*.c)
SRC_GUI_S   = $(wildcard $(P_GUI)/common/*.S) $(wildcard $(P_GUI)/server/*.S) \
                $(wildcard $(P_GUI)/widgets/*.S)
OBJ_GUI_C   = $(SRC_GUI_C:.c=.o)
OBJ_GUI_S   = $(SRC_GUI_S:.S=.o)
#$(OBJ_GUI_C) $(OBJ_GUI_S) $(OBJ_PTHREAD_C) $(OBJ_PTHREAD_S) -Map $(TARGET).map

OBJ     = $(OBJ_ARCH_C) $(OBJ_ARCH_S) $(OBJ_BSP_C) $(OBJ_BSP_S) $(OBJ_SYS_C) $(OBJ_SYS_S) \
            $(OBJ_LIBC_C) $(OBJ_LIBC_S) $(OBJ_FINSH_C) $(OBJ_FINSH_S) \
            $(OBJ_DFS_C) $(OBJ_DFS_S)
OBJ_COM = $(SRC_COM:.c=.o)

.PHONY : all
all: $(OBJ) $(OBJ_COM)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ) $(OBJ_COM) -lm
	$(OBJCOPY) -O binary $(TARGET) $(TARGET).bin
	$(OBJDUMP) -D $(TARGET) > $(TARGET).lst

%.o: %.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr $(OBJ) $(OBJ_COM) $(TARGET) $(TARGET).bin $(TARGET).lst $(TARGET).map