CC=gcc
CFLAGS=-Wall -Werror -Wpedantic -Wextra
LDFLAGS=-lncurses

ted: obj obj/main.o obj/func.o
	$(CC) $(CFLAGS) -o ted obj/main.o obj/func.o $(LDFLAGS)

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o obj/main.o -c src/main.c $(LDFLAGS)

obj/func.o: src/func.c
	$(CC) $(CFLAGS) -o obj/func.o -c src/func.c $(LDFLAGS)

obj:
	mkdir -p obj

clean:
	rm -rf obj ted

rebuild: clean ted

.PHONY: clean rebuild