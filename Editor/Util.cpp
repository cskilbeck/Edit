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
	_vsnwprintf_s(buffer, _TRUNCATE, fmt, v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

wstring Format(wstring const &str, ...)
{
	va_list v;
	va_start(v, str);
	WCHAR buffer[8192];
	_vsnwprintf_s(buffer, _TRUNCATE, str.c_str(), v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

string Format(char const *fmt, ...)
{
	va_list v;
	va_start(v, fmt);
	char buffer[8192];
	_vsnprintf_s(buffer, _TRUNCATE, fmt, v);
	return buffer;
}

//////////////////////////////////////////////////////////////////////

string Format(string const &str, ...)
{
	va_list v;
	va_start(v, str);
	char buffer[8192];
	_vsnprintf_s(buffer, _TRUNCATE, str.c_str(), v);
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

byte *LoadFile(TCHAR const *filename, size_t *size)
{
	byte *buf = null;
	FILE *f = null;

	if(_wfopen_s(&f, filename, TEXT("rb")) == 0)
	{
		fseek(f, 0, SEEK_END);
		uint32 len = ftell(f);
		fseek(f, 0, SEEK_SET);

		buf = new byte[len + 2];

		if(buf != null)
		{
			size_t s = fread_s(buf, len, 1, len, f);

			if(s != len)
			{
				DeleteArray(buf);
			}
			else
			{
				*((WCHAR *)(((char *)buf) + len)) = L'\0';
				if(size != null)
				{
					*size = len;
				}
			}
		}

		fclose(f);
	}
	else
	{
		MessageBox(null, Format(TEXT("File not found: %s"), filename).c_str(), TEXT("LoadFile"), MB_ICONERROR);
	}
	return buf;
}

//////////////////////////////////////////////////////////////////////

