CFLAGS= -Wall -Wextra -Werror -ansi -pedantic
FILE = proj1

run : 
	gcc $(CFLAGS) *.c -o $(FILE).out
	./$(FILE).out