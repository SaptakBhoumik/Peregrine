CC=g++
CFLAGS=-Wall -c -O3
all: lexer/lexer.cpp errors/errors.cpp
	 ${CC} ${CFLAGS} lexer/lexer.cpp -o lexer.o
	 ${CC} ${CFLAGS} errors/errors.cpp -o error.o
test_lexer: lexer/lexer.cpp lexer/lexer_test.cpp  errors/errors.cpp
	 ${CC} ${CFLAGS} lexer/lexer.cpp -o lexer.o
	 ${CC} ${CFLAGS} lexer/lexer_test.cpp -o lexer_test.o
	 ${CC} ${CFLAGS} errors/errors.cpp -o error.o
	 ${CC} error.o lexer_test.o lexer.o -o temp
	 ./temp
clean:
	rm *.o
	rm temp