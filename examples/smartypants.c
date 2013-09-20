#include "html.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_UNIT 1024
#define OUTPUT_UNIT 64

int
main(int argc, char **argv)
{
	struct hoedown_buffer *ib, *ob;
	size_t ret;
	FILE *in = stdin;

	/* opening the file if given from the command line */
	if (argc > 1) {
		in = fopen(argv[1], "r");
		if (!in) {
			fprintf(stderr, "Unable to open input file \"%s\": %s\n", argv[0], strerror(errno));
			return 1;
		}
	}

	/* reading everything */
	ib = hoedown_buffer_new(READ_UNIT);
	hoedown_buffer_grow(ib, READ_UNIT);
	while ((ret = fread(ib->data + ib->size, 1, ib->asize - ib->size, in)) > 0) {
		ib->size += ret;
		hoedown_buffer_grow(ib, ib->size + READ_UNIT);
	}

	if (in != stdin)
		fclose(in);

	/* performing SmartyPants parsing */
	ob = hoedown_buffer_new(OUTPUT_UNIT);

	hoedown_html_smartypants(ob, ib->data, ib->size);

	/* writing the result to stdout */
	(void)fwrite(ob->data, 1, ob->size, stdout);

	/* cleanup */
	hoedown_buffer_release(ib);
	hoedown_buffer_release(ob);

	return 0;
}
