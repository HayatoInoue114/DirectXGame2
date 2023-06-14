#include "DirectX12.h"




void DirectX12::DXGIFactory() {
	dxgiFactory = nullptr;

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
}

void DirectX12::Adapter() {
	useAdapter = nullptr;
	//良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプター情報を取得する
		adapterDesc = {};
		HRESULT hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));//取得できないのは一大事
		//ソフトウェアアダプタでなければ採用!
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;//ソフトウェアアダプタの場合は見なかったことにする
	}

	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);
}

void DirectX12::D3D12Device() {
	device = nullptr;
	
	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順の生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		HRESULT hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
		//指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr)) {
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("featureLevel : {}\n",featureLevelStrings[i]));
			break;
		}
	}
	
	//デバイスの生成がうまくいかなかったので起動できない
	assert(device != nullptr);
	Log("Complete create D3D12Device!!!\n");//初期化完了のログを出す
}

void DirectX12::Command() {
	commandQueue = nullptr;
	commandQueueDesc = {};


	HRESULT hr = device->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue));
	//コマンドキューが生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	commandAllocator = nullptr;

	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	//コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	commandList = nullptr;

	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	//コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
}

void DirectX12::SwapChain() {
	swapChain = nullptr;
	swapChainDesc = {};

	swapChainDesc.Width = kCliantWidth;		//画面の横。ウインドウのクライアント領域を同じものにしておく
	swapChainDesc.Height = kCliantHeight;	//画面の高さ。ウインドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//色の形式
	swapChainDesc.SampleDesc.Count = 1;	//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;	//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//モニタにうつしたら、中身を破棄
	//コマンドキュー、ウインドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, windowsAPI_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(hr));
}

void DirectX12::DescriptorHeap() {
	rtvDescriptorHeap = nullptr;
	rtvDescriptorHeapDesc = {};
	
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;	//ダブルバッファように二つ、多くても別に構わない
	HRESULT hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプターヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));

	//SwapChainからResourceを引っ張ってくる
	swapChainResource[0] = { nullptr };
	swapChainResource[1] = { nullptr };
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResource[0]));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResource[1]));
	assert(SUCCEEDED(hr));

	rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//出力結果をSRGBを交換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	//２dテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	
	//まず１つ目を作る。１つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	rtvHandle[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResource[0], &rtvDesc, rtvHandle[0]);
	//２つ目のディスクリプタハンドルを得る（自力で)
	rtvHandle[1].ptr = rtvHandle[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//２つ目を作る
	device->CreateRenderTargetView(swapChainResource[1], &rtvDesc, rtvHandle[1]);
	

}

void DirectX12::GetBackBuffer() {
	//これから書き込むバックバッファのインデックスを取得
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
}

void DirectX12::RTV() {
	//描画先のRTVを設定する
	commandList->OMSetRenderTargets(1, &rtvHandle[backBufferIndex], false, nullptr);

	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };	//青っぽい色、RGBAの順
	commandList->ClearRenderTargetView(rtvHandle[backBufferIndex], clearColor, 0, nullptr);
}



void DirectX12::CommandKick() {
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);
	//GPUとSの画面の交換を行うよう通知する
	swapChain->Present(1, 0);	
}

void DirectX12::NextFlameCommandList() {
	//次のフレーム用のコマンドリストを準備
	HRESULT hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator, nullptr);
	assert(SUCCEEDED(hr));
}

//void DirectX12::DebugLayer() {
//	#ifdef _DEBUG
//	ID3D12Debug1* debugControllar = nullptr;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugControllar)))) {
//		//デバッグレイヤーを有効化する
//		debugControllar->EnableDebugLayer();
//		//さらにGPU側でもチェックを行うようにする
//		debugControllar->SetEnableGPUBasedValidation(TRUE);
//	}
//#endif // _DEBUG
//}

void DirectX12::Error() {
	#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		
		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//windows11でのDXGいデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			//https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		//解放
		infoQueue->Release();
	}
#endif // _DEBUG
}

void DirectX12::Barrier() {
	barrier = {};

	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = swapChainResource[backBufferIndex];
	//遷移前（現在）のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
}

void DirectX12::ScreenDisplay() {
	//画面に描く処理は全て終わり、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
}

void DirectX12::CommandConfirm() {
	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	HRESULT hr = commandList->Close();
	assert(SUCCEEDED(hr));
}

void DirectX12::Fence() {
	//初期値0でFenceを作る
	fence = nullptr;
	fenceValue = 0;
	
	HRESULT hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	/*hr = device->GetDeviceRemovedReason();*/
	assert(SUCCEEDED(hr));

	//FenceのSignalを待つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void DirectX12::Signal() {
	//Fenceの値を更新
	fenceValue++;
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue->Signal(fence, fenceValue);

	//Fenceの値が出したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue) {
		//指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void DirectX12::ResourceLeakCheck() {
	//リソースリークチェック
	
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}

void DirectX12::Release() {
	CloseHandle(fenceEvent);
		fence->Release();
		
		rtvDescriptorHeap->Release();
		swapChainResource[0]->Release();
		swapChainResource[1]->Release();
		swapChain->Release();
		commandList->Release();
		commandAllocator->Release();
		commandQueue->Release();
		device->Release();
		useAdapter->Release();
		dxgiFactory->Release();
		CloseWindow(windowsAPI_->GetHwnd());
}

void DirectX12::Init(WindowsAPI* windowsAPI) {
	windowsAPI_ = windowsAPI;
	windowsAPI->Init();
	DXGIFactory();
	Adapter();
	D3D12Device();

	Error();
	Command();
	SwapChain();
	DescriptorHeap();
	Fence();
}

void DirectX12::Update() {
	//ゲームの処理
	
	Signal();
	CommandKick();


	ResourceLeakCheck();
}

void DirectX12::PreDraw() {
	GetBackBuffer();
	Barrier();
	RTV();
}

void DirectX12::PostDraw() {
	ScreenDisplay();
	CommandConfirm();
	CommandKick();
	Signal();
	NextFlameCommandList();
}