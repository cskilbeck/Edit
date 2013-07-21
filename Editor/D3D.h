//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class D3D
{

public:

	D3D();
	~D3D();

	void Open(HWND hwnd);
	void Resize(int width, int height);
	void Present();
	void Clear(uint32 color);
	void Close();

	uint Width() const;
	uint Height() const;

	void DrawATriangle(float width, float height);

//////////////////////////////////////////////////////////////////////

private:

	void GetBackBuffer();
	void InitShaders();

	static const DXGI_FORMAT	kBackBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

	uint						mWidth;
	uint						mHeight;

	ID3D11Device *				mDevice;
	IDXGISwapChain *			mSwapChain;
	ID3D11DeviceContext *		mDeviceContext;
	ID3D11RenderTargetView *	mBackBuffer;
	ID3D11PixelShader *			mPixelShader;
	ID3D11VertexShader *		mVertexShader;
	ID3D11InputLayout *			mVertexLayout;
	ID3D11Buffer *				mMatrixBuffer;
	ID3D11SamplerState *		mSamplerState;

	ID3D11Buffer *				mVertexBuffer;
	ID3D11RasterizerState *		mRasterizerState;

	friend class Texture;
};

//////////////////////////////////////////////////////////////////////

extern D3D d3d;