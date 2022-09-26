#include "TerrainHeader.hlsl"

struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord1 : TEXCOORD1;
	float2 texcoord2 : TEXCOORD2;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float4 bone : BONE;
	float4 weight : WEIGHT;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT vout;

	vout.Position = input.position.xyz;
	vout.TexCoord = float3(input.texcoord1.xy, 0.0f);

	return vout;
}