CC=gcc
CFLAGS=-Wall -Werror -Wpedantic -Wextra
HIDE=@

all: obj my_bash
	@echo Complete!

my_bash: obj/main.o
	$(HIDE)$(CC) obj/main.o -o my_bash

obj/main.o: src/main.c
	$(HIDE)$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj:
	mkdir -p obj

clean:
	rm -rf obj my_bash

rebuild: clean all

.PHONY: clean rebuild