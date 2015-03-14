// Global variables - Passed in from application

float4x4 worldViewProjMat:WORLDVIEWPROJ;          // Transforms positions to view space
float4x4 worldInverseTransposeMat:WORLDINVTRANS;  // Transforms normals to world space
float4x4 worldMat:WORLD;                  // World matrix
float3 eyePos:EYEPOS;                      // Eye position (camera position)

// Animation Data
float4x4 finalXForms[35];           // Matrix Palette

// Light
float3 lightDirection:LIGHTDIRECTIONDIR; //light direction
float3 ambientLight:AMBIENTCOLOR;                // Ambient Light Color
float3 specularLight:SPECULARCOLOR;               // Specular Light Color
float3 diffuseLight:LIGHTCOLORDIR;                // Diffuse Light Color
float ambientIntensity:AMBIENTINTENSITY;
float directionalIntensity:LIGHTINTENSITYDIR;
float specularIntensity:SPECULARINTENSITY;
float  specularPower:SPECULARPOWER;               // Specular Power

// Texture
bool isTex = true;
texture tex:TEXTURE;                        // Image texture to be applied to object
texture ntex:NORMALTEX;

sampler texSamp = sampler_state     // Texture sampler state
{
    Texture = <tex>;
    // Trilinear Filtering
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_OUT
{
    float4 posH : POSITION0;        // Vertex position in homgenous clip space
    float3 normW : TEXCOORD0;       // Vertex normal in world space
    float3 posW : TEXCOORD1;        // Vertex position in world space
    float2 tex0 : TEXCOORD2;        // Vertex uv texture coordinate 0
};

// Vertex Shader
VS_OUT VS(float3 pos : POSITION0, float3 norm : NORMAL0, float2 tex0 : TEXCOORD0, 
    float weight0 : BLENDWEIGHT0, int4 boneIndex : BLENDINDICES0)
{
    VS_OUT vsOut = (VS_OUT) 0;
    
    // Animation vertex processing
    float weight1 = 1.0f - weight0;
    
    float4 p = weight0 * mul(float4(pos, 1.0f), finalXForms[boneIndex[0]]);
    p       += weight1 * mul(float4(pos, 1.0f), finalXForms[boneIndex[1]]);
    p.w = 1.0f;
    
    // We do not normalize normals, done in pixel shader now.
    float4 n = weight0 * mul(float4(norm, 0.0f), finalXForms[boneIndex[0]]);
    n       += weight1 * mul(float4(norm, 0.0f), finalXForms[boneIndex[1]]);
    n.w = 0.0f;
    
    // Transform vertex normal to world space (norm * WITMat)
    vsOut.normW = mul(n, worldInverseTransposeMat).xyz;

    // Transform vertex position to world space
    vsOut.posW = mul(p, worldMat).xyz;  
    
    // Transform vertex position to homogenous clip space (pos * WVPMat)
    vsOut.posH = mul(p, worldViewProjMat);
    
    // Normalize the normals to unit length
    //vsOut.normW = normalize(vsOut.normW);
    
    // Pass on texture coordinate to pixel shader
    vsOut.tex0 = tex0;

    return vsOut;   
}

// Pixel Shader
float4 PS(VS_OUT vsOut): COLOR
{
    // Get texel from texture map taht gets mapped to this pixel
    float4 texColor = tex2D(texSamp, vsOut.tex0);

    // Interpolated normals can "unnormalize", so normalize
    vsOut.normW = normalize(vsOut.normW);
    
    // Compute vector from the vertex to the eye position (V)
    float3 toEye = normalize(eyePos - vsOut.posW);
    
  
    // Determine how much specular (if any), makes it to the eye
  
    //////////////////////////////////////////////////////////////////////////
    // Blinn-Phong specular calculation
    //////////////////////////////////////////////////////////////////////////
    // Half vector used in Blinn-Phong shading (faster than Phong)
    float3 H = normalize(-lightDirection + toEye);
    float T = pow(max(dot(H, vsOut.normW), 0.0f), specularPower);
    
    // Determine the diffuse light intensity that strikes the vertex
    float S = max(dot(lightDirection, vsOut.normW), 0.0f);
    
    // Compute Ambient, Diffuse, Specular terms seperately
    float3 spec = T * specularLight.rgb;
    float3 diffuse;
    diffuse = S * diffuseLight.rgb;
    
    float3 ambient = ambientLight;
    
    // Sum all of the terms together and copy diffuse alpha
    return float4(((ambient + diffuse * directionalIntensity) * texColor.rgb + spec * specularIntensity) , texColor.a);
}

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