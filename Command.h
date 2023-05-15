#pragma once
#include "DirectX12.h"

DirectX12 directXCommand;

class Command
{
public:
	Command();
	~Command();

	void CommandQueue();

	void CommandAllocator();

	void CommandList();

	void Init();

private:
	//コマンドアロケータを生成する
	ID3D12CommandAllocator* commandAllocator;
	
	//コマンドリストを生成する
	ID3D12GraphicsCommandList* commandList;
};


