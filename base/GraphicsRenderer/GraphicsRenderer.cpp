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

	for (int i = 0; i < MAXPSO; i++) {
		//RootSignature作成
		descriptionRootSignature_[i] = {};
		descriptionRootSignature_[i].Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//DescriptorRange
		descriptorRange_[0] = {};
		descriptorRange_[0].BaseShaderRegister = 0; //0から始まる
		descriptorRange_[0].NumDescriptors = 1;
		descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算
		descriptorRangeForInstancing_[0] = {};
		descriptorRangeForInstancing_[0].BaseShaderRegister = 0; //0から始まる
		descriptorRangeForInstancing_[0].NumDescriptors = 1;
		descriptorRangeForInstancing_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRangeForInstancing_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

		//RootParameter作成。複数設定できるので配列。

		rootParameters_[i][0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters_[i][0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters_[i][0].Descriptor.ShaderRegister = 0;

		if (i != 1) {
			rootParameters_[i][1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters_[i][1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters_[i][1].Descriptor.ShaderRegister = 0;

			rootParameters_[i][2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters_[i][2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters_[i][2].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters_[i][2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			rootParameters_[i][4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters_[i][4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters_[i][4].Descriptor.ShaderRegister = 1;
		}

		rootParameters_[i][3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters_[i][3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters_[i][3].Descriptor.ShaderRegister = 1;
		

		rootParameters_[1][1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters_[1][1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters_[1][1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing_;
		rootParameters_[1][1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing_);
	
		rootParameters_[1][2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters_[1][2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters_[1][2].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing_;
		rootParameters_[1][2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing_);
		
		rootParameters_[1][4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters_[1][4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters_[1][4].Descriptor.ShaderRegister = 1;

		descriptionRootSignature_[i].pParameters = rootParameters_[i];
		descriptionRootSignature_[i].NumParameters = _countof(rootParameters_[i]);

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
	inputElementDescs_[0] = {};
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs_[1] = {};
	inputElementDescs_[1].SemanticName = "TEXCOORD";
	inputElementDescs_[1].SemanticIndex = 0;
	inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs_[2] = {};
	inputElementDescs_[2].SemanticName = "NORMAL";
	inputElementDescs_[2].SemanticIndex = 0;
	inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (int i = 0; i < MAXPSO; i++) {
	
		inputLayoutDesc_[i] = {};
		inputLayoutDesc_[i].pInputElementDescs = inputElementDescs_;
		inputLayoutDesc_[i].NumElements = _countof(inputElementDescs_);
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
	for (int i = 0; i < MAXPSO; i++) {
		directX12_->GetCommandList()->RSSetViewports(1, &viewport_);	//Viewportを設定
		directX12_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);	//Scirssorを設定
		//RootSignatureを設定。PSOに設定しているけど別途設定が必要
		directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignature_[i].Get());
		directX12_->GetCommandList()->SetPipelineState(PipelineManagerState_[i].Get());	//PSOを設定
	}
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