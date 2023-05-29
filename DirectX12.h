#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "FormatString.h"
#include <dxgidebug.h>

#include "WindowsAPI.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class WindowsAPI;

class DirectX12
{
public:
	DirectX12();
	~DirectX12();

	void DXGIFactory();

	void Adapter();

	void D3D12Device();

	void Command();

	void SwapChain(WindowsAPI* windowsAPI);

	void Descriptor();

	void Init(WindowsAPI* windowsAPI);
	
	/*void DebugLayer();*/

	void Error();

	void Barrier();

	void ScreenDisplay();

	void CommandConfirm();

	void Fence();

	void Update();

	void ResourceLeakCheck();

	void Release();

	ID3D12Device* GetDevice() { return device; }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList; }
public:
	void CommandList();

	void CommandKick();

	void Signal();

private:
	HRESULT hr;
	ID3D12Device* device;
	IDXGIFactory7* dxgiFactory;

	

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter;

	//コマンドキューを生成する
	ID3D12CommandQueue* commandQueue;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;

	//コマンドアロケータを生成する
	ID3D12CommandAllocator* commandAllocator;

	//コマンドリストを生成する
	ID3D12GraphicsCommandList* commandList;

	WindowsAPI* windowsAPI;

	//スワップチェーンを生成する
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//ディスクリプターヒープの生成
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle{};

	//RTVを２つ作るのでディスクリプタ２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];

	
	UINT backBufferIndex{};
	
	ID3D12Resource* swapChainResource[2];
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	
	//初期値0でFenceを作る
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue = 0;

	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent;

	IDXGIDebug1* debug;
};

