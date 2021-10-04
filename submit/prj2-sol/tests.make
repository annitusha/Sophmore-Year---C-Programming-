CFLAGS = -g -Wall -std=c18

CHECK_LIBS = -lcheck -lm -lrt -lpthread -lsubunit

VALGRIND = valgrind --leak-check=full

do-tests:	tests
		@if [ -n "$(CK_SUITE)" ] ; \
		then \
		  CK_RUN_SUITE=$(CK_SUITE) ./$< ; \
		else \
		  ./$<  ; \
		fi

valgrind-tests:	tests
		@if [ -n "$(CK_SUITE)" ] ; \
		then \
		  CK_RUN_SUITE=$(CK_SUITE) $(VALGRIND) ./$< ; \
		else \
		  $(VALGRIND) ./$<  ; \
		fi


tests:		tests.o int-set.o int-set-strings.o
		$(CC) $^ $(CHECK_LIBS) -o $@

int-set.o:	int-set.c int-set.h
int-set-strings.o: int-set-strings.c int-set-strings.h


