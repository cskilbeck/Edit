//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

wstring GetCurrentFolder()
{
	WCHAR buffer[16384];
	buffer[0] = 0;
	GetCurrentDirectory(ARRAYSIZE(buffer), buffer);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

wstring GetExecutableFilename()
{
	WCHAR filename[32768];
	filename[0] = 0;
	filename[ARRAYSIZE(filename) - 1] = 0;
	GetModuleFileName(GetModuleHandle(null), filename, ARRAYSIZE(filename) - 1);
	return filename;
}

//////////////////////////////////////////////////////////////////////

wstring Format(WCHAR const *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer) - 1, fmt, v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

wstring Format(wstring const &str, ...)
{
	va_list v;
	va_start(v, str);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer) - 1, str.c_str(), v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

void TRACE(WCHAR const *format, ...)
{
	va_list v;
	va_start(v, format);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer) - 1, format, v);
	OutputDebugString(buffer);
}

//////////////////////////////////////////////////////////////////////

void TRACE(wstring const &format, ...)
{
	va_list v;
	va_start(v, format);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer) - 1, format.c_str(), v);
	OutputDebugString(buffer);
}

//////////////////////////////////////////////////////////////////////

