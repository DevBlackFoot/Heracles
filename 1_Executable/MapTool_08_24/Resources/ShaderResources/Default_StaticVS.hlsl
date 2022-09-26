#include "Default_StaticTypes.hlsl"

cbuffer CB_Matrix : register(b0)
{
	float4x4 transform;
	float4x4 world;
};

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
	VS_OUTPUT Output;	
	float3 N = mul(world, input.normal);
	float3 T = mul(world, input.tangent);
	float3 B = mul(world, input.binormal);

	float3x3 Tangent = { T,B,N };
	
    Output.position = mul(transform, input.position);
	Output.texcoord1 = input.texcoord1;

	return Output;
}