CC = gcc
CFLAGS = -g -Og -Wall -std=c18

FEED = feed-cookie-monster
TARGETS = $(FEED)

OBJS = $(FEED).o cookie-wrapper.obj

all:			$(TARGETS)

$(FEED):		$(OBJS)
			$(CC) $^ -o $@

ifeq ($(GEN),1)

cookie-wrapper.obj:	cookie-wrapper.o
			cp $< $@

endif

$(FEED).o:		$(FEED).c cookie-wrapper.h

cookie-wrapper.o:	cookie-wrapper.c cookie-wrapper.h
			$(CC) -Og -Wall -std=c18 -c $<

clean:
			rm -f $(FEED) *.o *~

