mycalc: build/main.o lib/libcalc.a
	gcc -Wall -o mycalc build/main.o -L./lib -lcalc

build/main.o: src/main.c
	gcc -Wall -Werror -Wpedantic -o build/main.o -c src/main.c

lib/libcalc.a: build/functional.o
	ar rc lib/libcalc.a build/functional.o

build/functional.o: lib/src/functional.c lib/include/functional.h
	gcc -Wall -Werror -Wpedantic -o build/functional.o -c lib/src/functional.c

clean:
	rm -rf build/* lib/*.a mycalc

.PHONY: clean