/* stack.h - simple stacking */

#ifndef HOEDOWN_STACK_H
#define HOEDOWN_STACK_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hoedown_stack {
	void **item;
	size_t size;
	size_t asize;
};

int hoedown_stack_new(struct hoedown_stack *, size_t);
void hoedown_stack_free(struct hoedown_stack *);
int hoedown_stack_grow(struct hoedown_stack *, size_t);
int hoedown_stack_push(struct hoedown_stack *, void *);
void *hoedown_stack_pop(struct hoedown_stack *);
void *hoedown_stack_top(struct hoedown_stack *);

#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_STACK_H **/
