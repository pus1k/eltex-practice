all: mycalc dmycalc

rebuild: clean mycalc dmycalc


mycalc: build/main.o lib/libcalc.a
	gcc -o mycalc build/main.o -L./lib -lcalc

build/main.o: src/main.c
	gcc -Wall -Werror -Wpedantic -o build/main.o -c src/main.c

lib/libcalc.a: build/functional.o
	ar rc lib/libcalc.a build/functional.o

build/functional.o: lib/static/src/functional.c lib/static/include/functional.h
	gcc -Wall -Werror -Wpedantic -o build/functional.o -c lib/static/src/functional.c



dmycalc: build/dmain.o lib/addition.so lib/substraction.so lib/multiplication.so lib/division.so
	gcc -o dmycalc build/dmain.o -ldl

build/dmain.o: src/dmain.c
	gcc -Wall -Werror -Wpedantic -o build/dmain.o -c src/dmain.c -g

lib/addition.so: build/addition.o
	gcc -shared build/addition.o -o lib/addition.so

build/addition.o: lib/addition/addition.c lib/addition/addition.h 
	gcc -Wall -Werror -Wpedantic -fPIC lib/addition/addition.c -c -o build/addition.o

lib/substraction.so: build/substraction.o
	gcc -shared build/substraction.o -o lib/substraction.so

build/substraction.o: lib/substraction/substraction.c lib/substraction/substraction.h
	gcc -Wall -Werror -Wpedantic -fPIC lib/substraction/substraction.c -c -o build/substraction.o

lib/multiplication.so: build/multiplication.o
	gcc -shared build/multiplication.o -o lib/multiplication.so
	
build/multiplication.o: lib/multiplication/multiplication.c lib/multiplication/multiplication.h
	gcc -Wall -Werror -Wpedantic -fPIC lib/multiplication/multiplication.c -c -o build/multiplication.o

lib/division.so: build/division.o
	gcc -shared build/division.o -o lib/division.so
	
build/division.o: lib/division/division.c lib/division/division.h
	gcc -Wall -Werror -Wpedantic -fPIC lib/division/division.c -c -o build/division.o



clean:
	rm -rf build/* lib/*.a lib/*.so mycalc dmycalc

.PHONY: clean