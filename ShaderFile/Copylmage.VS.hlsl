#include "Copylmage.hlsli"

static const int kNumVertex = 3;
static const float4 kPositions[kNumVertex] =
{
    { -1.0f, 1.0f, 0.0f, 1.0f }, //左上
    { 3.0f, 1.0f, 0.0f, 1.0f }, // 右上
    { -1.0f, -3.0f, 0.0f, 1.0f } //左下
};
static const float2 kTexcoords[kNumVertex] =
{
    { -1.0f, 1.0f, 0.0f, 1.0f }, //左上
    { 3.0f, 1.0f, 0.0f, 1.0f }, // 右上
    { -1.0f, -3.0f, 0.0f, 1.0f } //左下
};
VertexShaderOutput main(int vertexId : SV_VertexID)
{
    VertexShaderOutput output;
    output.position = kPositions[vertexId];
    output.texcoord = kTexcoords[vertexId];
    return output;
}