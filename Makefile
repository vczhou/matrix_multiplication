CFLAGS=-g -std=c99 -O3 -Werror -Wall

main : main.o cstuff.o Makefile
	gcc $(CFLAGS) -o main main.o cstuff.o

%.o : %.c Makefile
	gcc $(CFLAGS) -MD -c $*.c

%.o : %.S Makefile
	gcc $(CFLAGS) -MD -c $*.S

run : main
	./main

test : run

clean :
	rm -f *.d
	rm -f *.o
	rm -f main
	rm -f freq.txt

-include *.d
