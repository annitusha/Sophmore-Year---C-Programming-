PROJECT = key-value
SRC_FILES =  key-value.c \
	main.c\
	Makefile\
	process-file.h\
	key-value.h\
	process-file.c\
	README


CFLAGS = -g -Wall -std=c18
LDFLAGS = -lm

$(PROJECT):	main.o key-value.o process-file.o
		$(CC) main.o key-value.o process-file.o $(LDFLAGS) -o $@

clean:
		rm -f *.o $(PROJECT) $(PROJECT).tar.gz

depend:
		$(CC) -MM $(CPPFLAGS) *.c

dist:			$(PROJECT).tar.gz

$(PROJECT).tar.gz:		$(SRC_FILES)
				tar -cf $(PROJECT).tar $(SRC_FILES)
				gzip -f $(PROJECT).tar

key-value.o: key-value.c key-value.h
main.o: main.c process-file.h
process-file.o: process-file.c key-value.h process-file.h





