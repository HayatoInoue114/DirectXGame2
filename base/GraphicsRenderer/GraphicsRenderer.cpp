#include "GraphicsRenderer.h"

GraphicsRenderer* GraphicsRenderer::GetInstance() {
	static GraphicsRenderer instance;

	return &instance;
}


void GraphicsRenderer::Dxc() {
	HRESULT hr;
	//dxcCompilerを初期化
	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	//現時点でincludeはしないが、includeに対応する溜めの設定を行っておく
	includeHandler_ = nullptr;
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

IDxcBlob* GraphicsRenderer::CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するProfile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcComiler,
	IDxcIncludeHandler* includeHandler)
 {
	HRESULT hr;
	//これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読み込む
	shaderSource_ = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource_);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer = {};
	shaderSourceBuffer.Ptr = shaderSource_->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource_->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8の文字コードであることを通知

	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,//ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
		L"-Od",//最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};
	//実際にShaderをコンパイルする
	shaderResult_ = nullptr;
	hr = dxcComiler->Compile(
		&shaderSourceBuffer,			//読み込んだファイル
		arguments,						//コンパイルオプション
		_countof(arguments),			//コンパイルオプションの数
		includeHandler,					//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult_)		//コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
	//警告・エラーが出てたらログに出して止める
	shaderError_ = nullptr;
	shaderResult_->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError_), nullptr);
	if (shaderError_ != nullptr && shaderError_->GetStringLength() != 0) {
		Log(shaderError_->GetStringPointer());
		//警告・エラーダメ絶対
		assert(false);
	}
	//コンパイル結果から実行用のバイナリ部分を取得
	shaderBlob_ = nullptr;
	hr = shaderResult_->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob_), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource_->Release();
	shaderResult_->Release();
	//実行用のバイナリを返却
	return shaderBlob_;
}

