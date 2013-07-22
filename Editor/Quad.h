#pragma once

class Quad
{
public:

	Quad()
	{
	}

	~Quad()
	{
	}

	void Create(ID3D11Device *device, float x, float y, float width, float height, int numLines)
	{
		Vertex *v = new Vertex[numLines * 6];

		int o = 0;
		for(int i=0; i<numLines; ++i)
		{
			v[o].mX = x;			v[o].mY = y;			v[o].mU = 0.0f;	v[o].mV = 0.0f;	v[o++].mColor = 0xffffffff;	// ABGR
			v[o].mX = x + width;	v[o].mY = y;			v[o].mU = 1.0f;	v[o].mV = 0.0f;	v[o++].mColor = 0xffffffff;
			v[o].mX = x;			v[o].mY = y + height;	v[o].mU = 0.0f;	v[o].mV = 1.0f;	v[o++].mColor = 0xffffffff;

			v[o].mX = x;			v[o].mY = y + height;	v[o].mU = 0.0f;	v[o].mV = 1.0f;	v[o++].mColor = 0xffffffff;
			v[o].mX = x + width;	v[o].mY = y;			v[o].mU = 1.0f;	v[o].mV = 0.0f;	v[o++].mColor = 0xffffffff;
			v[o].mX = x + width;	v[o].mY = y + height;	v[o].mU = 1.0f;	v[o].mV = 1.0f;	v[o++].mColor = 0xffffffff;
			y += height;
		}

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * 6 * numLines;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vertData;
		vertData.pSysMem = v;

		device->CreateBuffer(&vertexBufferDesc, &vertData, &mVertexBuffer);
	}

	void PrepareToDraw(ID3D11DeviceContext *context)
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Draw(ID3D11DeviceContext *context, int index)
	{
		context->Draw(6, index * 6);
	}

	void Destroy()
	{
		Release(mVertexBuffer);
	}

private:

	ID3D11Buffer *		mVertexBuffer;

};