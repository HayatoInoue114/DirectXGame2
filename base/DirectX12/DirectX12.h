#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "../../utility/FormatString/FormatString.h"
#include <dxgidebug.h>
#include "../../math/Vector4.h"
#include "../../manager/ImGuiManager/ImGuiManager.h"
#include "../../externals/DirectXTex/DirectXTex.h"
#include <wrl.h>
#include <chrono>

#include "../WindowsAPI/WindowsAPI.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib")

class DirectX12
{
public:

	static DirectX12* GetInstance();

	void Init();

	void Finalize();


	/// <summary>
/// 描画前処理
/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	void PreDrawForPostEffect();

	void PostDrawForPostEffect();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);


	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_; }

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList_; }

	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	uint32_t GetSrvIndex() { return srvIndex_; }

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return commandQueue_; }

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator() { return commandAllocator_; }

	Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain() { return swapChain_; }
private:
	void DXGIFactory();

	void Adapter();

	void D3D12Device();

	void Command();

	void SwapChain();

	void DescriptorHeap();

	//FPS固定化初期化
	void InitializeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

	/*void RTV();*/



	void DebugLayer();

	void Error();

	void Barrier();

	void ScreenDisplay();

	void CommandConfirm();

	void Fence();

	/*void Update();*/

	void CreateDepthStencilResource();

	void CreateDSV();

	void SetRenderTargets();

	void InitializeDescriptorSize();

	//void SetImGuiDescriptorHeap();

	void PushImGuiDrawCommand();

	void GetBackBuffer();

	void ClearRTV();

	void CommandKick();

	void Signal();

	void NextFlameCommandList();

	void ClearDepthBuffer();

public:
		uint64_t fenceValue_{};
private:
	Microsoft::WRL::ComPtr<ID3D12Device> device_{};
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_{};

	DXGI_ADAPTER_DESC3 adapterDesc_{};

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_{};

	//コマンドキューを生成する
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_{};
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

	//コマンドアロケータを生成する
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_{};

	//コマンドリストを生成する
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_{};
	
	//スワップチェーンを生成する
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	//ディスクリプターヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_{};

	//RTVを２つ作るのでディスクリプタ２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_[3]{};

	
	UINT backBufferIndex_{};
	
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResource_[2]{};
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier_{};
	
	//初期値0でFenceを作る
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_{};
	

	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent_{};

	DirectX::TexMetadata metadata_{};
	DirectX::TexMetadata metadata2_{};

	ID3D12Resource* textureResource_{};
	ID3D12Resource* textureResource2_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_{};
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};

	

	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	uint32_t descriptorSizeRTV_{};
	uint32_t descriptorSizeDSV_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_;

	uint32_t srvIndex_;
};

