/* buffer.h - simple, fast buffers */

#ifndef HOEDOWN_BUFFER_H
#define HOEDOWN_BUFFER_H

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#define __attribute__(x)
#define inline
#endif

typedef enum {
	HOEDOWN_BUF_OK = 0,
	HOEDOWN_BUF_ENOMEM = -1,
} hoedown_buferror_t;

/* struct hoedown_buffer: character array buffer */
struct hoedown_buffer {
	uint8_t *data;		/* actual character data */
	size_t size;	/* size of the string */
	size_t asize;	/* allocated size (0 = volatile buffer) */
	size_t unit;	/* reallocation unit size (0 = read-only buffer) */
};

/* BUFPUTSL: optimized hoedown_buffer_puts of a string literal */
#define BUFPUTSL(output, literal) \
	hoedown_buffer_put(output, literal, sizeof(literal) - 1)

/* hoedown_buffer_grow: increasing the allocated size to the given value */
int hoedown_buffer_grow(struct hoedown_buffer *, size_t);

/* hoedown_buffer_new: allocation of a new buffer */
struct hoedown_buffer *hoedown_buffer_new(size_t) __attribute__ ((malloc));

/* bufnullterm: NUL-termination of the string array (making a C-string) */
const char *hoedown_buffer_cstr(struct hoedown_buffer *);

/* hoedown_buffer_prefix: compare the beginning of a buffer with a string */
int hoedown_buffer_prefix(const struct hoedown_buffer *buf, const char *prefix);

/* hoedown_buffer_put: appends raw data to a buffer */
void hoedown_buffer_put(struct hoedown_buffer *, const void *, size_t);

/* hoedown_buffer_puts: appends a NUL-terminated string to a buffer */
void hoedown_buffer_puts(struct hoedown_buffer *, const char *);

/* hoedown_buffer_putc: appends a single char to a buffer */
void hoedown_buffer_putc(struct hoedown_buffer *, int);

/* hoedown_buffer_release: decrease the reference count and free the buffer if needed */
void hoedown_buffer_release(struct hoedown_buffer *);

/* hoedown_buffer_reset: frees internal data of the buffer */
void hoedown_buffer_reset(struct hoedown_buffer *);

/* hoedown_buffer_slurp: removes a given number of bytes from the head of the array */
void hoedown_buffer_slurp(struct hoedown_buffer *, size_t);

/* hoedown_buffer_printf: formatted printing to a buffer */
void hoedown_buffer_printf(struct hoedown_buffer *, const char *, ...) __attribute__ ((format (printf, 2, 3)));

#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_BUFFER_H **/
