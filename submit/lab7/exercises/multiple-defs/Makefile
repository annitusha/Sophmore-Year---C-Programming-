CFLAGS = -g -Wall -std=c18

multiple-defs: 	main.o f.o
		$(CC) main.o f.o $(LDFLAGS) -o $@

.PHONY:		clean
clean:
		rm -f *.o multiple-defs
