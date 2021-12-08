COURSE = cs220

TARGET = cache-sim

CPPFLAGS = -I $(HOME)/$(COURSE)/include
CFLAGS = -g -Wall -std=c18

LIBDIR = $$HOME/$(COURSE)/lib
LIB = cs220

LDFLAGS = -L $(LIBDIR)
LDLIBS = -l$(LIB)

OBJS = \
  cache-sim.o \
  main.o 

$(TARGET):	$(OBJS)
		$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -Wl,-rpath=$(LIBDIR) -lm -o $@
clean:		
		rm -f $(OBJS) $(TARGET) *~


