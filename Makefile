all: rtclock.o parSort.c
	gcc -Wall -g -o parSort parSort.c rtclock.o -lpthread

rtclock.o: rtclock.h rtclock.c
	gcc -Wall -g -c rtclock.c

clean:
	rm -rf *.o parSort
