
CURDIR := $(shell pwd)
TOPDIR  = $(CURDIR)/../..
include $(TOPDIR)/rules.mk

INCDIR  = $(INCSYS) $(INCBSP) $(INCLIBC) $(INCFINSH) \
		  ./include ./server 
INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))

CFLAGS += $(INCLUDE)
AFLAGS += $(INCLUDE)

SRC_COM     = $(wildcard common/*.c)
SRC_SERVER  = $(wildcard server/*.c)
SRC_WIDGETS = $(wildcard widgets/*.c)
SRC         = $(SRC_SERVER) $(SRC_WIDGETS)

OBJ_SERVER  = $(SRC_SERVER:.c=.o)
OBJ_WIDGETS = $(SRC_WIDGETS:.c=.o)
OBJ         = $(OBJ_SERVER) $(OBJ_WIDGETS)
OBJ_COM     = $(SRC_COM:.c=.o)

.PHONY : all
all: $(LIBGUI)

$(LIBGUI): $(OBJ) $(OBJ_COM)
	$(AR) rcs $(LIBGUI) $(OBJ) $(OBJ_COM)

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr $(OBJ) $(OBJ_COM) $(LIBGUI)