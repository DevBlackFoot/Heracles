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

cbuffer CB_Material : register(b4)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
}

cbuffer CB_MaterialReflect : register(b5)
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
	if(directionalLight.pad < 1.0f)
	{

		input.normal = normalize(input.normal);

		float3 toEye = viewPosition - input.worldPos.xyz;
		float distToEye = length(toEye);
		toEye /= distToEye;

		float4 texColor = textureMap.Sample(samAnisotropic, input.texcoord1);
		
		float3 _newNormal = normalMap.Sample(samAnisotropic, input.texcoord1);
		_newNormal = _newNormal * 2.0f - 1.0f;

		float3 _binormal = cross(input.normal, input.tangent);

		float3x3 _tangentSpace = float3x3(input.tangent, _binormal, input.normal);
		_newNormal = mul(_newNormal, _tangentSpace);
		input.normal = normalize(_newNormal);

		float4 _ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 _diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 _spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		float4 A, D, S;
		ComputeDirectionalLight(ambient, diffuse, specular, reflect, 
			directionalLight, input.normal, toEye,	A, D, S);

		_ambient += A * directionalLight.pad;
		_diffuse += D * directionalLight.pad;
		_spec += S * directionalLight.pad;

		ComputePointLight(ambient, diffuse, specular, reflect, 
			pointLight, (float3)input.worldPos, input.normal, toEye, A, D, S);

		_ambient += A;
		_diffuse += D;
		_spec += S;

		ComputeSpotLight(ambient, diffuse, specular, reflect, 
			spotLight, (float3)input.worldPos, input.normal, toEye,	A, D, S);

		_ambient += A;
		_diffuse += D;
		_spec += S;

		texColor.rgb *= texColor.rgb;
		texColor = texColor * (_ambient + _diffuse) + _spec;	
		texColor.rgb = sqrt(texColor.rgb);		

		return texColor;
	}
	else
	{
		return textureMap.Sample(samAnisotropic, input.texcoord1);
	}
}