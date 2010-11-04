CFLAGS = -Wall -Wextra -Isrc/ -Isrc/tests/
LFLAGS = -levent

OBJS = src/needlestack.o src/server.o src/filestack.o src/fileindex.o

all: needlestack tests

install: needlestack
	install needlestack /usr/local/bin

needlestack: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) -o needlestack

tests_dir:
	mkdir -p tests
tests: tests_dir fileindex_tests

fileindex_tests: src/fileindex.o src/tests/fileindex_tests.o
	$(CC) $(CFLAGS) $(LFLAGS) src/fileindex.o src/tests/fileindex_tests.o \
		-o tests/fileindex_tests
	./tests/fileindex_tests
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-@rm src/*.o src/tests/*.o needlestack tests/* 2>/dev/null >/dev/null
	-@rm -r tests 2>/dev/null >/dev/null