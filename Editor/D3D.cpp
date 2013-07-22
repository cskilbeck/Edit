//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "D3D.h"

//////////////////////////////////////////////////////////////////////

D3D::D3D()
	: mDevice(null)
	, mDebug(null)
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
	TRACE(TEXT("D3D()\n"));
}

//////////////////////////////////////////////////////////////////////

void D3D::Close()
{
	TRACE(TEXT("D3D::Close()\n"));

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

	if(mDebug != null)
	{
		//mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	Release(mDebug);
}

//////////////////////////////////////////////////////////////////////

D3D::~D3D()
{
	TRACE(TEXT("~D3D()\n"));
	Close();
}

//////////////////////////////////////////////////////////////////////

void D3D::Open(HWND hwnd)
{
	TRACE(TEXT("D3D::Open()\n"));

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

    D3D11CreateDeviceAndSwapChain(null, D3D_DRIVER_TYPE_HARDWARE, null, D3D11_CREATE_DEVICE_DEBUG,
								null, 0, D3D11_SDK_VERSION, &scd, &mSwapChain, &mDevice, null, &mDeviceContext);

	HRESULT hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&mDebug));

	GetBackBuffer();
	InitShaders();
}

//////////////////////////////////////////////////////////////////////

void D3D::SetupRenderState()
{
	mDeviceContext->RSSetState(mRasterizerState);
	mDeviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);
	mDeviceContext->VSSetShader(mVertexShader, null, 0);
	mDeviceContext->PSSetShader(mPixelShader, null, 0);
	mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);
}

//////////////////////////////////////////////////////////////////////

void D3D::Resize(int width, int height)
{
	TRACE(TEXT("D3D::Resize(%d,%d)\n"), width, height);
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

		mSwapChain->ResizeBuffers(1, width, height, kBackBufferFormat, 0);

		GetBackBuffer();

		XMMATRIX perPixelMatrix = XMMatrixTranspose(XMMATRIX( 2.0f/width,	 0.0f,		0.0f,		0.0f,
															  0.0f,			-2.0f/height,0.0f,		0.0f,
															  0.0f,			 0.0f,		1.0f,		0.0f,
															 -1.0f,			 1.0f,		0.0f,		1.0f));

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
	TRACE(TEXT("D3D::GetBackBuffer()\n"));
    ID3D11Texture2D *pBackBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    mDevice->CreateRenderTargetView(pBackBuffer, null, &mBackBuffer);
	Release(pBackBuffer);
    mDeviceContext->OMSetRenderTargets(1, &mBackBuffer, null);
}

//////////////////////////////////////////////////////////////////////

void D3D::InitShaders()
{
	TRACE(TEXT("D3D::InitShaders()\n"));
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

	CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
	mDevice->CreateSamplerState(&sampDesc, &mSamplerState);

	Release(PS_Buffer);
	Release(VS_Buffer);
}

