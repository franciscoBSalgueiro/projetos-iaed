CFLAGS= -Wall -Wextra -Werror -ansi -pedantic
FILE = proj2

run : build
	./$(FILE).out

test: build
	./test.sh ./$(FILE).out ./tests/p1-mooshak-tests
	./test.sh ./$(FILE).out ./tests/public_tests
	./test.sh ./$(FILE).out ./tests/extensive_test
	./test.sh ./$(FILE).out ./tests/community_tests
	./test.sh ./$(FILE).out ./tests/custom_tests

build:
	gcc $(CFLAGS) ./src/*.c -o $(FILE).out

zip : 
	cd ./src &&	zip $(FILE).zip *.c *.h
	mv ./src/$(FILE).zip ./$(FILE).zip

clean :
	rm vgcore*