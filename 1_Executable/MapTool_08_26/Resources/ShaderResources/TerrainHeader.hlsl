struct VS_OUTPUT
{
	float3 Position    : POSITION;	
	float3 TexCoord    : TEXCOORD;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float Edges[4]             : SV_TessFactor;
    float Inside[2]            : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 Position : BEZIERPOS;
	float3 TexCoord : TEXCOORD;
};

struct DS_OUTPUT
{
    float4 Position       : SV_POSITION;
    float3 WorldPos       : WORLDPOS;
	float3 LightDirection : LIGHTDIR;
	float3 ViewDirection  : VIEWDIR;
	float3 Normal         : NORMAL;
	float2 TexCoord       : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 color : SV_TARGET;
};

float4 BSplineBasis(float t)
{
    float invT = 1.0f - t;

    return float4( invT * invT * invT ,
                   3.0f * t * t * t - 6.0F * t * t + 4,
                   3.0f * (-t * t * t + t * t + t ) + 1,
                   t * t * t ) / 6.0F;
}


float4 DBSplineBasis(float t)
{
    float invT = 1.0f - t;

    return float4( -3 * invT * invT,
                   9 * t * t - 12 * t,
                   - 9 * t * t + 6 * t + 3,
                   3 * t * t );
}


float3 EvaluateBezier( float3 P[16], float4 BasisU, float4 BasisV )
{
    float3 Value = float3(0,0,0);

    Value  = BasisV.x * ( P[0] * BasisU.x + P[1] * BasisU.y + P[2] * BasisU.z + P[3] * BasisU.w );
    Value += BasisV.y * ( P[4] * BasisU.x + P[5] * BasisU.y + P[6] * BasisU.z + P[7] * BasisU.w );
    Value += BasisV.z * ( P[8] * BasisU.x + P[9] * BasisU.y + P[10] * BasisU.z + P[11] * BasisU.w );
    Value += BasisV.w * ( P[12] * BasisU.x + P[13] * BasisU.y + P[14] * BasisU.z + P[15] * BasisU.w );
    return Value;
}


float2 EvaluateTex(float2 tex[16], float2 uv)
{
    return lerp(tex[5],tex[10],uv.xy) ;
}

float4 OrenNayar (float3 n, float3 l, float3 v) 
{
	float fRoughness=0.003F;    
    float gamma=dot(v-n*dot(v,n),l-n*dot(l,n));
    float rough_sq = fRoughness * fRoughness;
 
    float A = 1.0f - 0.5f * (rough_sq / (rough_sq + 0.57f));
 
    float B = 0.45f * (rough_sq / (rough_sq + 0.09));
 
    float alpha = max( acos( dot( v, n ) ), acos( dot( l, n ) ) );
    float beta  = min( acos( dot( v, n ) ), acos( dot( l, n ) ) );
    float C = sin(alpha) * tan(beta);
    float final = (A + B * max( 0.0f, gamma ) * C);
    return max( 0.0f, dot( n, l ) ) * final;
}