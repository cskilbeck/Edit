//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Line
{

//////////////////////////////////////////////////////////////////////

public:

	Line()
	{
	}

	~Line()
	{
	}

	Line(Line const &o)
		: mLine(o.mLine)
		, mLength(o.mLength)
		, mIndex(o.mIndex)
	{
	}

	char const *Initialize(char const *line, uint index)
	{
		mIndex = index;
		mLine = line;
		char const *p = line;
		while(*p != 0 && *p != '\r' && *p != '\n')
		{
			++p;
		}
		mLength = p - line;
		if(*p == '\r')
		{
			++p;
		}
		if(*p == '\n')
		{
			++p;
		}
		return p;
	}

	void Render(Texture &t, HFONT font, int lineIndex)
	{
		HDC dc = t.GetDC();
		if(dc != null)
		{
			HFONT oldFont = SelectFont(dc, font);
			SetBkMode(dc, TRANSPARENT);
			SetTextColor(dc, 0x000000);
			int tabs[1] = { 28 };
			RECT rect;
			rect.left = 0;
			rect.right = t.Width();
			rect.top = 0;
			rect.bottom = t.Height();
			FillRect(dc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			TabbedTextOutA(dc, 0, 0, GetLine(), Length(), 1, tabs, 0);
			SelectFont(dc, oldFont);
			t.ReleaseDC();
		}
	}

	char const *GetLine() const
	{
		return mLine;
	}

	size_t Length() const
	{
		return mLength;
	}

//////////////////////////////////////////////////////////////////////

private:

	char const *	mLine;
	size_t			mLength;
	uint			mIndex;

};