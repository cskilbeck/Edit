//////////////////////////////////////////////////////////////////////
// Create a texture
// Use GDI to render some text into it
// Draw a quad with it

#include "stdafx.h"
#include "resource.h"
#include "D3D.h"

//////////////////////////////////////////////////////////////////////

D3D::D3D()
	: mDevice(null)
	, mDeviceContext(null)
	, mSwapChain(null)
	, mBackBuffer(null)
	, mPixelShader(null)
	, mVertexShader(null)
	, mVertexLayout(null)
	, mMatrixBuffer(null)
	, mVertexBuffer(null)
	, mRasterizerState(null)
	, mSamplerState(null)
{
}

//////////////////////////////////////////////////////////////////////

void D3D::Close()
{
	Release(mSamplerState);
	Release(mRasterizerState);
	Release(mVertexBuffer);
	Release(mMatrixBuffer);
	Release(mVertexLayout);
	Release(mVertexShader);
	Release(mPixelShader);
	Release(mBackBuffer);
	Release(mSwapChain);
	Release(mDeviceContext);
	Release(mDevice);
}

//////////////////////////////////////////////////////////////////////

D3D::~D3D()
{
	Close();
}

//////////////////////////////////////////////////////////////////////

void D3D::Open(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = kBackBufferFormat;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	RECT r;
	GetClientRect(hwnd, &r);
	mWidth = r.right;
	mHeight = r.bottom;

    D3D11CreateDeviceAndSwapChain(null,
									D3D_DRIVER_TYPE_HARDWARE,
									null,
									DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE | D3D11_CREATE_DEVICE_DEBUG,
									null,
									0,
									D3D11_SDK_VERSION,
									&scd,
									&mSwapChain,
									&mDevice,
									null,
									&mDeviceContext);

	GetBackBuffer();
	InitShaders();
}

//////////////////////////////////////////////////////////////////////

void D3D::DrawATriangle(float width, float height)
{
	Vertex v[6];

	v[0].mX = 0;
	v[0].mY = 0;
	v[0].mU = 0.0f;
	v[0].mV = 0.0f;
	v[0].mColor = 0xffffffff;	// ABGR
	v[1].mX = width;
	v[1].mY = 0;
	v[1].mU = 1.0f;
	v[1].mV = 0.0f;
	v[1].mColor = 0xffffffff;
	v[2].mX = 0;
	v[2].mY = height;
	v[2].mU = 0.0f;
	v[2].mV = 1.0f;
	v[2].mColor = 0xffffffff;
	v[3].mX = 0;
	v[3].mY = height;
	v[3].mU = 0.0f;
	v[3].mV = 1.0f;
	v[3].mColor = 0xffffffff;
	v[4].mX = width;
	v[4].mY = 0;
	v[4].mU = 1.0f;
	v[4].mV = 0.0f;
	v[4].mColor = 0xffffffff;
	v[5].mX = width;
	v[5].mY = height;
	v[5].mU = 1.0f;
	v[5].mV = 1.0f;
	v[5].mColor = 0xffffffff;

	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, v, sizeof(v));
	mDeviceContext->Unmap(mVertexBuffer, 0);

	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	mDeviceContext->RSSetState(mRasterizerState);

	mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);
	mDeviceContext->VSSetShader(mVertexShader, null, 0);

	mDeviceContext->PSSetShader(mPixelShader, null, 0);
	mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);

	mDeviceContext->Draw(6, 0);
}

//////////////////////////////////////////////////////////////////////

void D3D::Resize(int width, int height)
{
	if(width > 0 && height > 0)
	{
		mWidth = width;
		mHeight = height;

		D3D11_VIEWPORT viewport = { 0 };
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		mDeviceContext->RSSetViewports(1, &viewport);
		Release(mBackBuffer);

		mSwapChain->ResizeBuffers(1, width, height, kBackBufferFormat, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE);

		GetBackBuffer();

		float halfWidth = 2.0f / width;
		float halfHeight = -2.0f / height;

		XMMATRIX perPixelMatrix = XMMatrixTranspose(XMMATRIX(halfWidth,	0.0f,		0.0f,	0.0f,
															 0.0f,		halfHeight,	0.0f,	0.0f,
															 0.0f,		0.0f,		1.0f,	0.0f,
															 -1.0f,		1.0f,		0.0f,	1.0f));

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		mDeviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &perPixelMatrix, sizeof(XMMATRIX));
		mDeviceContext->Unmap(mMatrixBuffer, 0);
	}
}

//////////////////////////////////////////////////////////////////////

uint D3D::Width() const
{
	return mWidth;
}

//////////////////////////////////////////////////////////////////////

uint D3D::Height() const
{
	return mHeight;
}

//////////////////////////////////////////////////////////////////////

void D3D::Clear(uint32 color)
{
	float abgr[4];
	abgr[3] = ((color >> 24) & 0xff) / 255.0f;
	abgr[2] = ((color >> 16) & 0xff) / 255.0f;
	abgr[1] = ((color >>  8) & 0xff) / 255.0f;
	abgr[0] = ((color >>  0) & 0xff) / 255.0f;
	mDeviceContext->ClearRenderTargetView(mBackBuffer, abgr);
}

//////////////////////////////////////////////////////////////////////

void D3D::Present()
{
	mSwapChain->Present(1, 0);
}

//////////////////////////////////////////////////////////////////////

void D3D::GetBackBuffer()
{
    ID3D11Texture2D *pBackBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    mDevice->CreateRenderTargetView(pBackBuffer, null, &mBackBuffer);
	Release(pBackBuffer);
    mDeviceContext->OMSetRenderTargets(1, &mBackBuffer, null);
}

//////////////////////////////////////////////////////////////////////

void D3D::InitShaders()
{
	ID3DBlob *VS_Buffer = null;
	ID3DBlob *PS_Buffer = null;
	HRESULT hr;

	hr = D3DReadFileToBlob(TEXT("PixelShader.cso"), &PS_Buffer);
	if(SUCCEEDED(hr))
	{
		mDevice->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), null, &mPixelShader);
		mDeviceContext->PSSetShader(mPixelShader, 0, 0);
	}

	hr = D3DReadFileToBlob(TEXT("VertexShader.cso"), &VS_Buffer);
	if(SUCCEEDED(hr))
	{
		mDevice->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), null, &mVertexShader);

		D3D11_INPUT_ELEMENT_DESC IEDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 8,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 16,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		mDevice->CreateInputLayout(IEDesc, ARRAYSIZE(IEDesc), VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &mVertexLayout);
		mDeviceContext->IASetInputLayout(mVertexLayout);
	}

	D3D11_BUFFER_DESC matrixBufferDesc = { 0 };
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(XMMATRIX);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mDevice->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 400;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);
	mDevice->CreateBuffer(&vertexBufferDesc, null, &mVertexBuffer);

	CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	mDevice->CreateRasterizerState(&rasterizerDesc, &mRasterizerState);

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.BorderColor[ 0 ] = 1.0f;
	sampDesc.BorderColor[ 1 ] = 1.0f;
	sampDesc.BorderColor[ 2 ] = 1.0f;
	sampDesc.BorderColor[ 3 ] = 1.0f;
	sampDesc.MinLOD = -3.402823466e+38F;
	sampDesc.MaxLOD = 3.402823466e+38F;
	mDevice->CreateSamplerState(&sampDesc, &mSamplerState);

	Release(PS_Buffer);
	Release(VS_Buffer);
}

