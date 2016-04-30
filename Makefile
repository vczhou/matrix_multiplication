CFLAGS=-g -std=gnu99 -O3 -Werror -Wall

main : main.o cstuff.o pthread_solution.o Makefile
	gcc $(CFLAGS) -o main main.o cstuff.o pthread_solution.o  -lpthread

%.o : %.c Makefile
	gcc $(CFLAGS) -MD -c $*.c

run : main
	./main

test : run

clean :
	rm -f *.d
	rm -f *.o
	rm -f main
	rm -f freq.txt

-include *.d
