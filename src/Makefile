CC=gcc
LIBS = -lsqlite3
CFLAGS=-I.
DEPS = breadbank.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: login.o server.o
	$(CC) login.o server.o -o server $(LIBS)

.PHONY: clean

clean:
	$(RM) *.o