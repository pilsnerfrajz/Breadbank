CC=gcc
LIBS = -lsqlite3
CFLAGS=-I.
DEPS = breadbank.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: transfer.o login.o server.o
	$(CC) transfer.o login.o server.o -o server $(LIBS)

.PHONY: clean

clean:
	$(RM) *.o