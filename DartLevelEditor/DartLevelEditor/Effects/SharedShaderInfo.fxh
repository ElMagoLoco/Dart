struct InputVS_VP
{
	float3 Position : POSITION0;
};

struct InputVS_VPC
{
	float3 Position : POSITION0;
	float4 Color : COLOR0;
};

struct InputVS_VPN
{
	float3 Position : POSITION0;
	float3 Normal : NORMAL0;
};

struct InputVS_VPNT
{
	float3 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 Tex : TEXCOORD0;
};

struct InputVS_VPNC
{
	float3 Position : POSITION0;
	float3 Normal : NORMAL0;
	float4 Color : COLOR0;
};

struct InputVS_POM
{
	float3 Position : POSITION0;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 Binormal : BINORMAL0;
};

struct InputPS_General
{
	float4	PositionH : POSITION0;
	float3	Normal : TEXCOORD0;
	float4	Color : TEXCOORD1;
	float2	Tex : TEXCOORD2;
	float3	PositionW : TEXCOORD3;
	float		DistToEye : TEXCOORD4;
};

shared float4x4 gViewProj;
shared float4x4 gView;
shared float4x4 gWorld;
shared float4x4 gWVP;
shared float4x4 gWIT;
shared float4 gEye;
shared float4 gLightDir;
shared float4 gLightDiffuse;
shared float4 gLightAmbient;
shared float4 gLightSpecular;
shared float4 gLightPos;
shared texture2D gTexDiffuse;
shared float4 gMtrlAmbientColor;
shared float4 gMtrlDiffuseColor;
shared float4 gMtrlSpecularColor;
shared float gSpecularExp;

sampler2D SamplerLinearNoAnisotropyNoMips = sampler_state
{
	Texture = (gTexDiffuse);
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = WRAP;
	AddressV = WRAP;
};