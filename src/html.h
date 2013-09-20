/* html.h - HTML renderer */

#ifndef HOEDOWN_HTML_H
#define HOEDOWN_HTML_H

#include "markdown.h"
#include "buffer.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hoedown_html_renderopt {
	struct {
		int header_count;
		int current_level;
		int level_offset;
		int nesting_level;
	} toc_data;

	unsigned int flags;

	/* extra callbacks */
	void (*link_attributes)(struct hoedown_buffer *ob, const struct hoedown_buffer *url, void *self);
};

typedef enum {
	HOEDOWN_HTML_SKIP_HTML = (1 << 0),
	HOEDOWN_HTML_SKIP_STYLE = (1 << 1),
	HOEDOWN_HTML_SKIP_IMAGES = (1 << 2),
	HOEDOWN_HTML_SKIP_LINKS = (1 << 3),
	HOEDOWN_HTML_EXPAND_TABS = (1 << 4),
	HOEDOWN_HTML_SAFELINK = (1 << 5),
	HOEDOWN_HTML_TOC = (1 << 6),
	HOEDOWN_HTML_HARD_WRAP = (1 << 7),
	HOEDOWN_HTML_USE_XHTML = (1 << 8),
	HOEDOWN_HTML_ESCAPE = (1 << 9),
	HOEDOWN_HTML_PRETTIFY = (1 << 10),
} hoedown_html_render_mode;

typedef enum {
	HOEDOWN_HTML_TAG_NONE = 0,
	HOEDOWN_HTML_TAG_OPEN,
	HOEDOWN_HTML_TAG_CLOSE,
} hoedown_html_tag;

int
hoedown_html_is_tag(const uint8_t *tag_data, size_t tag_size, const char *tagname);

extern void
hoedown_html_renderer(struct hoedown_callbacks *callbacks, struct hoedown_html_renderopt *options_ptr, unsigned int render_flags);

extern void
hoedown_html_toc_renderer(struct hoedown_callbacks *callbacks, struct hoedown_html_renderopt *options_ptr, int nesting_level);

extern void
hoedown_html_smartypants(struct hoedown_buffer *ob, const uint8_t *text, size_t size);

#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_HTML_H **/
