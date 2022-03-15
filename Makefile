CFLAGS= -g -O2 -Wall -pedantic -ansi
FILE = proj1

run : 
	gcc $(CFLAGS) $(FILE).c -o $(FILE).out
	./$(FILE).out