

CFLAGS = -O3

parser.so: parser.o scanner.o
	clang++ -shared -o $@ parser.o scanner.o

parser.o: src/parser.c
	clang -c -fPIC $(CFLAGS) -I src src/parser.c -o $@

scanner.o: src/scanner.cc
	clang++ -c -fPIC $(CFLAGS)  -I src src/scanner.cc -o $@

