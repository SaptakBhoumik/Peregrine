CC := g++
CFLAGS := -Wall -I ./Peregrine

OUTPUT := peregrine.elf

SRC := $(shell find -name '*.cpp' ! -path './tests/*')
OBJS := $(subst .cpp,.o,$(subst Peregrine,objs,$(SRC)))

.PHONY: clean all dirs

all: $(OUTPUT)
	./$(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS)

objs/%.o: Peregrine/%.cpp | dirs
	$(CC) $(CFLAGS) -c $< -o $@ 

dirs:
	mkdir -p $(shell dirname $(OBJS))

clean:
	rm -rf ./objs
	rm -f $(OUTPUT)