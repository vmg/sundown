CFLAGS = -c -g -O3 -fpic -Wall -Werror -Wsign-compare -Isrc
LDFLAGS = -g -O3 -Wall -Werror

HOEDOWN_SRC=\
	src/autolink.o \
	src/buffer.o \
	src/escape.o \
	src/html.o \
	src/html_blocks.o \
	src/html_smartypants.o \
	src/markdown.o \
	src/stack.o

.PHONY:		all test clean

all:		libhoedown.so hoedown smartypants

# libraries

libhoedown.so: libhoedown.so.1
	ln -f -s $^ $@

libhoedown.so.1: $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) -shared $^ -o $@

# executables

hoedown: examples/hoedown.o $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) $^ -o $@

smartypants: examples/smartypants.o $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) $^ -o $@

# perfect hashing

src/html_blocks.c: html_block_names.gperf
	gperf -L ANSI-C -N hoedown_find_block_tag -c -C -E -S 1 --ignore-case -m100 $^ > $@

# testing

test: hoedown
	perl test/MarkdownTest_1.0.3/MarkdownTest.pl \
		--script=./hoedown --testdir=test/MarkdownTest_1.0.3/Tests --tidy

# housekeeping

clean:
	$(RM) -f src/*.o examples/*.o
	$(RM) -f libhoedown.so libhoedown.so.1 hoedown smartypants
	$(RM) -f hoedown.exe smartypants.exe

# generic object compilations

%.o: src/%.c examples/%.c
	$(CC) $(CFLAGS) -o $@ $<
