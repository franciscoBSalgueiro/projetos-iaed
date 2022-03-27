CFLAGS= -Wall -Wextra -Werror -ansi -pedantic
FILE = proj1

run : 
	gcc $(CFLAGS) ./src/*.c -o $(FILE).out
	./$(FILE).out

zip : 
	cd ./src &&	zip $(FILE).zip *.c *.h
	mv ./src/$(FILE).zip ./$(FILE).zip
