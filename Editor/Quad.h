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

	void Create(ID3D11Device *device, uint maxQuads)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * 6 * maxQuads;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.StructureByteStride = sizeof(Vertex);
		device->CreateBuffer(&vertexBufferDesc, null, &mVertexBuffer);
	}

	void Set(int index, float x, float y, float w, float h, float u1, float v1, float u2, float v2, uint32 color)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		if(SUCCEEDED(d3d.GetDeviceContext()->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
		{
			Vertex *v = reinterpret_cast<Vertex *>(msr.pData);
			int o = index * 6;
			v[o].mX = x;		v[o].mY = y;		v[o].mU = u1;	v[o].mV = v1;	v[o++].mColor = color;	// ABGR
			v[o].mX = x + w;	v[o].mY = y;		v[o].mU = u2;	v[o].mV = v1;	v[o++].mColor = color;
			v[o].mX = x;		v[o].mY = y + h;	v[o].mU = u1;	v[o].mV = v2;	v[o++].mColor = color;
			v[o].mX = x;		v[o].mY = y + h;	v[o].mU = u1;	v[o].mV = v2;	v[o++].mColor = color;
			v[o].mX = x + w;	v[o].mY = y;		v[o].mU = u2;	v[o].mV = v1;	v[o++].mColor = color;
			v[o].mX = x + w;	v[o].mY = y + h;	v[o].mU = u2;	v[o].mV = v2;	v[o++].mColor = color;
			d3d.GetDeviceContext()->Unmap(mVertexBuffer, 0);
		}
	}

	void Draw(int numQuads)
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		ID3D11DeviceContext *context = d3d.GetDeviceContext();
		context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->Draw(numQuads * 6, 0);
	}

	void Destroy()
	{
		Release(mVertexBuffer);
	}

private:

	ID3D11Buffer *		mVertexBuffer;

};