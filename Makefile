CFLAGS = -c -g -O3 -Wall -Wextra -Wno-unused-parameter -Isrc

ifneq ($(OS),Windows_NT)
	CFLAGS += -fPIC
endif

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

# Libraries

libhoedown.so: libhoedown.so.1
	ln -f -s $^ $@

libhoedown.so.1: $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) -shared $^ -o $@

libhoedown.a: $(HOEDOWN_SRC)
	$(AR) rcs libhoedown.a $^

# Executables

hoedown: examples/hoedown.o $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) $^ -o $@

smartypants: examples/smartypants.o $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) $^ -o $@

# Perfect hashing

src/html_blocks.c: html_block_names.gperf
	gperf -L ANSI-C -N hoedown_find_block_tag -c -C -E -S 1 --ignore-case -m100 $^ > $@

# Testing

test: hoedown
	perl test/MarkdownTest_1.0.3/MarkdownTest.pl \
		--script=./hoedown --testdir=test/MarkdownTest_1.0.3/Tests --tidy

# Housekeeping

clean:
	$(RM) src/*.o examples/*.o
	$(RM) libhoedown.so libhoedown.so.1 libhoedown.a
	$(RM) hoedown smartypants hoedown.exe smartypants.exe

# Generic object compilations

%.o: src/%.c examples/%.c
	$(CC) $(CFLAGS) -o $@ $<
