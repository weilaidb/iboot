
CURDIR := $(shell pwd)
TOPDIR  = $(CURDIR)/../../..
include $(TOPDIR)/rules.mk

INCDIR  = . $(INCSYS) $(INCBSP) $(INCLIBC) $(INCFINSH)
INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))

CFLAGS += $(INCLUDE)
AFLAGS += $(INCLUDE)

SRC_COM= 
SRC_C  = $(wildcard *.c)
SRC_S  = $(wildcard *.S)
SRC    = $(SRC_C) $(SRC_S)

OBJ_C  = $(SRC_C:.c=.o)
OBJ_S  = $(SRC_S:.S=.o)
OBJ    = $(OBJ_C) $(OBJ_S)
OBJ_COM= $(SRC_COM:.c=.o)

.PHONY : all
all: $(LIBC)

$(LIBC): $(OBJ) $(OBJ_COM)
	$(AR) rcs $(LIBC) $(OBJ) $(notdir $(OBJ_COM))

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr $(OBJ) $(OBJ_COM) $(LIBC)