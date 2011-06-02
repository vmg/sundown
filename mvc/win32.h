#ifndef UPS_WIN32_H
#define UPS_WIN32_H

// Redefinitions
#define strncasecmp _strnicmp
#define ssize_t int
#define inline __inline
#define snprintf _snprintf
#define va_copy(dst, src) ((void)((dst) = (src)))
#define __attribute__(x)

#pragma warning( disable : 4996 )

#if _DLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#endif