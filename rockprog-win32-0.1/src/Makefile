
## Created by Anjuta

CC = gcc
CFLAGS = -g -Wall -std=c99
OBJECTS = rockprog.o softrock.o
INCFLAGS = -I/include
LDFLAGS = -Xlinker --enable-auto-import -Xlinker -L/lib
LIBS = -lusb -lpopt

all: rockprog0

rockprog0: $(OBJECTS)
	$(CC) -o rockprog0 $(OBJECTS) $(LDFLAGS) $(LIBS)

.SUFFIXES:
.SUFFIXES:	.c .cc .C .cpp .o

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f *.o

.PHONY: all
.PHONY: count
.PHONY: clean
