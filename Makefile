CC=g++
CFLAGS= -Wall -g -std=c++17 -fprofile-arcs -ftest-coverage

all: tests
tests.o: tests.cpp
	${CC} -c tests.cpp -o tests.o ${CFLAGS}
hashtables.o: hashtables.cpp
	${CC} -c hashtables.cpp -o hashtables.o ${CFLAGS}
tests: tests.o hashtables.o
	${CC} hashtables.o tests.o -o tests ${CFLAGS}
clean:
	@rm -rf *.o *.gcno *.gcda *.gcov tests 2&>/dev/null
tar:
	tar czvf ../hashtables_cpp.tgz ../hashtables_cpp

