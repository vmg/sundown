#ifndef UPS_WIN32_H
#define UPS_WIN32_H

#include <string.h>

// Redefinitions
#define strncasecmp stricmplen
#define ssize_t int
#define inline __inline
#define snprintf _snprintf
#define va_copy(dst, src) ((void)((dst) = (src)))

// Cross platform definitions
#define LIBEXPORT __declspec(dllexport)
#define ATTRIBUTE_MALLOC
#define ATTRIBUTE_FORMAT

static inline int
stricmplen(const char *string1, const char *string2, int len)
{
	return stricmp(string1, string2);
}

#endif