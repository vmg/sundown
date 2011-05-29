/*
 * Copyright (c) 2011, Vicent Marti
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "markdown.h"
#include "html.h"
#include "buffer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64

const char *test_files_dir = "..\\..\\peg-markdown\\MarkdownTest_1.0.3\\Tests\\";

const char *test_files[] = {
    "Nested blockquotes",
/*    NULL, */
    "Tidyness",
    "Strong and em together",
    "Amps and angle encoding",
    "Auto links",
    "Backslash escapes",
    "Blockquotes with code blocks",
    "Code Blocks",
    "Code Spans",
    "Hard-wrapped paragraphs with list-like lines",
    "Horizontal rules",
    "Inline HTML (Advanced)",
    "Inline HTML (Simple)",
    "Inline HTML comments",
    "Links, inline style",
    "Links, reference style",
    "Links, shortcut references",
    "Literal quotes in titles",
    "Markdown Documentation - Basics",
    "Markdown Documentation - Syntax",
    "Ordered and unordered lists",
    "Tabs",
    NULL
};

char *str_cat(const char *s1, const char *s2)
{
    size_t l1 = strlen(s1);
    size_t l2 = strlen(s2);
    char *s = (char*)malloc(l1 + l2 + 1);
    strcpy(s, s1);
    strcpy(s + l1, s2);
    s[l1+l2] = 0;
    return s;
}

struct buf *read_fo(FILE *f)
{
	size_t ret;
	struct buf *b = bufnew(READ_UNIT);
	bufgrow(b, READ_UNIT);
	while ((ret = fread(b->data + b->size, 1, b->asize - b->size, f)) > 0) {
		b->size += ret;
		bufgrow(b, b->size + READ_UNIT);
	}
    return b;
}

struct buf *read_file(const char *file)
{
    struct buf *b;
    FILE *f = fopen(file, "r");
    assert(f);
    b = read_fo(f);
    fclose(f);
    return b;
}

void write_buf_to_file( const char *file, struct buf *b)
{
    FILE *f = fopen(file, "w");
    assert(f);
    fwrite(b->data, 1, b->size, f);
    fclose(f);
}

struct buf *read_test_file(const char *base)
{
    char *path = str_cat(test_files_dir, base);
    struct buf *b = read_file(path);
    free((void*)base);
    free(path);
    return b;
}

void write_buf(struct buf *b, const char *base)
{
    char *path = str_cat(test_files_dir, base);
    write_buf_to_file(path, b);
    free((void*)base);
    free(path);
}

struct buf* render_md_to_html(struct buf *md)
{
    struct mkd_renderer renderer;
    struct buf *ob = bufnew(OUTPUT_UNIT);
    upshtml_renderer(&renderer, 0);
    //ups_markdown(ob, md, &renderer, 0xFF);
    ups_markdown(ob, md, &renderer, 0);
    upshtml_free_renderer(&renderer);
    return ob;
}

int iswsp(char c) {
    return (c == ' ') || (c == '\t') || (c == '\n');
}

static void bufstrip(struct buf *b)
{
    size_t len;
    char *s = b->data;
    char *d = s;
    char *end = s + b->size;
    int at_line_start = 1;
    char c;
    while (s < end) {
        if (at_line_start) {
            if (iswsp(*s)) {
                s++;
                continue;
            }
            at_line_start = 0;
        }
        c = *s++;
        *d++ = c;
        if (c == '\n')
            at_line_start = 1;
    }
    s = b->data;
    len = d - s;
    if ((len > 0) && ('\n' == s[len-1])) {
        --len;
    }
    s[len] = 0;
    b->size = len;
}

int test_file(const char *basename)
{
    int cmp;
    struct buf *mdbuf = read_test_file(str_cat(basename, ".text"));
    struct buf *htmlbufref = read_test_file(str_cat(basename, ".html"));
    struct buf *htmlbuf = render_md_to_html(mdbuf);
    write_buf(htmlbuf, str_cat(basename, "_upskirt_ref.html"));
    bufstrip(htmlbufref);
    bufstrip(htmlbuf);
    cmp = bufcmp(htmlbuf, htmlbufref);
    if (cmp != 0) {
        printf("Error: failure in test file '%s'\n", basename);
    } else {
        printf("Ok: file '%s' passed\n", basename);
    }
    bufrelease(mdbuf);
    bufrelease(htmlbuf);
    bufrelease(htmlbufref);
    return cmp;
}

void printwithlen(const char *s, int len)
{
    //printf("%d:%s\n", len, s);
    printf("%s\n", s);
}

void pprint(const char *s, const char *e)
{
    struct buf *b = bufnew(e-s+128);
    const char *s1 = s;
    while (s < e) {
        if (*s == '\n') {
            bufputs(b, "\\n");
        } else if (*s == '\t') {
            bufputs(b, "\\t");
        } else if (*s == '\r') {
            bufputs(b, "\\r");
        } else {
            bufputc(b, *s);
        }
        ++s;
    }
    bufputc(b, 0);
    printwithlen(b->data, (int)b->size - 1);
}

void test_str(const char *s)
{
    struct buf *mdbuf;
    struct buf *htmlbuf;
    size_t sl = strlen(s);
    mdbuf = bufnew(sl + 16);
    bufgrow(mdbuf, sl + 16);
    memcpy(mdbuf->data, s, sl);
    mdbuf->size = sl;
    htmlbuf = render_md_to_html(mdbuf);
    //bufgrow(htmlbuf, htmlbuf->size + 16);
    //htmlbuf->data[htmlbuf->size] = 0;
    pprint(s, s + strlen(s));
    pprint(htmlbuf->data, htmlbuf->data + htmlbuf->size);
    bufrelease(mdbuf);
    bufrelease(htmlbuf);
}

void test_all_files()
{
    int i, ok = 0, fail = 0;
    for (i=0; test_files[i] != NULL; i++) {
        int res = test_file(test_files[i]);
        if (0 == res)
            ++ok;
        else
            ++fail;
    }
    printf("ok: %d, fail: %d\n", ok, fail);
}

void test_strings()
{
    test_str("foo");
    test_str("~foo~");
}

int main(int argc, char **argv)
{
    // test_all_files();
    test_strings();
    return 0;
}

/* vim: set filetype=c: */
