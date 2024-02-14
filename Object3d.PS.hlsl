#include "Object3d.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
};

struct DirectionalLight
{
	float32_t4 color; //!< ライトの色
	float32_t3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};


struct Camera {
	float32_t3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1); 
ConstantBuffer<Camera> gCamera : register(b2);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformdUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
	float cos = 0.0f;

	//textureのa値が0.5以下の時にPixelを棄却
	if(textureColor.a <= 0.5) {
		discard;
	}

	if (gMaterial.enableLighting != 0) {// LIghtingする場合
		//half lambert
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else {
		output.color = gMaterial.color * textureColor;
	}

	float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
	float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));

	float RdotE = dot(reflectLight,toEye);
	float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度

	//拡散反射
	float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
	
	//鏡面反射
	float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f,1.0f,1.0f);
	
	//拡散反射*鏡面反射
	output.color.rgb = diffuse * specular;

	//αは今まで通り
	output.color.a = gMaterial.color.a * textureColor.a;

	return output;
}