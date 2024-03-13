#include "Particle.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
};

struct DirectionalLight
{
	float32_t4 color; //!< ライトの色
	float32_t3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1); 

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformdUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
	output.color = gMaterial.color * textureColor * input.color;

	//textureのa値が0.5以下の時にPixelを棄却
	if(output.color.a == 0.0) {
		discard;
	}
	return output;
}