#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "FormatString.h"
#include <dxgidebug.h>
#include "Vector4.h"
#include "ImGuiManager.h"
#include "externals/DirectXTex/DirectXTex.h"

#include "WindowsAPI.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class WindowsAPI;

class DirectX12
{
public:

	void Init(WindowsAPI* windowsAPI);

	void DXGIFactory();

	void Adapter();

	void D3D12Device();

	void Command();

	void SwapChain();

	void DescriptorHeap();

	/*void RTV();*/

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();
	
	void DebugLayer();

	void Error();

	void Barrier();

	void ScreenDisplay();

	void CommandConfirm();

	void Fence();

	/*void Update();*/

	void ResourceLeakCheck();

	void Release();

	void SetImGuiDescriptorHeap();

	void PushImGuiDrawCommand();

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);


	ID3D12Device* GetDevice() { return device_; }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }

	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	ID3D12DescriptorHeap* GetSRVDescriptorHeap() { return srvDescriptorHeap_; }

	void LoadAndTransferTexture();

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void CreateSRV();

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU_; }

	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	void CreateDepthStencilResource();

	void CreateDSV();
public:
	void GetBackBuffer();

	void ClearRTV();

	void CommandKick();

	void Signal();

	void NextFlameCommandList();

private:
	WindowsAPI* windowsAPI_{};
	ID3D12Device* device_{};
	IDXGIFactory7* dxgiFactory_{};

	DXGI_ADAPTER_DESC3 adapterDesc_{};

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter_{};

	//コマンドキューを生成する
	ID3D12CommandQueue* commandQueue_{};
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

	//コマンドアロケータを生成する
	ID3D12CommandAllocator* commandAllocator_{};

	//コマンドリストを生成する
	ID3D12GraphicsCommandList* commandList_{};

	//スワップチェーンを生成する
	IDXGISwapChain4* swapChain_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	//ディスクリプターヒープの生成
	ID3D12DescriptorHeap* rtvDescriptorHeap_{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_{};

	//RTVを２つ作るのでディスクリプタ２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_[2]{};

	
	UINT backBufferIndex_{};
	
	ID3D12Resource* swapChainResource_[2]{};
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier_{};
	
	//初期値0でFenceを作る
	ID3D12Fence* fence_{};
	uint64_t fenceValue_{};

	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent_{};

	IDXGIDebug1* debug_{};

	ID3D12DescriptorHeap* srvDescriptorHeap_{};

	DirectX::TexMetadata metadata_{};

	ID3D12Resource* textureResource_{};

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_{};

	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_{};

	ID3D12Resource* depthStencilResource;

	ID3D12DescriptorHeap* dsvDescriptorHeap{};
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
};

