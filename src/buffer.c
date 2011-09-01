/*
 * Copyright (c) 2008, Natacha Porté
 * Copyright (c) 2011, Vicent Martí
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

#define BUFFER_MAX_ALLOC_SIZE (1024 * 1024 * 16) //16mb

#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MSVC compat */
#if defined(_MSC_VER)
#	define _buf_vsnprintf _vsnprintf
#else
#	define _buf_vsnprintf vsnprintf
#endif

/* bufcmp: buffer comparison */
int
bufcmp(const struct buf *a, const struct buf *b)
{
	size_t i = 0;
	size_t cmplen;

	if (a == b)
		return 0;

	if (!a)
		return -1;

	if (!b)
		return 1;

	cmplen = (a->size < b->size) ? a->size : b->size;

	while (i < cmplen && a->data[i] == b->data[i])
		++i;

	if (i < a->size) {
		if (i < b->size) return a->data[i] - b->data[i];
		return 1;
	} else {
		if (i < b->size) return -1;
		return 0;
	}
}

/* bufcmps: comparison of a string to a buffer */
int
bufcmps(const struct buf *a, const char *b)
{
	const size_t len = strlen(b);
	size_t cmplen = len;
	int r;

	if (!a || !a->size)
		return b ? 0 : -1;

	if (len < a->size)
		cmplen = a->size;

	r = strncmp(a->data, b, cmplen);

	if (r) return r;
	else if (a->size == len) return 0;
	else if (a->size < len) return -1;
	else return 1;
}

int
bufprefix(const struct buf *buf, const char *prefix)
{
	size_t i;

	for (i = 0; i < buf->size; ++i) {
		if (prefix[i] == 0)
			return 0;

		if (buf->data[i] != prefix[i])
			return buf->data[i] - prefix[i];
	}

	return 0;
}


/* bufdup: buffer duplication */
struct buf *
bufdup(const struct buf *src, size_t dupunit)
{
	size_t blocks;
	struct buf *ret;
	if (src == 0)
		return 0;

	ret = malloc(sizeof (struct buf));
	if (ret == 0)
		return 0;

	ret->unit = dupunit;
	ret->size = src->size;
	ret->ref = 1;
	if (!src->size) {
		ret->asize = 0;
		ret->data = 0;
		return ret;
	}

	blocks = (src->size + dupunit - 1) / dupunit;
	ret->asize = blocks * dupunit;
	ret->data = malloc(ret->asize);

	if (ret->data == 0) {
		free(ret);
		return 0;
	}

	memcpy(ret->data, src->data, src->size);
	return ret;
}

/* bufgrow: increasing the allocated size to the given value */
int
bufgrow(struct buf *buf, size_t neosz)
{
	size_t neoasz;
	void *neodata;
	if (!buf || !buf->unit || neosz > BUFFER_MAX_ALLOC_SIZE)
		return BUF_ENOMEM;

	if (buf->asize >= neosz)
		return BUF_OK;

	neoasz = buf->asize + buf->unit;
	while (neoasz < neosz)
		neoasz += buf->unit;

	neodata = realloc(buf->data, neoasz);
	if (!neodata)
		return BUF_ENOMEM;

	buf->data = neodata;
	buf->asize = neoasz;
	return BUF_OK;
}


/* bufnew: allocation of a new buffer */
struct buf *
bufnew(size_t unit)
{
	struct buf *ret;
	ret = malloc(sizeof (struct buf));

	if (ret) {
		ret->data = 0;
		ret->size = ret->asize = 0;
		ret->ref = 1;
		ret->unit = unit;
	}
	return ret;
}

/* bufnullterm: NULL-termination of the string array */
const char *
bufcstr(struct buf *buf)
{
	if (!buf || !buf->unit)
		return NULL;

	if (buf->size < buf->asize && buf->data[buf->size] == 0)
		return buf->data;

	if (buf->size + 1 <= buf->asize || bufgrow(buf, buf->size + 1) == 0) {
		buf->data[buf->size] = 0;
		return buf->data;
	}

	return NULL;
}

/* bufprintf: formatted printing to a buffer */
void
bufprintf(struct buf *buf, const char *fmt, ...)
{
	va_list ap;
	if (!buf || !buf->unit)
		return;

	va_start(ap, fmt);
	vbufprintf(buf, fmt, ap);
	va_end(ap);
}

/* bufput: appends raw data to a buffer */
void
bufput(struct buf *buf, const void *data, size_t len)
{
	if (!buf)
		return;

	if (buf->size + len > buf->asize && bufgrow(buf, buf->size + len) < 0)
		return;

	memcpy(buf->data + buf->size, data, len);
	buf->size += len;
}

/* bufputs: appends a NUL-terminated string to a buffer */
void
bufputs(struct buf *buf, const char *str)
{
	bufput(buf, str, strlen(str));
}


/* bufputc: appends a single char to a buffer */
void
bufputc(struct buf *buf, char c)
{
	if (!buf)
		return;

	if (buf->size + 1 > buf->asize && bufgrow(buf, buf->size + 1) < 0)
		return;

	buf->data[buf->size] = c;
	buf->size += 1;
}

/* bufrelease: decrease the reference count and free the buffer if needed */
void
bufrelease(struct buf *buf)
{
	if (!buf)
		return;

	if (--buf->ref == 0) {
		free(buf->data);
		free(buf);
	}
}


/* bufreset: frees internal data of the buffer */
void
bufreset(struct buf *buf)
{
	if (!buf)
		return;

	free(buf->data);
	buf->data = NULL;
	buf->size = buf->asize = 0;
}


/* bufset: safely assigns a buffer to another */
void
bufset(struct buf **dest, struct buf *src)
{
	if (src) {
		if (!src->asize) src = bufdup(src, 1);
		else src->ref += 1;
	}

	bufrelease(*dest);
	*dest = src;
}

/* bufslurp: removes a given number of bytes from the head of the array */
void
bufslurp(struct buf *buf, size_t len)
{
	if (!buf || !buf->unit || len <= 0)
		return;

	if (len >= buf->size) {
		buf->size = 0;
		return;
	}

	buf->size -= len;
	memmove(buf->data, buf->data + len, buf->size);
}

/* vbufprintf: stdarg variant of formatted printing into a buffer */
void
vbufprintf(struct buf *buf, const char *fmt, va_list ap)
{
	int n;

	if (buf == 0 || (buf->size >= buf->asize && bufgrow(buf, buf->size + 1)) < 0)
		return;

	n = _buf_vsnprintf(buf->data + buf->size, buf->asize - buf->size, fmt, ap);

	if (n < 0) {
#ifdef _MSC_VER
		n = _vscprintf(fmt, ap);
#else
		return;
#endif
	}

	if ((size_t)n >= buf->asize - buf->size) {
		if (bufgrow(buf, buf->size + n + 1) < 0)
			return;

		n = _buf_vsnprintf(buf->data + buf->size, buf->asize - buf->size, fmt, ap);
	}

	if (n < 0)
		return;

	buf->size += n;
}

