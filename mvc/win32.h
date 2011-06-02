#ifndef UPS_WIN32_H
#define UPS_WIN32_H

<<<<<<< HEAD
#include <string.h>

// Redefinitions
#define strncasecmp stricmplen
=======
// Redefinitions
#define strncasecmp _strnicmp
>>>>>>> ms-visual-studio
#define ssize_t int
#define inline __inline
#define snprintf _snprintf
#define va_copy(dst, src) ((void)((dst) = (src)))
<<<<<<< HEAD

// Cross platform definitions
#define LIBEXPORT __declspec(dllexport)
#define ATTRIBUTE_MALLOC
#define ATTRIBUTE_FORMAT

static inline int
stricmplen(const char *string1, const char *string2, int len)
{
	return stricmp(string1, string2);
}
=======
#define __attribute__(x)

#pragma warning( disable : 4996 )

#if _DLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
>>>>>>> ms-visual-studio

#endif