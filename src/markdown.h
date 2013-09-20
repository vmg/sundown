/* markdown.h - generic markdown parser */

#ifndef HOEDOWN_MARKDOWN_H
#define HOEDOWN_MARKDOWN_H

#include "buffer.h"
#include "autolink.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HOEDOWN_VERSION "0.0.1"
#define HOEDOWN_VER_MAJOR 0
#define HOEDOWN_VER_MINOR 0
#define HOEDOWN_VER_REVISION 1

/********************
 * TYPE DEFINITIONS *
 ********************/

/* hoedown_autolink - type of autolink */
enum hoedown_autolink {
	HOEDOWN_AUTOLINK_NONE,	/* used internally when it is not an autolink*/
	HOEDOWN_AUTOLINK_NORMAL,		/* normal http/http/ftp/mailto/etc link */
	HOEDOWN_AUTOLINK_EMAIL,			/* e-mail link without explit mailto: */
};

enum hoedown_tableflags {
	HOEDOWN_TABLE_ALIGN_L = 1,
	HOEDOWN_TABLE_ALIGN_R = 2,
	HOEDOWN_TABLE_ALIGN_CENTER = 3,
	HOEDOWN_TABLE_ALIGNMASK = 3,
	HOEDOWN_TABLE_HEADER = 4
};

enum hoedown_extensions {
	HOEDOWN_EXT_NO_INTRA_EMPHASIS = (1 << 0),
	HOEDOWN_EXT_TABLES = (1 << 1),
	HOEDOWN_EXT_FENCED_CODE = (1 << 2),
	HOEDOWN_EXT_AUTOLINK = (1 << 3),
	HOEDOWN_EXT_STRIKETHROUGH = (1 << 4),
	HOEDOWN_EXT_UNDERLINE = (1 << 5),
	HOEDOWN_EXT_SPACE_HEADERS = (1 << 6),
	HOEDOWN_EXT_SUPERSCRIPT = (1 << 7),
	HOEDOWN_EXT_LAX_SPACING = (1 << 8),
	HOEDOWN_EXT_DISABLE_INDENTED_CODE = (1 << 9),
	HOEDOWN_EXT_HIGHLIGHT = (1 << 10),
	HOEDOWN_EXT_FOOTNOTES = (1 << 11),
	HOEDOWN_EXT_QUOTE = (1 << 12)
};

/* hoedown_callbacks - functions for rendering parsed data */
struct hoedown_callbacks {
	/* block level callbacks - NULL skips the block */
	void (*blockcode)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, const struct hoedown_buffer *lang, void *opaque);
	void (*blockquote)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	void (*blockhtml)(struct hoedown_buffer *ob,const  struct hoedown_buffer *text, void *opaque);
	void (*header)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, int level, void *opaque);
	void (*hrule)(struct hoedown_buffer *ob, void *opaque);
	void (*list)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, int flags, void *opaque);
	void (*listitem)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, int flags, void *opaque);
	void (*paragraph)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	void (*table)(struct hoedown_buffer *ob, const struct hoedown_buffer *header, const struct hoedown_buffer *body, void *opaque);
	void (*table_row)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	void (*table_cell)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, int flags, void *opaque);
	void (*footnotes)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	void (*footnote_def)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, unsigned int num, void *opaque);

	/* span level callbacks - NULL or return 0 prints the span verbatim */
	int (*autolink)(struct hoedown_buffer *ob, const struct hoedown_buffer *link, enum hoedown_autolink type, void *opaque);
	int (*codespan)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*double_emphasis)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*emphasis)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*underline)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*highlight)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*quote)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*image)(struct hoedown_buffer *ob, const struct hoedown_buffer *link, const struct hoedown_buffer *title, const struct hoedown_buffer *alt, void *opaque);
	int (*linebreak)(struct hoedown_buffer *ob, void *opaque);
	int (*link)(struct hoedown_buffer *ob, const struct hoedown_buffer *link, const struct hoedown_buffer *title, const struct hoedown_buffer *content, void *opaque);
	int (*raw_html_tag)(struct hoedown_buffer *ob, const struct hoedown_buffer *tag, void *opaque);
	int (*triple_emphasis)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*strikethrough)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*superscript)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);
	int (*footnote_ref)(struct hoedown_buffer *ob, unsigned int num, void *opaque);

	/* low level callbacks - NULL copies input directly into the output */
	void (*entity)(struct hoedown_buffer *ob, const struct hoedown_buffer *entity, void *opaque);
	void (*normal_text)(struct hoedown_buffer *ob, const struct hoedown_buffer *text, void *opaque);

	/* header and footer */
	void (*doc_header)(struct hoedown_buffer *ob, void *opaque);
	void (*doc_footer)(struct hoedown_buffer *ob, void *opaque);
};

struct hoedown_markdown;

/*********
 * FLAGS *
 *********/

/* list/listitem flags */
#define HOEDOWN_LIST_ORDERED	1
#define HOEDOWN_LI_BLOCK	2  /* <li> containing block data */

/**********************
 * EXPORTED FUNCTIONS *
 **********************/

extern struct hoedown_markdown *
hoedown_markdown_new(
	unsigned int extensions,
	size_t max_nesting,
	const struct hoedown_callbacks *callbacks,
	void *opaque);

extern void
hoedown_markdown_render(struct hoedown_buffer *ob, const uint8_t *document, size_t doc_size, struct hoedown_markdown *md);

extern void
hoedown_markdown_free(struct hoedown_markdown *md);

extern void
hoedown_version(int *major, int *minor, int *revision);

#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_MARKDOWN_H **/
