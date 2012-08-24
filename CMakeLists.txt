cmake_minimum_required(VERSION 2.8)

set(PROJECT_TITLE "sundown")
set(PROJECT_VERSION "git")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})

project(${PROJECT_TITLE} C)

# Compiler flags
set(CMAKE_C_FLAGS_RELEASE "-Wall -Werror -std=c99")
set(CMAKE_C_FLAGS_DEBUG "-Wall -Werror -std=c99 -ggdb -DDEBUG=1")
include_directories( ${PROJECT_SOURCE_DIR} "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/html" )

# Sources
set(LIBSUNDOWN_SOURCE
	src/autolink.c
	src/buffer.c
	src/markdown.c
	src/stack.c
)

# HTML Source
set(LIBSUNDOWN_HTML_SOURCE
	html/houdini_href_e.c
	html/houdini_html_e.c
	html/html.c
	html/html_smartypants.c
)

add_subdirectory( "examples" )

# Create libraries
add_library( ${PROJECT_TITLE}-static STATIC
	${LIBSUNDOWN_SOURCE}
	${LIBSUNDOWN_HTML_SOURCE}
)

add_library( ${PROJECT_TITLE}-shared SHARED
	${LIBSUNDOWN_SOURCE}
	${LIBSUNDOWN_HTML_SOURCE}
)

set_target_properties( ${PROJECT_TITLE}-shared PROPERTIES OUTPUT_NAME ${PROJECT_TITLE} )
