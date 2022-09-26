/********************************
*								*
*	Legacy Model PS.hlsl		*
*								*
*	Created : 2022/07/26		*
*	Updated : 2022/07/26		*
*********************************/

#include "LegacyModelTypes.hlsli"

/* Texture Map */
Texture2D DiffuseMap : register( t0 );
Texture2D NormalMap  : register( t1 );

/* Sampler */
sampler   Sampler    : register( s0 );

/* View */
cbuffer cbView : register( b0 )
{
	float3 ViewPosition;
};

/* Directional Light */
cbuffer cbDirectionalLight : register( b1 )
{
	float3 LightDirection;
	float4 DLightDiffuse;
	float4 DLightAmbient;
	float  DLightSpecularPower;
}

/* Material */
cbuffer cbMaterial : register( b2 )
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
}

/* PS Main */
float4 main(VS_Output input) : SV_TARGET0
{
	float3 N = normalize( input.Normal );
	float3 T = input.Tangent;
	T = normalize( T - dot( T, N ) * N );
	float3 B = cross( N, T );

	float3x3 TSpace = { T, B, N };

	float4 DiffuseColor = DiffuseMap.Sample( Sampler, input.TexCoord );
	float3 Normal = NormalMap.Sample( Sampler, input.TexCoord ).xyz;
	Normal = mul( Normal.xyz, TSpace );

	// Ambient
	float4 ret = 0;
	ret.x = DLightAmbient.x * MaterialAmbient.x;
	ret.y = DLightAmbient.y * MaterialAmbient.y;
	ret.z = DLightAmbient.z * MaterialAmbient.z;

	// Diffuse & Specular
	float NDotL = dot( Normal, LightDirection );
	float Specular = 0.0f;

	if (NDotL > 0.0f)
	{
		ret += NDotL * DLightDiffuse * MaterialDiffuse;

		float3 V = normalize( ViewPosition - input.WorldPos ); // View Vector
		float3 H = normalize( LightDirection + V ); // Half Vector

		float NDotH = max( dot( Normal, H ), 0.0f );

		Specular = pow( NDotH, DLightSpecularPower );
	}

	// Gamma Correction
	DiffuseColor = pow(DiffuseColor, 2.2f);
	DiffuseColor = DiffuseColor * saturate(ret) + Specular * MaterialSpecular;
	DiffuseColor = pow(DiffuseColor, 1.0f / 2.2f);

	DiffuseColor.a = 1.0f;

	return DiffuseColor;
}