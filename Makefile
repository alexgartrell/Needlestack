CFLAGS = -Wall -Wextra -Isrc/ -Isrc/tests/
LFLAGS = -levent

all: mystack tests

mystack: src/mystack.o src/server.o
	$(CC) $(CFLAGS) $(LFLAGS) src/mystack.o src/server.o -o mystack

tests_dir:
	mkdir -p tests
tests: tests_dir indextrie_tests

indextrie_tests: src/indextrie.o src/tests/indextrie_tests.o
	$(CC) $(CFLAGS) $(LFLAGS) src/indextrie.o src/tests/indextrie_tests.o \
		-o tests/indextrie_tests
	./tests/indextrie_tests
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-@rm src/*.o src/tests/*.o mystack tests/* 2>/dev/null >/dev/null
	-@rm -r tests 2>/dev/null >/dev/null