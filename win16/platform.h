/* Win16 platform boundary for the Microsoft WinFile source port.
 * Copyright (c) 2026 GeosOne. Licensed under the MIT License. */
#ifndef WINFILE_WIN16_PLATFORM_H
#define WINFILE_WIN16_PLATFORM_H
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>

#if !defined(_M_I86) || defined(__WINDOWS_386__)
#error This adapter requires the Win16 SDK and a 16-bit memory model
#endif

#ifdef UNICODE
#error Win16 uses Windows ANSI strings, not the Win32 Unicode API
#endif
#define TEXT(value) value
typedef char TCHAR;
typedef char FAR *LPTSTR;
typedef const char FAR *LPCTSTR;
typedef int INT;
typedef unsigned short USHORT;
typedef union {
    struct { DWORD LowPart; LONG HighPart; };
    signed __int64 QuadPart;
} LARGE_INTEGER;
typedef struct { DWORD dwLowDateTime, dwHighDateTime; } FILETIME;
typedef char W16RequiresLargeModel[(sizeof(void *) == 4 && sizeof(int) == 2) ? 1 : -1];
typedef char W16Requires64BitSize[(sizeof(LARGE_INTEGER) == 8) ? 1 : -1];

#define NO_ERROR 0
#define ERROR_INVALID_PARAMETER 87
#define CCHMAXPATHCOMP 256
#define CHAR_NULL '\0'
#define CHAR_BACKSLASH '\\'
#define CHAR_SLASH '/'
#define CHAR_COLON ':'
#define CHAR_DQUOTE '"'
#define CHAR_PIPE '|'
#define CHAR_GREATER '>'
#define CHAR_LESS '<'
#define CHAR_STAR '*'
#define CHAR_QUESTION '?'
#define CHAR_DOT '.'
#define CHAR_SPACE ' '
#define CHAR_ZERO '0'
#define SZ_PERCENTD "%d"
#define COUNTOF(value) (sizeof(value) / sizeof((value)[0]))
#define CopyMemory(target, source, size) _fmemcpy(target, source, size)

extern HINSTANCE hAppInstance;
extern TCHAR szDecimal[];
LPVOID W16Alloc(UINT size);
void W16Free(LPVOID pointer);
UINT W16Size(LPVOID pointer);
DWORD I_LFNCanon(WORD type, LPTSTR source, LPTSTR target);
WORD I_LFNEditName(LPTSTR source, LPTSTR mask, LPTSTR result, INT size);
LPTSTR ShortSizeFormatInternal(LPTSTR buffer, LARGE_INTEGER size);
#endif
