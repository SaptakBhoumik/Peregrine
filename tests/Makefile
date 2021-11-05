CC := g++

TEST_OUTPUT := compiler_test.elf

DOCTEST := compiler/doctest.h

TEST_SRC := $(shell find -name '*.cpp')
TEST_OBJS := $(subst .cpp,.o,$(TEST_SRC))

OBJS := $(shell find .. -path '../objs/*.o' ! -name 'main.o')

.PHONY: clean all

all: $(TEST_OUTPUT) 
	./$(TEST_OUTPUT)

$(TEST_OUTPUT): $(DOCTEST) $(TEST_OBJS) $(OBJS)
	$(CC) -o $(TEST_OUTPUT) $(TEST_OBJS) $(OBJS)

$(DOCTEST):
	wget https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h
	mv doctest.h compiler/doctest.h

%.o: %.cpp
	$(CC) -I ../Peregrine -c $< -o $@

clean:
	rm -f *.o
	rm -f $(TEST_OUTPUT)
