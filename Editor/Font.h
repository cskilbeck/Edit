//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Font
{
public:

	Font();
	~Font();

	void Create(WCHAR const *name, uint size);
	void Destroy();

	operator HFONT() const
	{
		return mFont;
	}

	uint Height() const
	{
		return mHeight;
	}

private:

	HFONT	mFont;
	uint	mHeight;

};
