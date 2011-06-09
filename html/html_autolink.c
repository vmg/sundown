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

#include "autolink.h"
#include "buffer.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static void
autolink_escape_cb(struct buf *ob, const struct buf *text, void *unused)
{
	size_t  i = 0, org;

	while (i < text->size) {
		org = i;

		while (i < text->size &&
			text->data[i] != '<' &&
			text->data[i] != '>' &&
			text->data[i] != '&' &&
			text->data[i] != '"')
			i++;

		if (i > org)
			bufput(ob, text->data + org, i - org);

		if (i >= text->size)
			break;

		switch (text->data[i]) {
			case '<': BUFPUTSL(ob, "&lt;"); break;
			case '>': BUFPUTSL(ob, "&gt;"); break;
			case '&': BUFPUTSL(ob, "&amp;"); break;
			case '"': BUFPUTSL(ob, "&quot;"); break;
			default: bufputc(ob, text->data[i]); break;
		}

		i++;
	}
}

static inline int
is_closing_a(const char *tag, size_t size)
{
	size_t i;

	if (tag[0] != '<' || size < STRLEN("</a>") || tag[1] != '/')
		return 0;

	i = 2;

	while (i < size && isspace(tag[i]))
		i++;

	if (i == size || tag[i] != 'a')
		return 0;

	i++;

	while (i < size && isspace(tag[i]))
		i++;

	if (i == size || tag[i] != '>')
		return 0;

	return i;
}

static size_t
autolink__skip_tag(struct buf *ob, char *text, size_t size)
{
	size_t i = 0;

	while (i < size && text[i] != '>')
		i++;

	if (size > 3 && text[1] == 'a' && isspace(text[2])) {
		while (i < size) {
			size_t tag_len = is_closing_a(text + i, size - i);
			if (tag_len) {
				i += tag_len;
				break;
			}
			i++;
		}
	}

	bufput(ob, text, i + 1);
	return i + 1;
}

typedef size_t (*autolink_parse_cb)(size_t *rewind, struct buf *, char *, size_t, size_t);

typedef enum {
	AUTOLINK_ACTION_NONE = 0,
	AUTOLINK_ACTION_WWW,
	AUTOLINK_ACTION_EMAIL,
	AUTOLINK_ACTION_URL,
	AUTOLINK_ACTION_SKIP_TAG
} autolink_action;

static autolink_parse_cb g_callbacks[] = {
	NULL,
	ups_autolink__www,	/* 1 */
	ups_autolink__email,/* 2 */
	ups_autolink__url,	/* 3 */
};

static const char *g_hrefs[] = {
	NULL,
	" href=\"http://",
	" href=\"mailto:",
	" href=\"",
};

void
upshtml_autolink(
	struct buf *ob,
	struct buf *text,
	unsigned int flags,
	const char *link_attr,
	void (*link_text_cb)(struct buf *ob, const struct buf *link, void *payload),
	void *payload)
{
	size_t i, end;
	struct buf *link = bufnew(16);
	char active_chars[256];

	if (!text || text->size == 0)
		return;

	memset(active_chars, 0x0, sizeof(active_chars));

	active_chars['<'] = AUTOLINK_ACTION_SKIP_TAG;

	if (flags & AUTOLINK_EMAILS)
		active_chars['@'] = AUTOLINK_ACTION_EMAIL;

	if (flags & AUTOLINK_URLS) {
		active_chars['w'] = AUTOLINK_ACTION_WWW;
		active_chars['W'] = AUTOLINK_ACTION_WWW;
		active_chars[':'] = AUTOLINK_ACTION_URL;
	}

	if (link_text_cb == NULL)
		link_text_cb = &autolink_escape_cb;

	bufgrow(ob, text->size);

	i = end = 0;

	while (i < text->size) {
		size_t rewind;
		char action;

		while (end < text->size && (action = active_chars[(int)text->data[end]]) == 0)
			end++;

		bufput(ob, text->data + i, end - i);

		if (end >= text->size)
			break;

		i = end;
		end = 0;
		link->size = 0;

		if (action == AUTOLINK_ACTION_SKIP_TAG) {
			end = autolink__skip_tag(ob, text->data + i, text->size - i);
		} else {
			end = g_callbacks[(int)action](&rewind, link, text->data + i, i, text->size - i);

			/* print the link */
			if (end > 0) {
				ob->size -= rewind;
				BUFPUTSL(ob, "<a");
				if (link_attr) bufputs(ob, link_attr);
				bufputs(ob, g_hrefs[(int)action]);
				bufput(ob, link->data, link->size);
				BUFPUTSL(ob, "\">");
				link_text_cb(ob, link, payload);
				BUFPUTSL(ob, "</a>");
			}
		}

		if (!end)
			end = i + 1;
		else { 
			i += end;
			end = i;
		} 
	}

	bufrelease(link);
}


