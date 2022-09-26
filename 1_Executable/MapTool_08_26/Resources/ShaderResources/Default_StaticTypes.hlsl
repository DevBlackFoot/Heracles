struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord1 : TEXCOORD1;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float4 worldPos : POSITION;
	float1 pad : PSIZE;	
};