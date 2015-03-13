#include "SharedShaderInfo.fxh"

//struct Input_PS
//{
//	float4 PositionH : POSITION0;
//	float4 Color : COLOR0;
//};

InputPS_General LevelEditorVS(InputVS_VPC In)
{
	InputPS_General Out = (InputPS_General)0;
	Out.PositionH = mul(float4(In.Position, 1.0f), gWVP);
	Out.Color = In.Color + gMtrlDiffuseColor;

	return Out;
}

float4 LevelEditorPS(InputPS_General In) : COLOR
{
	return In.Color;//float4(1.0f, 0.0f, 0.0f, 1.0f);
}


technique LevelEditorTech
{
	pass P0
	{
		Lighting = FALSE;
	//	FillMode = WireFrame;
		Ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
		VertexShader = compile vs_3_0 LevelEditorVS();
		PixelShader = compile ps_3_0 LevelEditorPS();
	}
}