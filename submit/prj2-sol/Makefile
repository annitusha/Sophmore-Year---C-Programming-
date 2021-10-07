TARGET = int-set

CFLAGS = -g -Wall -std=c18
LDFLAGS = -lm

$(TARGET):	main.o int-set.o int-set-strings.o
		$(CC) main.o int-set.o int-set-strings.o  $(LDFLAGS) -o $@

clean:		rm -f *.o $(TARGET) $(TARGET).tar.gz

depend:		$(CC) -MM $(CPPFLAGS) *.c


int-set.o:	int-set.c int-set.h

main.o:		main.c int-set.h int-set-strings.h

int-set-strings.o:	int-set-strings.c int-set-strings.h
