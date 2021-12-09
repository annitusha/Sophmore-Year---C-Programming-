CC = gcc
CFLAGS = -g -Wall -std=c18
LDLIBS = -lm

TARGETS = gen-rand-points stat-points

all:			$(TARGETS)

gen-rand-points:	gen-rand-points.o
stat-points:		stat-points.o dyn-array.o

gen-rand-points.o:	gen-rand-points.c point2.h
stat-points.o:		stat-points.c dyn-array.h point2.h

clean:
		rm -f $(TARGETS) *.o *~
