#include "SkyBoxHeader.hlsl"

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

TextureCube gCubeMap;


float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
	return gCubeMap.Sample(samTriLinearSam, input.PosL);	
}