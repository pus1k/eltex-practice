mycalc: libcalc build/main.o
    gcc -Wall -o mycalc build/main.o -L. -lcalc

build/main.o: src/main.c
    gcc -Wall -Werror -Wpedantic -o build/main.o -c src/main.c

libcalc: build/functional.o
    ar rc lib/libcalc.a build/functional.o

build/functional.o: lib/src/functional.c
    gcc -Wall -Werror -Wpedantic -o build/functional.o lib/src/functional.c

clean:
    rm -rf build/* mycalc

.PHONY: clean