
CURDIR := $(shell pwd)
TOPDIR  = $(CURDIR)/../../..
include $(TOPDIR)/rules.mk

INCDIR  = . $(INCSYS) $(INCBSP) $(INCLIBC) $(INCFINSH)
INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))

CFLAGS += $(INCLUDE)
AFLAGS += $(INCLUDE)

SRC_CC = $(wildcard ../common/*.c)
SRC_CS = $(wildcard ../common/*.S)
SRC_C  = $(wildcard *.c)
SRC_S  = $(wildcard *.S)
SRC    = $(SRC_C) $(SRC_S)

OBJ_C  = $(SRC_C:.c=.o)
OBJ_S  = $(SRC_S:.S=.o)
OBJ    = $(OBJ_C) $(OBJ_S)
OBJ_CC = $(SRC_CC:.c=.o)
OBJ_CS = $(SRC_CS:.S=.o)
OBJ_COM= $(OBJ_CC) $(OBJ_CS)

.PHONY : all
all: $(LIBARCH)

$(LIBARCH): $(OBJ) $(OBJ_COM)
	$(AR) rcs $(LIBARCH) $(OBJ) $(OBJ_COM)

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr $(OBJ) $(OBJ_COM) $(LIBARCH)