Upskirt
=======

> The Markdown library that sucks less than your Markdown library

Why doesn't Upskirt suck?
-------------------------

Markdown is not easy to parse: it's not well standarized, and has a ton of unofficial
extensions and corner cases which makes writing a parser for it not trivial. This also
makes any Markdown parser or converter inherently sucky. Here's why Upskirt is the
least sucky of the Markdown parsers:

*	**If your Markdown library is not standards compliant, Upskirt is.**

	Upskirt passes out of the box the official Markdown v1.0.0 and v1.0.3
	test suites, and has been extensively tested with additional corner cases
	to make sure its output is as sane as possible at all times.

*	**If your Markdown library doesn't support Markdown extensions, Upskirt does.**

	Upskirt has optional support for several (unofficial) Markdown extensions,
	such as non-strict emphasis, fenced code blocks, tables, autolinks,
	strikethrough and more.

*	**If your Markdown library is ready to be used in production, you are probably in
	for a surprise.**
	
	Upskirt is the only Markdown library ready to be used in a production environment
	where users can submit arbritrary and potentially malicious contents to render.

	Upskirt has been extensively security audited, and includes protection against
	all possible DOS attacks (stack overflows, out of memory situations, malformed
	Markdown syntax...) and against client attacks through malicious embedded HTML.

	We've worked very hard to make Upskirt never crash or run out of memory
	under *any* input. Upskirt renders all the Markdown content in GitHub and so
	far hasn't crashed a single time. 

*	**If your Markdown library only does HTML, Upskirt does much more.**

	Upskirt is not stuck with XHTML output: the Markdown parser of the library
	is decoupled from the renderer, so it's trivial to extend the library with
	custom renderers. A fully functional XHTML renderer is included.

*	**If your Markdown library is slow, Upskirt is not.**

	Upskirt is written in C, with a special emphasis on performance. When wrapped
	on a dynamic language such as Python or Ruby, it has shown to be up to 40
	times faster than other native alternatives.

*	**If your Markdown library has unreasonable dependencies, Upskirt doesn't.**

	Upskirt is a zero-dependency library composed of 3 `.c` files and their headers.
	No dependencies, no bullshit. Only standard C99 that builds everywhere.

Credits
-------

Upskirt wouldn't be possible without the contributions of the following people: 

* Natacha Porté (original author)
* Vicent Martí
* Ben Noordhuis
* Bruno Michel

Help us
-------

Upskirt is all about security. If you find a (potential) security vulnerability in the library, or a way to make it crash
through malicious input, please report it to us, either directly via email or by opening an Issue on GitHub, and help
make the web safer for everybody.

Install
-------

There is nothing to install. Upskirt is composed of 3 `.c` files (`markdown.c`, `buffer.c` and `array.c`),
so just throw them in your project. Zero-dependency means zero-dependency. You might want to include `render/xhtml.c`
if you want to use the included XHTML renderer, or write your own renderer. Either way, it's all fun and joy.

If you are hardcore, you can use the included `Makefile` to build Upskirt into a dynamic library, or to build
the sample `markdown` executable, which is just a commandline Markdown to XHTML parser.

License
-------

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
