COURSE = cs220

COURSE_DIR = $(HOME)/$(COURSE)

TARGET = fn-trace

CFLAGS = -g -Wall -I $(COURSE_DIR)/include -Og -std=c18 -fPIC

LDLIBS =  -l cs220 -lcapstone -ldl
LDFLAGS = -L $(COURSE_DIR)/lib

OBJS = \
  fn-trace.o \
  main.o \
  x86-64_lde.o

$(TARGET):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

clean:		
		rm -f $(OBJS) $(TARGET) fns fns.o *~


DEPENDS:
		gcc $(CFLAGS) -MM *.c

#make DEPENDS output with cs220/include dependencies removed
fn-trace.o: fn-trace.c fn-trace.h x86-64_lde.h 
main.o: main.c fn-trace.h
x86-64_lde.o: x86-64_lde.c x86-64_lde.h 
