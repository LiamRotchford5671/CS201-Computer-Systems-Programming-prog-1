CFLAGS = -Wall -ansi -pedantic -O0

hw1: prog1.c implement.c
	gcc -o hw1 prog1.c implement.c

clean: 
	rm hw1 
