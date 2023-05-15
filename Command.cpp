#include "Command.h"



Command::Command()
{
	commandAllocator = nullptr;
	commandList = nullptr;
}

Command::~Command()
{
}

void Command::CommandQueue() {
	//コマンドキューを生成する

	ID3D12CommandQueue* commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	directXCommand.hr = directXCommand.device->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue));
	//コマンドキューが生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(directXCommand.hr));
}

void Command::CommandAllocator() {
	directXCommand.hr = directXCommand.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	//コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(directXCommand.hr));
}

void Command::CommandList() {
	
	directXCommand.hr = directXCommand.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	//コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(directXCommand.hr));
}

void Command::Init() {
	CommandQueue();
	CommandAllocator();
	CommandList();
}