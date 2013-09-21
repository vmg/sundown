# Makefile

DEPDIR=depends

# "Machine-dependant" options
#MFLAGS=-fPIC

CFLAGS=-c -g -O3 -fPIC -Wall -Werror -Wsign-compare -Isrc
LDFLAGS=-g -O3 -Wall -Werror 


HOEDOWN_SRC=\
	src/autolink.o \
	src/buffer.o \
	src/escape.o \
	src/html.o \
	src/html_blocks.o \
	src/html_smartypants.o \
	src/markdown.o \
	src/stack.o

all:		libhoedown.so hoedown smartypants html_blocks

.PHONY:		all html_blocks test clean

# libraries

libhoedown.so:	libhoedown.so.1
	ln -f -s $^ $@

libhoedown.so.1: $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) -shared $^ -o $@

# executables

hoedown:	examples/hoedown.o $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) $^ -o $@

smartypants: examples/smartypants.o $(HOEDOWN_SRC)
	$(CC) $(LDFLAGS) $^ -o $@

# perfect hashing

src/html_blocks.c: html_block_names.gperf
	gperf -L ANSI-C -N hoedown_find_block_tag -c -C -E -S 1 --ignore-case -m100 $^ > $@

test: hoedown
	perl test/MarkdownTest_1.0.3/MarkdownTest.pl \
		--script=./hoedown --testdir=test/MarkdownTest_1.0.3/Tests --tidy

# housekeeping
clean:
	rm -f src/*.o examples/*.o
	rm -f libhoedown.so libhoedown.so.1 hoedown smartypants
	rm -f hoedown.exe smartypants.exe
	rm -rf $(DEPDIR)


# dependencies

include $(wildcard $(DEPDIR)/*.d)


# generic object compilations

%.o:	src/%.c examples/%.c
	@mkdir -p $(DEPDIR)
	@$(CC) -MM $< > $(DEPDIR)/$*.d
	$(CC) $(CFLAGS) -o $@ $<

