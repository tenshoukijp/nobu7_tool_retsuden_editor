#include <stdio.h>
#include <windows.h>

#pragma unmanaged
void OutputDebugStream( const char *format, ... ) {
    char szBufDebug[4096] = "";
    int length = sizeof(szBufDebug);

    va_list arg;

    va_start(arg, format);
	_vsnprintf_s(szBufDebug, length, _TRUNCATE, format, arg);
    va_end(arg);

	szBufDebug[length - 1] = '\0'; // ”Ô•º

    OutputDebugString( szBufDebug );
}