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
	uint Height() const;

	operator HFONT()
	{
		return mFont;
	}

private:

	HFONT mFont;

};
