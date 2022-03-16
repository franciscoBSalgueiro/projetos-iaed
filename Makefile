CFLAGS= -Wall -Wextra -Werror -ansi -pedantic
FILE = proj1

run : 
	gcc $(CFLAGS) $(FILE).c -o $(FILE).out
	./$(FILE).out