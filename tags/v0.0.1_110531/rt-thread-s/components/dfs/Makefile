
CURDIR := $(shell pwd)
TOPDIR  = $(CURDIR)/../..
include $(TOPDIR)/rules.mk

INCDIR  = $(INCSYS) $(INCBSP) $(INCLIBC) $(INCFINSH) \
		  ./include ./filesystems/devfs ./filesystems/romfs
INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))

CFLAGS += $(INCLUDE)
AFLAGS += $(INCLUDE)

SRC_COM   = $(wildcard src/*.c)
SRC_DEVFS = $(wildcard filesystems/devfs/*.c)
SRC_ROMFS = $(wildcard filesystems/romfs/*.c)
SRC_FATFS = $(wildcard filesystems/elmfat/*.c)
SRC       = $(SRC_DVFS) $(SRC_ROMFS) $(SRC_FATFS)

OBJ_DEVFS = $(SRC_DEVFS:.c=.o)
OBJ_ROMFS = $(SRC_ROMFS:.c=.o)
OBJ_FATFS = $(SRC_FATFS:.c=.o)
OBJ       = $(OBJ_DEVFS) $(OBJ_ROMFS) $(OBJ_FATFS)
OBJ_COM   = $(SRC_COM:.c=.o)

.PHONY : all
all: $(LIBDFS)

$(LIBDFS): $(OBJ) $(OBJ_COM)
	$(AR) rcs $(LIBDFS) $(OBJ) $(OBJ_COM)

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr $(OBJ) $(OBJ_COM) $(LIBDFS)