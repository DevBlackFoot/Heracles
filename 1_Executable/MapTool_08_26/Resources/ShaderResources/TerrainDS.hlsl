#include "TerrainHeader.hlsl"

cbuffer CB_TerrainData : register ( b0 )
{    
    float3 cameraPosition;
    float3 lightDirection;
	float pad1;
	float pad2;
};

cbuffer CB_Matrix : register (b1)
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

[domain("quad")]
DS_OUTPUT DSMain( HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 16> bezpatch )
{
	
	float3 P[16];
	float2 T[16];
	for(int i=0;i<16;i++)
	{
		float3 tp=bezpatch[i].Position;
		
		T[i] = bezpatch[i].TexCoord.xy;
		float I=HeightMap.SampleLevel(samTriLinearSam, bezpatch[i].TexCoord.xy/16,0,0).x;

		P[i] = tp + float3(0, 0, I*800.0F);
	}

	
    float4 BasisU = BSplineBasis( UV.x );
    float4 BasisV = BSplineBasis( UV.y );
    float4 dBasisU = DBSplineBasis( UV.x );
    float4 dBasisV = DBSplineBasis( UV.y );

    float3 WorldPos = EvaluateBezier( P, BasisU, BasisV );
	
	float2 TexPos = EvaluateTex( T, UV);
    float3 Tangent = EvaluateBezier( P, dBasisU, BasisV );
    float3 Binormal = EvaluateBezier( P, BasisU, dBasisV );
	
    float3 Normal = normalize( cross( Tangent, Binormal ) );

    DS_OUTPUT Output;
	float3x3 tangentMat=float3x3(normalize(Tangent),normalize(Binormal),normalize(Normal));
	
    Output.Position = mul( float4(WorldPos,1), worldViewPojection );
    Output.WorldPos = WorldPos;
	Output.TexCoord= TexPos;
	
	Output.LightDirection=mul(tangentMat,lightDirection);
	Output.ViewDirection=mul(tangentMat,normalize(cameraPosition));
	Output.Normal=Normal;
    return Output;    
}