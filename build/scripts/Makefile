# Makefile for the sourceinclusion build utility.
#

CPP = g++
OPTIM = -O2 -Wall -fomit-frame-pointer
INCDIRS = -I../../include

CFLAGS = $(OPTIM) $(INCDIRS)

all: sourceinclusion

sourceinclusion: sourceinclusion.cpp ../../include/Inventor/soconfig.h ../../include/Inventor/confdep.h
	$(CPP) $(CFLAGS) -o sourceinclusion sourceinclusion.cpp

clean:
	rm -f core *.o *~ sourceinclusion
