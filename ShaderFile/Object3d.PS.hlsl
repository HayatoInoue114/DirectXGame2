#include "Object3d.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    int eneblePhong;
    float shininess;
    int enableEnvTexture;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float4> gTexture : register(t0);
TextureCube<float4> gEnviromentTexture : register(t1);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    output.color = gMaterial.color * textureColor;
    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

    output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));

    float RdotE = dot(reflectLight, toEye);
    float specularPow = pow(saturate(RdotE), gMaterial.shininess);

    float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;

    float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

    //output.color.rgb = diffuse + specular;
    //output.color.a = gMaterial.color.a * textureColor.a;
    if (gMaterial.eneblePhong != 0)
    {
      
    }
    if (gMaterial.enableLighting != 0)
    {
       
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    float3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
    float3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
    float4 enviromentColor = gEnviromentTexture.Sample(gSampler, reflectedVector);
		
    output.color.rgb += enviromentColor.rgb;
    output.color.a = gMaterial.color.a * textureColor.a;
    if (gMaterial.enableEnvTexture != 0)
    {
        
    }

    return output;
}