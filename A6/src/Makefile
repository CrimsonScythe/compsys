GCC=gcc -O3 -g -Wall -Wextra -pedantic -std=gnu11
LD_FLAGS= -lpthread

all: peer name_server

csapp.o: csapp.c csapp.h
	$(GCC) -c $< -o $@

common.o: common.c common.h
	$(GCC) -c $< -o $@

peer: peer.c peer.h common.o csapp.o
	$(GCC) $< *.o -o $@ $(LD_FLAGS)

name_server: name_server.c name_server.h common.o csapp.o
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

zip: ../src.zip

../src.zip: clean
	cd .. && zip -r src.zip src/Makefile src/*.c src/*.h

clean:
	rm -rf *.o peer name_server vgcore*
