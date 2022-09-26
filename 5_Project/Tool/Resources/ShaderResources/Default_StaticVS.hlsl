#include "Default_StaticTypes.hlsl"

cbuffer CB_Matrix : register(b0)
{	
	float4x4 world;
	float4x4 worldInvTranspose;
	float4x4 worldViewPojection;
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
	
	Output.position = mul(worldViewPojection, input.position);	
	Output.texcoord1 = input.texcoord1;
	Output.normal = mul((float3x3)world, input.normal);
	Output.tangent = mul((float3x3)world, input.tangent);
	Output.worldPos = mul(input.position, world);
	Output.binormal = mul((float3x3)world, input.binormal);
	Output.pad = 0.0f;

	return Output;
}