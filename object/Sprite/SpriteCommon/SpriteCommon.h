#pragma once
#include <array>
#include <wrl.h>
#include <d3d12.h>

class SpriteCommon
{
public:
	/// <summary>
	///	テクスチャ読み込み
	/// </summary>
	/// <param name="index">テクスチャ番号</param>
	//void LoadTexture(uint32_t index);

private:
	// SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	// テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;
};

