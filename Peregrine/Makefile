CC=g++
CFLAGS=-Wall -c -O3
all: lexer/lexer.cpp errors/errors.cpp
	 ${CC} ${CFLAGS} lexer/lexer.cpp -o lexer.o
	 ${CC} ${CFLAGS} errors/errors.cpp -o error.o
	 ${CC} ${CFLAGS} parser/parser.cpp -o parser.o
test_all: lexer/lexer.cpp lexer/lexer_test.cpp  errors/errors.cpp
	 ${CC} ${CFLAGS} lexer/lexer.cpp -o lexer.o
	 ${CC} ${CFLAGS} lexer/lexer_test.cpp -o lexer_test.o
	 ${CC} ${CFLAGS} errors/errors.cpp -o error.o
	 ${CC} ${CFLAGS} parser/parser.cpp -o parser.o
	 ${CC} error.o lexer_test.o lexer.o -o temp
	 ./temp
	 ${CC} parser.o error.o lexer_test.o lexer.o -o temp
	 ./temp
clean:
	rm *.o
	rm temp