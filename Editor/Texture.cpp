//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3D.h"

//////////////////////////////////////////////////////////////////////

Texture::Texture()
	: mTexture(null)
	, mShaderResourceView(null)
	, mSurface(null)
{
}

//////////////////////////////////////////////////////////////////////

Texture::~Texture()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////

void Texture::Destroy()
{
	Release(mSurface);
	Release(mShaderResourceView);
	Release(mTexture);
}

//////////////////////////////////////////////////////////////////////

uint Texture::Width() const
{
	return mWidth;
}

//////////////////////////////////////////////////////////////////////

uint Texture::Height() const
{
	return mHeight;
}

//////////////////////////////////////////////////////////////////////

void Texture::Create(int width, int height)
{
	Destroy();

	mWidth = width;
	mHeight = height;
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
	d3d.mDevice->CreateTexture2D(&desc, null, &mTexture);
	d3d.mDevice->CreateShaderResourceView(mTexture, null, &mShaderResourceView);
	mTexture->QueryInterface(__uuidof(IDXGISurface1), (void **)(&mSurface));
}

//////////////////////////////////////////////////////////////////////

HDC Texture::GetDC()
{
	HDC dc;
	mSurface->GetDC(FALSE, &dc);
	return dc;
}

//////////////////////////////////////////////////////////////////////

void Texture::ReleaseDC()
{
	mSurface->ReleaseDC(null);
}

//////////////////////////////////////////////////////////////////////

void Texture::Activate()
{
	d3d.mDeviceContext->PSSetShaderResources(0, 1, &mShaderResourceView);
}

//////////////////////////////////////////////////////////////////////

