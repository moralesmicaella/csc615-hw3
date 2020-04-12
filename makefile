make: assignment3.c
	gcc -Wall -o assignment3 assignment3.c -lwiringPi -lpthread

clean:
	rm assignment3
