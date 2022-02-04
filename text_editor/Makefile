all: ted

rebuild: clean ted

ted: build/editor.o build/moves.o build/editor_functions.o
	gcc -o ted build/editor.o build/moves.o build/editor_functions.o -lncurses

build/editor.o: src/editor.c
	gcc -Wall -Werror -Wpedantic -o build/editor.o -c src/editor.c -lncurses

build/moves.o: src/moves.c
	gcc -Wall -Werror -Wpedantic -o build/moves.o -c src/moves.c -lncurses

build/editor_functions.o: src/editor_functions.c
	gcc -Wall -Werror -Wpedantic -o build/editor_functions.o -c src/editor_functions.c -lncurses

clean:
	rm -rf build/* ted

.PHONY: clean