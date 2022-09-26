#include "Default_StaticTypes.hlsl"
#include "LightHelper.fx"

cbuffer CB_ViewPos : register(b0)
{
	float3 viewPosition;
	float1 pad;
};

cbuffer CB_FxDirectionalLight : register(b1)
{
	FxDirectionalLight directionalLight;
};

cbuffer CB_FxPointLight : register(b2)
{
	FxPointLight pointLight;
};

cbuffer CB_FxSpotLight : register(b3)
{
	FxSpotLight spotLight;
};

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

	float4 D = textureMap.Sample(samAnisotropic, input.texcoord1);
	float3 N = normalMap.Sample(samAnisotropic, input.texcoord1).xyz*2.0f - 1.0f;
	N = normalize(N);	
	D.a = 1.0f;		

	return D;
}