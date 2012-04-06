CC=gcc
CCFLAGS=-Wall
LDFLAGS=-lX11 -L/usr/X11R6/lib

all: hostcodeIR

hostcodeIR: hostcodeIR.c
	$(CC) $(CCFLAGS) $(LDFLAGS) $< -o $@
