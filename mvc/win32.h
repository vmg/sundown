#ifndef UPS_WIN32_H
#define UPS_WIN32_H

#include <string.h>

// Redefinitions
#define strncasecmp _strnicmp
#define ssize_t int
#define inline __inline
#define snprintf _snprintf
#define va_copy(dst, src) ((void)((dst) = (src)))
#define __attribute__(x)

#pragma warning( disable : 4996 )

#if defined(_DLL)
#define WIN32EXPORT __declspec(dllexport)
#else
#define WIN32EXPORT
#endif

#endif