all: obj mycalc dmycalc

rebuild: clean all


mycalc: obj/main.o lib/libcalc.a
	gcc -o mycalc obj/main.o -L./lib -lcalc

obj/main.o: src/main.c
	gcc -Wall -Werror -Wpedantic -o obj/main.o -c src/main.c

lib/libcalc.a: obj/functional.o
	ar rc lib/libcalc.a obj/functional.o

obj/functional.o: lib/static/src/functional.c lib/static/include/functional.h
	gcc -Wall -Werror -Wpedantic -o obj/functional.o -c lib/static/src/functional.c



dmycalc: obj/dmain.o lib/addition.so lib/substraction.so lib/multiplication.so lib/division.so
	gcc -o dmycalc obj/dmain.o -ldl

obj/dmain.o: src/dmain.c
	gcc -Wall -Werror -Wpedantic -o obj/dmain.o -c src/dmain.c -g

lib/addition.so: obj/addition.o
	gcc -shared obj/addition.o -o lib/addition.so

obj/addition.o: lib/addition/addition.c lib/addition/addition.h 
	gcc -Wall -Werror -Wpedantic -fPIC lib/addition/addition.c -c -o obj/addition.o

lib/substraction.so: obj/substraction.o
	gcc -shared obj/substraction.o -o lib/substraction.so

obj/substraction.o: lib/substraction/substraction.c lib/substraction/substraction.h
	gcc -Wall -Werror -Wpedantic -fPIC lib/substraction/substraction.c -c -o obj/substraction.o

lib/multiplication.so: obj/multiplication.o
	gcc -shared obj/multiplication.o -o lib/multiplication.so
	
obj/multiplication.o: lib/multiplication/multiplication.c lib/multiplication/multiplication.h
	gcc -Wall -Werror -Wpedantic -fPIC lib/multiplication/multiplication.c -c -o obj/multiplication.o

lib/division.so: obj/division.o
	gcc -shared obj/division.o -o lib/division.so
	
obj/division.o: lib/division/division.c lib/division/division.h
	gcc -Wall -Werror -Wpedantic -fPIC lib/division/division.c -c -o obj/division.o

obj:
	mkdir -p obj

clean:
	rm -rf obj lib/*.a lib/*.so mycalc dmycalc

.PHONY: clean