Hoedown
=======

[![Build Status](https://travis-ci.org/devinus/hoedown.png?branch=master)](https://travis-ci.org/devinus/hoedown)

`Hoedown` is a revived fork of [Sundown](https://github.com/vmg/sundown),
the Markdown parser based on the original code of the
[Upskirt library](http://fossil.instinctive.eu/libupskirt/index)
by Natacha Porté.

Features
--------

*	**Fully standards compliant**

	`Hoedown` passes out of the box the official Markdown v1.0.0 and v1.0.3
	test suites, and has been extensively tested with additional corner cases
	to make sure its output is as sane as possible at all times.

*	**Massive extension support**

	`Hoedown` has optional support for several (unofficial) Markdown extensions,
	such as non-strict emphasis, fenced code blocks, tables, autolinks,
	strikethrough and more.

*	**UTF-8 aware**

	`Hoedown` is fully UTF-8 aware, both when parsing the source document and when
	generating the resulting (X)HTML code.

*	**Tested & Ready to be used on production**

	`Hoedown` has been extensively security audited, and includes protection against
	all possible DOS attacks (stack overflows, out of memory situations, malformed
	Markdown syntax...) and against client attacks through malicious embedded HTML.

	We've worked very hard to make `Hoedown` never crash or run out of memory
	under *any* input. `Hoedown` renders all the Markdown content in GitHub and so
	far hasn't crashed a single time.

*	**Customizable renderers**

	`Hoedown` is not stuck with XHTML output: the Markdown parser of the library
	is decoupled from the renderer, so it's trivial to extend the library with
	custom renderers. A fully functional (X)HTML renderer is included.

*	**Optimized for speed**

	`Hoedown` is written in C, with a special emphasis on performance. When wrapped
	on a dynamic language such as Python or Ruby, it has shown to be up to 40
	times faster than other native alternatives.

*	**Zero-dependency**

	`Hoedown` is a zero-dependency library composed of 4 `.c` files and their headers.
	No dependencies, no bullshit. Only standard C99 that builds everywhere.

Bindings
--------

You can see a community-maintained list of `Hoedown` bindings at
[the wiki](https://github.com/devinus/hoedown/wiki/Bindings).

Help us
-------

`Hoedown` is all about security. If you find a (potential) security vulnerability in the
library, or a way to make it crash through malicious input, please report it to us,
by leaving an Issue at <https://github.com/devinus/hoedown/issues/new>.

Unicode character handling
--------------------------

Given that the Markdown spec makes no provision for Unicode character handling, `Hoedown`
takes a conservative approach towards deciding which extended characters trigger Markdown
features:

*	Punctuation characters outside of the U+007F codepoint are not handled as punctuation.
	They are considered as normal, in-word characters for word-boundary checks.

*	Whitespace characters outside of the U+007F codepoint are not considered as
	whitespace. They are considered as normal, in-word characters for word-boundary checks.

Install
-------

There is nothing to install. `Hoedown` is composed of 4 `.c` files (`markdown.c`,
`buffer.c`, `stack.c`, and `autolink.c`) and their headers, so just throw them in your
project. Zero-dependency means zero-dependency. You might want to include `html.c`,
`html_smartypants.c` and `escape.c` if you want to use the included XHTML renderer, or
write your own renderer. Either way, it's all fun and joy.

If you are hardcore, you can use the included `Makefile` to build `Hoedown` into a dynamic
library, or to build the sample `hoedown` executable, which is just a commandline
Markdown to XHTML parser. (If gcc gives you grief about `-fPIC`, e.g. with MinGW, try
`make MFLAGS=` instead of just `make`.)
