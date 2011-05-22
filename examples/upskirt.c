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

struct buf *read_test_file(const char *base)
{
    char *path = str_cat(test_files_dir, base);
    struct buf *b = read_file(path);
    free((void*)base);
    free(path);
    return b;
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

int
main(int argc, char **argv)
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
    return 0;
}

/* vim: set filetype=c: */
