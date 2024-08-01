#include "Object3d.hlsli"

// TransformationMatrix 構造体の定義
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};

// Well 構造体の定義
struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};

// 定数バッファと構造化バッファの定義
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
StructuredBuffer<Well> gMatrixPalette : register(t0);

// VertexShaderInput 構造体の定義
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
};

// Skinned 構造体の定義
struct Skinned
{
    float4 position;
    float3 normal;
};

// Skinning 関数の実装
Skinned Skinning(VertexShaderInput input)
{
    Skinned skinned;
    
    // スキニングの位置変換
    skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f; // 確実に w を 1 に設定
    
    // スキニングの法線変換
    skinned.normal = mul(input.normal, (float3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal); // 正規化して戻す
    
    return skinned;
}

// main 関数の実装
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // スキニング計算を行い、スキニング後の頂点情報を取得
    Skinned skinned = Skinning(input);
    
    // スキニング結果を使って変換
    output.position = mul(skinned.position, gTransformationMatrix.WVP);
    output.worldPosition = (float3)mul(skinned.position, gTransformationMatrix.World);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(skinned.normal, (float3x3) gTransformationMatrix.WorldInverseTranspose));
    
    return output;
}
