//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

wstring GetCurrentFolder();
wstring	GetExecutableFilename();
wstring	Format(WCHAR const *fmt, ...);
wstring	Format(wstring const &fmt, ...);
string Format(char const *fmt, ...);
string Format(string const &fmt, ...);
void TRACE(WCHAR const *format, ...);
void TRACE(wstring const &format, ...);
byte *LoadFile(TCHAR const *filename, size_t *size);

//////////////////////////////////////////////////////////////////////

template <typename T> void Release(T * &p)
{
	if(p != null)
	{
		p->Release();
		p = null;
	}
}

//////////////////////////////////////////////////////////////////////

template <typename T> void Delete(T * &p)
{
	if(p != null)
	{
		delete p;
		p = null;
	}
}

//////////////////////////////////////////////////////////////////////

template <typename T> void DeleteArray(T * &p)
{
	if(p != null)
	{
		delete[] p;
		p = null;
	}
}

//////////////////////////////////////////////////////////////////////
