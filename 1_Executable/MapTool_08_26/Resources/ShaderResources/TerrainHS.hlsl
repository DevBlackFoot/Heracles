#include "TerrainHeader.hlsl"

cbuffer CB_TerrainData : register( b0 )
{    
    float3 cameraPosition;
    float3 lightDirection;
	float pad1;
	float pad2;
};

HS_CONSTANT_DATA_OUTPUT BezierConstantHS( InputPatch<VS_OUTPUT, 16> ip, uint PatchID : SV_PrimitiveID )
{    
    HS_CONSTANT_DATA_OUTPUT Output;
    	
	float3 Top = (ip[5].Position + ip[6].Position)/2.0F;
	float3 Bottom = (ip[9].Position + ip[10].Position)/2.0F;
	float3 Left = (ip[5].Position + ip[9].Position)/2.0F;
	float3 Right = (ip[6].Position + ip[10].Position)/2.0F;
	float3 Center = (ip[5].Position + ip[10].Position)/2.0F;

	float4 D = float4(
		length(Top - cameraPosition),
		length(Bottom - cameraPosition),
		length(Left - cameraPosition),
		length(Right - cameraPosition));

	
	float section=75.0F;
	float maxTile=15;
	D=clamp(ceil(D/section),0,maxTile);
	D=16-D;
	
	Output.Edges[0] = (int)D.z;
	Output.Edges[1] = (int)D.x;
	Output.Edges[2] = (int)D.w;
	Output.Edges[3] = (int)D.y;

	float f=length(Center - cameraPosition);
	float DC= 16-clamp(ceil(f/section),0,maxTile);
	
	Output.Inside[0] = Output.Inside[1] = DC;
	
    return Output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("BezierConstantHS")]
HS_OUTPUT HSMain( InputPatch<VS_OUTPUT, 16> p, 
                    uint i : SV_OutputControlPointID,
                    uint PatchID : SV_PrimitiveID )
{
    HS_OUTPUT Output;
    Output.Position = p[i].Position;
	Output.TexCoord = p[i].TexCoord;
    return Output;
}