CFLAGS= -Wall -Wextra -Werror -ansi -pedantic
FILE = proj1

run : build
	./$(FILE).out

test: build
	./test.sh ./$(FILE).out ./tests/community_tests
	./test.sh ./$(FILE).out ./tests/tests

build:
	gcc $(CFLAGS) ./src/*.c -o $(FILE).out

zip : 
	cd ./src &&	zip $(FILE).zip *.c *.h
	mv ./src/$(FILE).zip ./$(FILE).zip

clean :
	rm vgcore*