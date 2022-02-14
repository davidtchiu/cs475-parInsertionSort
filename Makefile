all: rtclock.o parSort.c
	gcc -Wall -o parSort parSort.c rtclock.o -lpthread

rtclock.o: rtclock.h rtclock.c
	gcc -Wall -c rtclock.c

clean:
	rm -rf *.o parSort
