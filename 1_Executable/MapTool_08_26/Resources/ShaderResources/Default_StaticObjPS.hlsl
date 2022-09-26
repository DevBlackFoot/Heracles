#include "Default_StaticTypes.hlsl"

cbuffer CB_ViewPos : register(b0)
{
	float3 viewPosition;
	float1 pad;
};
cbuffer CB_Material : register(b1)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
}

cbuffer CB_FxDirectionalLight : register(b2)
{
	float4 directionalLight_Ambient;
	float4 directionalLight_Diffuse;
	float4 directionalLight_Specular;
	float3 directionalLight_Direction;
	float directionalLight_power;
};

cbuffer CB_MaterialReflect : register(b3)
{
	float4 reflect;	
}

SamplerState samAnisotropic : SAMPLER: register(s0)
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D textureMap:register(t0);
Texture2D normalMap:register(t1);

float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
	
	float4 DiffuseColor = textureMap.Sample( samAnisotropic, input.texcoord1 );
	


	return DiffuseColor;
}