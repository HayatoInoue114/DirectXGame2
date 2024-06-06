struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float4 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
    float worldPosition : WORLDPOSITION;
};