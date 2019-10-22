CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Winline -Og -fno-optimize-sibling-calls

PRIMIFY = primify
PRASM   = prasm
PRUN    = prun
PRERF   = prerf

PROGRAMS = quicksort heapsort mergesort quicksort_2 mergesort_2

all:
	make $(PROGRAMS)

../src.zip: Makefile mergesort_2.c heapsort.c quicksort.c quicksort.prime quicksort_2.prime mergesort.c io.c alloc.c data/ tests/ README run-performance.sh run-test.sh
	make clean
	cd .. && zip src.zip src/Makefile src/mergesort_2.c src/heapsort.c src/quicksort.c src/quicksort_2.prime src/quicksort.prime src/mergesort.c src/io.c src/alloc.c src/data/* src/tests/* src/README src/run-performance.sh src/run-test.sh

quicksort: quicksort.prime
	$(PRASM) $@.prime

quicksort_2: quicksort_2.prime
	$(PRASM) $@.prime

heapsort: heapsort.c
	$(CC) $(CFLAGS) -S $<
	$(PRIMIFY) $@.s
	$(PRASM) $@.prime

mergesort: mergesort.c
	$(CC) $(CFLAGS) -S $<
	$(PRIMIFY) $@.s
	$(PRASM) $@.prime

mergesort_2: mergesort_2.c
	$(CC) $(CFLAGS) -S $<
	$(PRIMIFY) $@.s
	$(PRASM) $@.prime

clean:
	rm -f $(foreach prg,$(PROGRAMS),$(prg).s)
	rm -f mergesort.prime mergesort_2.prime heapsort.prime
	rm -rf runs
	rm -rf test_runs
