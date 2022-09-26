#include "Default_StaticTypes.hlsl"

SamplerState textureSampler;
Texture2D textureMap:register(t0);
Texture2D normalMap:register(t1);


float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
	float4 D;
	D.r = 1.0f;
	D.g = 1.0f;
	D.b = 1.0f;
	D.a = 1.0f;
	
	return D;
}