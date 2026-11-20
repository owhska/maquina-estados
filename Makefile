CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Werror -pedantic -Iinclude
SRC     = src/maquina.c
TEST    = tests/test_maquina.c

.PHONY: all test clean

all: test

test: $(SRC) $(TEST) include/maquina.h | build
	$(CC) $(CFLAGS) $(SRC) $(TEST) -o build/test_maquina
	./build/test_maquina

build:
	mkdir -p build

clean:
	rm -rf build
