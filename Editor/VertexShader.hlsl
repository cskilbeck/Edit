//////////////////////////////////////////////////////////////////////

cbuffer Matrix
{
	matrix screen;
};

//////////////////////////////////////////////////////////////////////

struct VS_Input
{
	float2 position: POSITION;
	float2 texcoord: TEXCOORD0;
	float4 color: COLOR;
};

//////////////////////////////////////////////////////////////////////

struct VS_Output
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float4 color: COLOR;
};

//////////////////////////////////////////////////////////////////////

VS_Output main(VS_Input i)
{
    VS_Output o;
    o.position = mul(float4(i.position.x, i.position.y, 1.0f, 1.0f), screen);
	o.texcoord = i.texcoord;
    o.color = i.color;
    return o;
}