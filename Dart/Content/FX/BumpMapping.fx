// Bump Mapping by Darrell Harrison Smith 2014
////////////////////////////////////
//gloabl variables
////////////////////////////////////
//world matrices
float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJ;
//texture
texture gModelTexture : TEXTURE;
texture gNormalTexture : NORMALTEX;
//lighting
float3 gLightDirection : LIGHTDIRECTIONDIR;
float4 gLightColor : LIGHTCOLORDIR;
float gLightIntensity : LIGHTINTENSITYDIR;
float4 gSpecularColor : SPECULARCOLOR;
float gSpecularIntensity : SPECULARINTENSITY;
float gSpecularPower : SPECULARPOWER;
float gAmbientIntensity : AMBIENTINTENSITY;
float4 gAmbientColor : AMBIENTCOLOR;
//eye direction
float3 gEyePos : EYEPOS;
//other 
bool gParallax = true;
bool gSelfShadow = false;
float gScale = 0.04f;
float gBias = 0.02f;

//samplers
sampler textureSampler = sampler_state
{
    Texture = <gModelTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU  = WRAP;
    AddressV  = WRAP;
};
sampler normalSampler = sampler_state
{
    Texture = <gNormalTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU  = WRAP;
    AddressV  = WRAP;
};
////////////////////////////////////
//vs input and output
////////////////////////////////////
struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

struct VS_OUTPUT
{
    float4 position : POSITION0;
    float2 texCoord : TEXCOORD0;
    float3 direction : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
};

////////////////////////////////////
//vs and ps
////////////////////////////////////
void VS(in VS_INPUT IN, out VS_OUTPUT OUT)
{
    OUT.position = mul(IN.position, gWorldViewProj);//transform position
    // 3x3 matrix transforms world space to tangent space
    float3x3 WorldToTangent;
    WorldToTangent[0] = mul(IN.tangent, gWorld);
    WorldToTangent[1] = mul(cross(IN.tangent, IN.normal), gWorld);
    WorldToTangent[2] = mul(IN.normal, gWorld);
    OUT.direction = mul(WorldToTangent, gLightDirection);
    float4 PosWorld = mul(IN.position, gWorld);
    OUT.viewDir = mul(WorldToTangent, gEyePos - PosWorld);
    OUT.texCoord = IN.texCoord; 
}

float4 PS(in VS_OUTPUT IN) : COLOR
{
    // ambient
    float4 Ambient = gAmbientIntensity * gAmbientColor;
    Ambient.w = 1.0f;

    float3 LightDir = normalize(IN.direction);
    float3 ViewDir = normalize(IN.viewDir);

    // texture and normal map    
    float4 TexColor;
    float3 Normal;
    if(gParallax)
    {
        //fetch height map
        TexColor = gScale * tex2D(textureSampler, IN.texCoord) - gBias;
        float Height = TexColor.w;
        //computer new tex coords    
        float2 TexColorCorrected = Height * ViewDir + IN.texCoord;
        //fetch color map
        TexColor = tex2D(textureSampler, TexColorCorrected);
        //map normal from 0.0 to 1.0 range into -1.0 to 1.0 range
        Normal = (2.0f * (tex2D(normalSampler, TexColorCorrected))) - 1.0f;
    }
    else
    {
        TexColor = tex2D(textureSampler, IN.texCoord);
        // map normal from 0.0 to 1.0 range into -1.0 to 1.0 range
        Normal = (2.0f * (tex2D(normalSampler, IN.texCoord))) - 1.0f;
    }
    // diffuse
    float Diffuse = saturate(dot(Normal, LightDir));
    float4 DiffuseIC = gLightIntensity * gLightColor * Diffuse;
    DiffuseIC.w = 1.0f;
    //self shadowing
    float Shadow;
    if(gSelfShadow)
    {
        Shadow = 4.0f * LightDir.z;
    }
    else
    {
        Shadow = 1.0f;
    }
    float4 Color;
    if(Shadow > 0.0f)
    {
        // reflection
        float3 Reflect = normalize(2 * Diffuse * Normal - LightDir);
        // specular
        float Specular = min(pow(saturate(dot(Reflect, ViewDir)), gSpecularPower), TexColor.w);
        float4 SpecularIC = gSpecularIntensity * gSpecularColor * Specular;
    
        Color = (Ambient + DiffuseIC * 3.0f) / 4.0f + SpecularIC;
    }
    else
    {
        Color = (Ambient + DiffuseIC * 3.0f) / 4.0f;
    }
    return TexColor * Color;
}
//6370
////////////////////////////////////
//technique
////////////////////////////////////
technique Main
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		
    }
}