# Makefile

# Copyright (c) 2009, Natacha Porté
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

DEPDIR=depends
CFLAGS=-c -g -O3 -Wall -Werror -Isrc -Irender -fPIC
LDFLAGS=-g -O3 -Wall -Werror
CC=gcc

all:		libupskirt.so upskirt smartypants

.PHONY:		all clean

# libraries

libupskirt.so:	libupskirt.so.1
	ln -f -s $^ $@

libupskirt.so.1: src/markdown.o src/array.o src/buffer.o
	$(CC) $(LDFLAGS) -shared -Wl $^ -o $@

# executables

upskirt:	examples/upskirt.o src/markdown.o src/array.o src/buffer.o render/html.o render/html_smartypants.o
	$(CC) $(LDFLAGS) $^ -o $@

smartypants: examples/smartypants.o src/buffer.o render/html_smartypants.o
	$(CC) $(LDFLAGS) $^ -o $@

# housekeeping
clean:
	rm -f src/*.o render/*.o examples/*.o
	rm -f libupskirt.so libupskirt.so.1 upskirt
	rm -rf $(DEPDIR)


# dependencies

include $(wildcard $(DEPDIR)/*.d)


# generic object compilations

%.o:	src/%.c examples/%.c render/%.c
	@mkdir -p $(DEPDIR)
	@$(CC) -MM $< > $(DEPDIR)/$*.d
	$(CC) $(CFLAGS) -o $@ $<

