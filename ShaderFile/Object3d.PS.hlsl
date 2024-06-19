#include "Object3d.hlsli"

struct Material {
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
};

struct DirectionalLight
{
	float4 color; //!< ライトの色
	float3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1); 

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformdUV = mul(float4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);

	//textureのa値が0.5以下の時にPixelを棄却
	if(textureColor.a <= 0.5) {
		discard;
	}

	if (gMaterial.enableLighting != 0) {// LIghtingする場合
		//half lambert
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else {
		output.color = gMaterial.color * textureColor;
	}

	return output;
}