#/***************************************************************************
#*   Copyright (C) 2011 by swkyer <swkyer@gmail.com>                       *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************/
TARGET   = sdloader
TEXTBASE = 0x0c000000

# cross compile
CROSS_COMPILE  = arm-none-eabi

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

CFLAGS  = -g -O2 -Wall -Wno-unknown-pragmas -nostdinc -nostdlib -fno-builtin $(PLATFORM_CFLAGS)
AFLAGS  = $(CFLAGS) -D__ASSEMBLY__

INCDIR  = . 
INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))

CFLAGS += $(INCLUDE)
AFLAGS += $(INCLUDE)

LDSCRIPT = ./link.lds
LDFLAGS  = $(CPUFLAGS) -nostartfiles -T $(LDSCRIPT) -Ttext $(TEXTBASE)


SRC_COM = 

SRC_C   = $(wildcard *.c)
SRC_S   = $(wildcard *.S)
OBJ_C   = $(SRC_C:.c=.o)
OBJ_S   = $(SRC_S:.S=.o)

OBJ     = $(OBJ_C) $(OBJ_S)
OBJ_COM = $(SRC_COM:.c=.o)


.PHONY : all
all: $(OBJ) $(OBJ_COM)
	$(CC) $(LDFLAGS) -o $(TARGET).elf $(OBJ) $(OBJ_COM)
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).lst

%.o: %.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr $(OBJ) $(OBJ_COM) $(TARGET).elf $(TARGET).bin $(TARGET).lst $(TARGET).map