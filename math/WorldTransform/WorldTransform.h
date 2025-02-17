#pragma once
#include "../MT/MT.h"
#include "../../base/DirectX12/DirectX12.h"
#include <wrl.h>

#include "../TransformationMatrix.h"

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld; // ローカル → ワールド変換行列
	Matrix4x4 WorldInverseTranspose;  // ワールド行列を逆転置行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotate = { 0, 0, 0 };
	// ローカル座標
	Vector3 translate = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_{};
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void UpdateMatrix();
};