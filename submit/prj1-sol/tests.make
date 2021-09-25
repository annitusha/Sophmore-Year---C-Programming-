TARGETS =  do-tests8 do-tests16

CC = gcc
CFLAGS = -Wall -DDO_TESTS

CHECK_LIBS = -lcheck -lm -lrt -lpthread -lsubunit


C_SRCS =  tests.c  morse.c

SRCS = $(C_SRCS) morse.h tests.h

all:		$(TARGETS)

do-tests8:	tests8
		./tests8

do-tests16:	tests16
		./tests16

tests8:		$(SRCS)
		$(CC) $(CFLAGS) $(C_SRCS) $(CHECK_LIBS) -o $@

tests16:	$(SRCS)
		$(CC) $(CFLAGS) -DBYTE_SIZE=2 \
                      $(C_SRCS) $(CHECK_LIBS) -o $@


clean:
		rm -f *~ tests8 tests16