void GraphicsRenderer::CreateRootSignature() {
	HRESULT hr;
	
	for (int i = 0; i < MAXPSO - 1; i++) {
		descriptorRange_[i][0].BaseShaderRegister = 0;
		descriptorRange_[i][0].NumDescriptors = 1;
		descriptorRange_[i][0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange_[i][0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	for (int i = 0; i < MAXPSO; i++) {
		//RootSignature作成
		descriptionRootSignature_[i] = {};
		descriptionRootSignature_[i].Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//DescriptorRange
		//descriptorRange_[0] = {};
		//descriptorRange_[0].BaseShaderRegister = 0; //0から始まる
		//descriptorRange_[0].NumDescriptors = 1;
		//descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		//descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算
		descriptorRangeForInstancing_[0] = {};
		descriptorRangeForInstancing_[0].BaseShaderRegister = 0; //0から始まる
		descriptorRangeForInstancing_[0].NumDescriptors = 1;
		descriptorRangeForInstancing_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRangeForInstancing_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

		//RootParameter作成。複数設定できるので配列。

		D3D12_ROOT_PARAMETER rootParameters[MAXPSO][5] = {};

		/*rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[1].Descriptor.ShaderRegister = 0;*/

		rootParameters[i][0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[i][0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[i][0].Descriptor.ShaderRegister = 0;

		if (i != 1) {
			rootParameters[i][1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[i][1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[i][1].Descriptor.ShaderRegister = 0;

			rootParameters[i][2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[i][2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[i][2].DescriptorTable.pDescriptorRanges = descriptorRange_[i];
			rootParameters[i][2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_[i]);

			rootParameters[i][4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[i][4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[i][4].Descriptor.ShaderRegister = 1;
		}

		rootParameters[i][3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[i][3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[i][3].Descriptor.ShaderRegister = 1;
		

		rootParameters[1][1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1][1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[1][1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing_;
		rootParameters[1][1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing_);
	
		rootParameters[1][2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1][2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[1][2].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing_;
		rootParameters[1][2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing_);
		
		rootParameters[1][4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[1][4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[1][4].Descriptor.ShaderRegister = 1;

		descriptionRootSignature_[i].pParameters = rootParameters[i];
		descriptionRootSignature_[i].NumParameters = _countof(rootParameters[i]);

		staticSamplers_[i][0] = {};
		staticSamplers_[i][0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
		staticSamplers_[i][0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
		staticSamplers_[i][0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers_[i][0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers_[i][0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
		staticSamplers_[i][0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMixmapを使う
		staticSamplers_[i][0].ShaderRegister = 0; //レジスタ番号0を使う
		staticSamplers_[i][0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う

		descriptionRootSignature_[i].pStaticSamplers = staticSamplers_[i];
		descriptionRootSignature_[i].NumStaticSamplers = _countof(staticSamplers_[i]);

		signatureBlob_[i] = nullptr;
		errorBlob_[i] = nullptr;
		hr = D3D12SerializeRootSignature(&descriptionRootSignature_[i],
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_[i], &errorBlob_[i]);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_[i]->GetBufferPointer()));
			assert(false);
		}

		rootSignature_[i] = nullptr;
		hr = DirectX12::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlob_[i]->GetBufferPointer(),
			signatureBlob_[i]->GetBufferSize(), IID_PPV_ARGS(rootSignature_[i].GetAddressOf()));
		assert(SUCCEEDED(hr));
	}

	
}

void GraphicsRenderer::InputLayout() {
	for (int i = 0; i < MAXPSO; i++) {
		inputElementDescs_[i][0] = {};
		inputElementDescs_[i][0].SemanticName = "POSITION";
		inputElementDescs_[i][0].SemanticIndex = 0;
		inputElementDescs_[i][0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs_[i][0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs_[i][1] = {};
		inputElementDescs_[i][1].SemanticName = "TEXCOORD";
		inputElementDescs_[i][1].SemanticIndex = 0;
		inputElementDescs_[i][1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs_[i][1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs_[i][2] = {};
		inputElementDescs_[i][2].SemanticName = "NORMAL";
		inputElementDescs_[i][2].SemanticIndex = 0;
		inputElementDescs_[i][2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs_[i][2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[i] = {};
		inputLayoutDesc_[i].pInputElementDescs = inputElementDescs_[i];
		inputLayoutDesc_[i].NumElements = _countof(inputElementDescs_[i]);
	}
	

	for (int i = 0; i < MAXPSO; i++) {
	
		
	}
	
}

void GraphicsRenderer::BlendState() {
	for (int i = 0; i < MAXPSO; i++) {
		blendDesc_[i] = {};
		//全ての色要素を書き込む
		blendDesc_[i].RenderTarget[0].RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_ALL;
		blendDesc_[i].RenderTarget[0].BlendEnable = true;
		blendDesc_[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_[i].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc_[i].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc_[i].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc_[i].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	}
}

void GraphicsRenderer::ResterizerState() {
	for (int i = 0; i < MAXPSO; i++) {
		rasterizerDesc_[i] = {};
		//裏面（時計回り）を表示しない
		rasterizerDesc_[i].CullMode = D3D12_CULL_MODE_BACK;
		//三角形の中を塗りつぶす
		rasterizerDesc_[i].FillMode = D3D12_FILL_MODE_SOLID;
	}
}

void GraphicsRenderer::BuildShader() {
	//Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"./ShaderFile/Object3d.VS.hlsl", L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompileShader(L"./ShaderFile/Object3d.PS.hlsl", L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);

	//Particle
	particleVertexShaderBlob_ = CompileShader(L"./ShaderFile/Particle.VS.hlsl", L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(particleVertexShaderBlob_ != nullptr);

	particlePixelShaderBlob_ = CompileShader(L"./ShaderFile/Particle.PS.hlsl", L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(particlePixelShaderBlob_ != nullptr);
}

void GraphicsRenderer::CreatePSO() {
	HRESULT hr;

	for (int i = 0; i < MAXPSO; i++) {
		PipelineManagerStateDesc_[i] = {};
		PipelineManagerStateDesc_[i].pRootSignature = rootSignature_[i].Get();//RootSignature
		PipelineManagerStateDesc_[i].InputLayout = inputLayoutDesc_[i];//InputLayout

		if (i == 0) {
			PipelineManagerStateDesc_[i].VS = { vertexShaderBlob_->GetBufferPointer(),
			vertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { pixelShaderBlob_->GetBufferPointer(),
			pixelShaderBlob_->GetBufferSize() };//PixelShader
		}
		if (i == 1) {
			PipelineManagerStateDesc_[i].VS = { particleVertexShaderBlob_->GetBufferPointer(),
			particleVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { particlePixelShaderBlob_->GetBufferPointer(),
			particlePixelShaderBlob_->GetBufferSize() };//PixelShader
		}
		
		
		PipelineManagerStateDesc_[i].BlendState = blendDesc_[i];//BlendState
		PipelineManagerStateDesc_[i].RasterizerState = rasterizerDesc_[i];//RasterizeState
		//書き込むRTVの情報
		PipelineManagerStateDesc_[i].NumRenderTargets = 1;
		PipelineManagerStateDesc_[i].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//利用するトポロジ（形状）のタイプ。三角形
		PipelineManagerStateDesc_[i].PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//どのように画面に色を打ち込むかの設定（気にしなくてよい）
		PipelineManagerStateDesc_[i].SampleDesc.Count = 1;
		PipelineManagerStateDesc_[i].SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//DepthStencilの設定
		PipelineManagerStateDesc_[i].DepthStencilState = depthStencilDesc_;
		PipelineManagerStateDesc_[i].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		PipelineManagerState_[i] = nullptr;
		hr = DirectX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&PipelineManagerStateDesc_[i],
			IID_PPV_ARGS(&PipelineManagerState_[i]));
		assert(SUCCEEDED(hr));
	}
	for (int i = 0; i < MAXPSO; i++) {
	
	}
	
}

void GraphicsRenderer::Viewport() {
	viewport_ = {};
	//クライアント領域サイズと一緒にして画面残帯に表示
	viewport_.Width = kCliantWidth;
	viewport_.Height = kCliantHeight;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

void GraphicsRenderer::ScissorRect() {
	scissorRect_ = {};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = kCliantWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = kCliantHeight;
}

void GraphicsRenderer::Initialize(DirectX12* directX12) {
	directX12_ = directX12;
	Dxc();
	CreateRootSignature();
	InputLayout();
	BlendState();
	ResterizerState();
	BuildShader();
	DepthStencilState();
	CreatePSO();
}

void GraphicsRenderer::Release() {
	for (int i = 0; i < MAXPSO; i++) {
		if (errorBlob_[i]) {
			errorBlob_[i]->Release();
		}
	}
	
	/*pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();*/
}

void GraphicsRenderer::DrawCall() {
	directX12_->GetCommandList()->RSSetViewports(1, &viewport_);	//Viewportを設定
	directX12_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);	//Scirssorを設定
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GraphicsRenderer::SetRootSignatureAndPSO(int n) {
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignature_[n].Get());
	directX12_->GetCommandList()->SetPipelineState(PipelineManagerState_[n].Get());	//PSOを設定
}

void GraphicsRenderer::DepthStencilState() {
	depthStencilDesc_ = {};
	//Depthの機能を有効化する
	depthStencilDesc_.DepthEnable = true;
	//書き込みをします
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

//#include "GraphicsRenderer.h"
//
//GraphicsRenderer* GraphicsRenderer::GetInstance() {
//	static GraphicsRenderer instance;
//
//	return &instance;
//}
//
//void GraphicsRenderer::Dxc() {
//	HRESULT hr;
//	//dxcCompilerを初期化
//	dxcUtils = nullptr;
//	dxcCompiler = nullptr;
//	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
//	assert(SUCCEEDED(hr));
//	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
//	assert(SUCCEEDED(hr));
//
//	//現時点でincludeはしないが、includeに対応する溜めの設定を行っておく
//	includeHandler = nullptr;
//	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
//	assert(SUCCEEDED(hr));
//}
//
//IDxcBlob* GraphicsRenderer::CompileShader(
//	//CompilerするShaderファイルへのパス
//	const std::wstring& filePath,
//	//Compilerに使用するProfile
//	const wchar_t* profile,
//	//初期化で生成したものを3つ
//	IDxcUtils* dxcUtils,
//	IDxcCompiler3* dxcComiler,
//	IDxcIncludeHandler* includeHandler)
//{
//	HRESULT hr;
//	//これからシェーダーをコンパイルする旨をログに出す
//	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
//	//hlslファイルを読み込む
//	shaderSource = nullptr;
//	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
//	//読めなかったら止める
//	assert(SUCCEEDED(hr));
//	//読み込んだファイルの内容を設定する
//	DxcBuffer shaderSourceBuffer = {};
//	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
//	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
//	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8の文字コードであることを通知
//
//	LPCWSTR arguments[] = {
//		filePath.c_str(),//コンパイル対象のhlslファイル名
//		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
//		L"-T",profile,//ShaderProfileの設定
//		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
//		L"-Od",//最適化を外しておく
//		L"-Zpr",//メモリレイアウトは行優先
//	};
//	//実際にShaderをコンパイルする
//	shaderResult = nullptr;
//	hr = dxcComiler->Compile(
//		&shaderSourceBuffer,			//読み込んだファイル
//		arguments,						//コンパイルオプション
//		_countof(arguments),			//コンパイルオプションの数
//		includeHandler,					//includeが含まれた諸々
//		IID_PPV_ARGS(&shaderResult)		//コンパイル結果
//	);
//	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
//	assert(SUCCEEDED(hr));
//	//警告・エラーが出てたらログに出して止める
//	shaderError = nullptr;
//	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
//	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
//		Log(shaderError->GetStringPointer());
//		//警告・エラーダメ絶対
//		assert(false);
//	}
//	//コンパイル結果から実行用のバイナリ部分を取得
//	shaderBlob = nullptr;
//	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
//	assert(SUCCEEDED(hr));
//	//成功したログを出す
//	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
//	//もう使わないリソースを解放
//	shaderSource->Release();
//	shaderResult->Release();
//	//実行用のバイナリを返却
//	return shaderBlob;
//}
//
//void GraphicsRenderer::CreateRootSignature(DirectX12* directX12) {
//	HRESULT hr;
//	//RootSignature作成
//	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature = {};
//	descriptionRootSignature.Flags =
//		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	//DescriptorRange
//	descriptorRange[0] = {};
//	descriptorRange[0].BaseShaderRegister = 0; //0から始まる
//	descriptorRange[0].NumDescriptors = 1;
//	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
//	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算
//
//	//RootParameter作成。複数設定できるので配列。
//	D3D12_ROOT_PARAMETER rootParameters[4] = {};
//
//	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	rootParameters[0].Descriptor.ShaderRegister = 0;
//
//	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
//	rootParameters[1].Descriptor.ShaderRegister = 0;
//
//	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //DescriptorTableを使う
//	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
//	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; //Tableの中身の配列を指定
//	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Tableで利用する数
//
//	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //CBVを使う
//	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
//	rootParameters[3].Descriptor.ShaderRegister = 1; //レジスタ番号1を使う
//
//	descriptionRootSignature.pParameters = rootParameters;
//	descriptionRootSignature.NumParameters = _countof(rootParameters);
//
//	staticSamplers[0] = {};
//	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
//	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
//	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
//	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMixmapを使う
//	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
//	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
//
//	descriptionRootSignature.pStaticSamplers = staticSamplers;
//	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
//
//	signatureBlob = nullptr;
//	errorBlob = nullptr;
//	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
//		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
//	if (FAILED(hr)) {
//		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
//		assert(false);
//	}
//	rootSignature = nullptr;
//	hr = directX12->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
//		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
//	assert(SUCCEEDED(hr));
//}
//
//void GraphicsRenderer::InputLayout() {
//	inputElementDescs[0] = {};
//	inputElementDescs[0].SemanticName = "POSITION";
//	inputElementDescs[0].SemanticIndex = 0;
//	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[1] = {};
//	inputElementDescs[1].SemanticName = "TEXCOORD";
//	inputElementDescs[1].SemanticIndex = 0;
//	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[2] = {};
//	inputElementDescs[2].SemanticName = "NORMAL";
//	inputElementDescs[2].SemanticIndex = 0;
//	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputLayoutDesc = {};
//	inputLayoutDesc.pInputElementDescs = inputElementDescs;
//	inputLayoutDesc.NumElements = _countof(inputElementDescs);
//}
//
//void GraphicsRenderer::BlendState() {
//	blendDesc = {};
//	//全ての色要素を書き込む
//	blendDesc.RenderTarget[0].RenderTargetWriteMask =
//		D3D12_COLOR_WRITE_ENABLE_ALL;
//}
//
//void GraphicsRenderer::ResterizerState() {
//	rasterizerDesc = {};
//	//裏面（時計回り）を表示しない
//	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
//	//三角形の中を塗りつぶす
//	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
//}
//
//void GraphicsRenderer::BuildShader() {
//	//Shaderをコンパイルする
//	vertexShaderBlob = CompileShader(L"./ShaderFile/Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
//	assert(vertexShaderBlob != nullptr);
//
//	pixelShaderBlob = CompileShader(L"./ShaderFile/Object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
//	assert(pixelShaderBlob != nullptr);
//}
//
//void GraphicsRenderer::CreatePSO(DirectX12* directX12) {
//	HRESULT hr;
//	graphicsPipelineStateDesc = {};
//	graphicsPipelineStateDesc.pRootSignature = rootSignature;//RootSignature
//	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;//InputLayout
//	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
//	vertexShaderBlob->GetBufferSize() };//VertexShader
//	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
//	pixelShaderBlob->GetBufferSize() };//PixelShader
//	graphicsPipelineStateDesc.BlendState = blendDesc;//BlendState
//	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizeState
//	//書き込むRTVの情報
//	graphicsPipelineStateDesc.NumRenderTargets = 1;
//	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	//利用するトポロジ（形状）のタイプ。三角形
//	graphicsPipelineStateDesc.PrimitiveTopologyType =
//		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//	//どのように画面に色を打ち込むかの設定（気にしなくてよい）
//	graphicsPipelineStateDesc.SampleDesc.Count = 1;
//	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
//
//	//DepthStencilの設定
//	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
//	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
//
//	graphicsPipelineState = nullptr;
//	hr = directX12->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
//		IID_PPV_ARGS(&graphicsPipelineState));
//	assert(SUCCEEDED(hr));
//}
//
//void GraphicsRenderer::Viewport() {
//	viewport = {};
//	//クライアント領域サイズと一緒にして画面残帯に表示
//	viewport.Width = kCliantWidth;
//	viewport.Height = kCliantHeight;
//	viewport.TopLeftX = 0;
//	viewport.TopLeftY = 0;
//	viewport.MinDepth = 0.0f;
//	viewport.MaxDepth = 1.0f;
//}
//
//void GraphicsRenderer::ScissorRect() {
//	scissorRect = {};
//	//基本的にビューポートと同じ矩形が構成されるようにする
//	scissorRect.left = 0;
//	scissorRect.right = kCliantWidth;
//	scissorRect.top = 0;
//	scissorRect.bottom = kCliantHeight;
//}
//
//void GraphicsRenderer::Initialize(DirectX12* directX12) {
//	Dxc();
//	CreateRootSignature(directX12);
//	InputLayout();
//	BlendState();
//	ResterizerState();
//	BuildShader();
//	DepthStencilState();
//	CreatePSO(directX12);
//}
//
//void GraphicsRenderer::Release() {
//	graphicsPipelineState->Release();
//	if (errorBlob) {
//		errorBlob->Release();
//	}
//	rootSignature->Release();
//	pixelShaderBlob->Release();
//	vertexShaderBlob->Release();
//}
//
//void GraphicsRenderer::DrawCall(DirectX12* directX12) {
//	directX12->GetCommandList()->RSSetViewports(1, &viewport);	//Viewportを設定
//	directX12->GetCommandList()->RSSetScissorRects(1, &scissorRect);	//Scirssorを設定
//	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
//	directX12->GetCommandList()->SetGraphicsRootSignature(rootSignature);
//	directX12->GetCommandList()->SetPipelineState(graphicsPipelineState);	//PSOを設定
//}
//
//void GraphicsRenderer::DepthStencilState() {
//	depthStencilDesc = {};
//	//Depthの機能を有効化する
//	depthStencilDesc.DepthEnable = true;
//	//書き込みをします
//	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	//比較関数はLessEqual。つまり、近ければ描画される
//	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
//}