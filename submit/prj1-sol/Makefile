
TARGET = morse-encode

CFLAGS = -g -Wall -std=c18
LDFLAGS = -lm

$(TARGET):	main.o file-utils.o morse.o
		$(CC) main.o file-utils.o morse.o  $(LDFLAGS) -o $@

clean:		rm -f *.o $(TARGET) $(TARGET).tar.gz

depend:		$(CC) -MM $(CPPFLAGS) *.c


morse-decode:	morse-encode
		ln -s -f $< $@



file-utils.o:	file-utils.c file-utils.h

morse.o:        morse.c morse.h

main.o:         main.c file-utils.h morse.h

tests.o:	tests.c morse.h tests.h

