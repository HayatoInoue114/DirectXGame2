#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <string.h>
#include <format>


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectX12
{
public:
	DirectX12();
	~DirectX12();

	void DXGIFactory();

	void Adapter();

	void D3D12Device();

	void Init();
private:
	IDXGIFactory7* dxgiFactory;

	HRESULT hr;

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter;
};

