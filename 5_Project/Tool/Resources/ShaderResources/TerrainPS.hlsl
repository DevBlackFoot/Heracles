#include "TerrainHeader.hlsl"

cbuffer CB_TerrainData : register( b1 )
{    
    float3 cameraPosition;
    float3 lightDirection;
	float pad1;
	float pad2;
};

cbuffer CB_Matrix : register(b0)
{	
	float4x4 world;
	float4x4 worldInvTranspose;
	float4x4 worldViewPojection;
};

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

Texture2D<float4> HeightMap;
Texture3D<float4> DiffuseMap;
Texture3D<float4> NormalMap;

PS_OUTPUT PSMain(DS_OUTPUT input)
{
	PS_OUTPUT outP;

	float3 No = normalize(input.Normal);
	float4 diffuse =  DiffuseMap.Sample( samTriLinearSam, float3(input.TexCoord.xy,No.z));
	float3 N = normalize(NormalMap.Sample(samTriLinearSam, float3(input.TexCoord.xy,No.z)).xyz*2.0f-1.0f);

	float3 L = normalize(input.LightDirection.xyz);
	float3 V = normalize(input.ViewDirection.xyz);
    
	outP.color = OrenNayar(N,-L,V) * diffuse ;

	return outP;
}