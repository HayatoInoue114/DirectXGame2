#pragma once
#include "../MT/MT.h"
#include "../../base/DirectX12/DirectX12.h"
#include <wrl.h>
#include "../../ViewProjection/ViewProjection.h"

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};


struct WorldTransform {

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };

	Matrix4x4 matWorld = {};
	Matrix4x4 worldMatrix = {};
	Matrix4x4 sMatWorld = {};

	void Initialize();

	void TransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, ViewProjection& view);

	void STransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, ViewProjection& view);

	void UpdateMatrix();

};