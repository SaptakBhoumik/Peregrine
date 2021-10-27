CC=g++
CFLAGS=-Wall -c -O3
all: 
	 ${CC} ${CFLAGS} lexer/lexer.cpp -o lexer.o
	 ${CC} ${CFLAGS} errors/errors.cpp -o error.o
	 ${CC} ${CFLAGS} parser/parser.cpp -o parser.o
test_all:
	 ${CC} ${CFLAGS} lexer/lexer.cpp -o lexer.o
	 ${CC} ${CFLAGS} lexer/lexer_test.cpp -o lexer_test.o
	 ${CC} ${CFLAGS} errors/errors.cpp -o error.o
	 ${CC} ${CFLAGS} parser/parser.cpp -o parser.o
	 ${CC} ${CFLAGS} parser/literals.cpp -o literals.o
	 ${CC} ${CFLAGS} parser/parser_test.cpp -o parser_test.o
	 ${CC}  error.o lexer_test.o lexer.o -o temp
	 ./temp
	 ${CC} parser.o parser_test.o error.o lexer.o literals.o -o temp
	 ./temp
clean:
	rm *.o
	rm temp