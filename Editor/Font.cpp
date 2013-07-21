//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Font::Font()
	: mFont(null)
{
}

//////////////////////////////////////////////////////////////////////

Font::~Font()
{
	if(mFont != null)
	{
		DeleteObject(mFont);
		mFont = null;
	}
}

//////////////////////////////////////////////////////////////////////

uint Font::Height() const
{
	uint height = 0;
	if(mFont != null)
	{
		HDC screenDC = GetDC(null);
		HFONT oldFont = SelectFont(screenDC, mFont);
		TEXTMETRIC t;
		GetTextMetrics(screenDC, &t);
		SelectFont(screenDC, oldFont);
		height = t.tmHeight + t.tmExternalLeading;
	}
	return height;
}

//////////////////////////////////////////////////////////////////////

void Font::Create(WCHAR const *name, uint size)
{
	LOGFONT logFont = { 0 };
	HDC screenDC = GetDC(null);
	uint lpy = GetDeviceCaps(screenDC, LOGPIXELSY);
	ReleaseDC(null, screenDC);
	logFont.lfHeight = -MulDiv(size, lpy, 72);
	logFont.lfWeight = FW_NORMAL;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = CLEARTYPE_QUALITY;
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(logFont.lfFaceName, name);
	mFont = CreateFontIndirect(&logFont);
}

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////



