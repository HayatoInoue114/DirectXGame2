#include "GraphicsRenderer.h"
#include <string>
#include <format>
#include "../../manager/SrvManager/SrvManager.h"
#include "../../manager/TextureManager/TextureManager.h"

GraphicsRenderer* GraphicsRenderer::GetInstance() {
	static GraphicsRenderer instance;

	return &instance;
}


void GraphicsRenderer::Dxc() {
	HRESULT hr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));
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
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler)
 {
	HRESULT hr;
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource_);
	assert(SUCCEEDED(hr));

	DxcBuffer shaderSourceBuffer = {};
	shaderSourceBuffer.Ptr = shaderSource_->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource_->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E", L"main",
		L"-T", profile,
		L"-Zi", L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult_)
	);
	assert(SUCCEEDED(hr));

	shaderResult_->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError_), nullptr);
	if (shaderError_ != nullptr && shaderError_->GetStringLength() != 0) {
		Log(shaderError_->GetStringPointer());
		assert(false);
	}

	hr = shaderResult_->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob_), nullptr);
	assert(SUCCEEDED(hr));

	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));

	shaderSource_->Release();
	shaderResult_->Release();

	return shaderBlob_;
}

void GraphicsRenderer::CreateRootSignature() {
	HRESULT hr;


	for (int i = 0; i < MAXPSO; i++) {

		descriptorRange_[0] = {};
		descriptorRange_[0].BaseShaderRegister = 0; // 0から始まる
		descriptorRange_[0].NumDescriptors = 1;
		descriptorRange_[0].RegisterSpace = 0;
		descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		descriptorRangeForInstancing_[0] = {};
		descriptorRangeForInstancing_[0].BaseShaderRegister = 1;
		descriptorRangeForInstancing_[0].NumDescriptors = 1;
		descriptorRangeForInstancing_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRangeForInstancing_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		// RootSignature作成
		descriptionRootSignature_[i] = {};
		descriptionRootSignature_[i].Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成。複数設定できるので配列。
		D3D12_ROOT_PARAMETER rootParameters[7] = {};

		//*  共通  *//
		switch (i)
		{
		case Object3d:
			// Material
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].Descriptor.ShaderRegister = 0;
			// wvp
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[1].Descriptor.ShaderRegister = 0;
			// texture
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);
			// Light
			rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[3].Descriptor.ShaderRegister = 1;
			//camera
			rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[4].Descriptor.ShaderRegister = 2;
			// textureCube
			rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[5].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing_;
			rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 6;
			break;
		
		case Particle:
			// Material
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].Descriptor.ShaderRegister = 0;
			// srv
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);
			// texture
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);
			// Light
			rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[3].Descriptor.ShaderRegister = 1;

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 4;
			break;

		case Skinning:
			// Material
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].Descriptor.ShaderRegister = 0;
			// wvp
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[1].Descriptor.ShaderRegister = 0;
			// texture
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);
			// Light
			rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[3].Descriptor.ShaderRegister = 1;
			//camera
			rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[4].Descriptor.ShaderRegister = 2;
			// textureCube
			rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[5].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing_;
			rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing_);
			//Palette
			rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[6].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 7;
			break;

#pragma region PostEffect
		case CopyImage:
			// srv
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 1;
			break;
		case Grayscale:
			// srv
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 1;
			break;
		case Vignette:
			// srv
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 1;
			break;
		case BoxFilter:
			// srv
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 1;
			break;
		case LuminanceBasedOutline:
			// srv
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 1;
			break;
		//case DepthBasedOutline:
		//	// Material
		//	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		//	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//	rootParameters[0].Descriptor.ShaderRegister = 0;
		//	// srv
		//	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange_;
		//	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

		//	// rootSignatureに設定
		//	descriptionRootSignature_[i].pParameters = rootParameters;
		//	descriptionRootSignature_[i].NumParameters = 1;
		//	break;

#pragma endregion PostEffect
		case Skybox:
			// Material
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].Descriptor.ShaderRegister = 0;
			// wvp
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[1].Descriptor.ShaderRegister = 0;
			// texture
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);
			//camera

			// rootSignatureに設定
			descriptionRootSignature_[i].pParameters = rootParameters;
			descriptionRootSignature_[i].NumParameters = 3;
			break;
		}

		staticSamplers_[i][0] = {};
		staticSamplers_[i][0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
		staticSamplers_[i][0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
		staticSamplers_[i][0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers_[i][0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers_[i][0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
		staticSamplers_[i][0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipMapを使う
		staticSamplers_[i][0].ShaderRegister = 0; // レジスタ番号0を使う
		staticSamplers_[i][0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

		descriptionRootSignature_[i].pStaticSamplers = staticSamplers_[i];
		descriptionRootSignature_[i].NumStaticSamplers = _countof(staticSamplers_[i]);

		signatureBlob_[i] = nullptr;
		errorBlob_[i] = nullptr;
		hr = D3D12SerializeRootSignature(&descriptionRootSignature_[i],
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_[i], &errorBlob_[i]);
		if (FAILED(hr)) {
			if (errorBlob_[i]) {
				Log(reinterpret_cast<char*>(errorBlob_[i]->GetBufferPointer()));
			}
			else {
				Log("Unknown error occurred during root signature serialization.");
			}
			assert(false);
		}

		rootSignature_[i] = nullptr;
		hr = DirectX12::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlob_[i]->GetBufferPointer(),
			signatureBlob_[i]->GetBufferSize(), IID_PPV_ARGS(rootSignature_[i].GetAddressOf()));
		if (FAILED(hr)) {
			Log("Failed to create root signature.");
			assert(false);
		}
		std::wstring rootname = std::format(L"RootParam{}", i);
		rootSignature_[i]->SetName(rootname.c_str());
	}
}

void GraphicsRenderer::InputLayout() {
	for (int i = 0; i < MAXPSO; i++) {
		if (i != Skybox) {
			inputElementDescs_[i][0].SemanticName = "POSITION";
			inputElementDescs_[i][0].SemanticIndex = 0;
			inputElementDescs_[i][0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElementDescs_[i][0].InputSlot = 0;
			inputElementDescs_[i][0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][0].InstanceDataStepRate = 0;

			inputElementDescs_[i][1].SemanticName = "TEXCOORD";
			inputElementDescs_[i][1].SemanticIndex = 0;
			inputElementDescs_[i][1].Format = DXGI_FORMAT_R32G32_FLOAT;
			inputElementDescs_[i][1].InputSlot = 0;
			inputElementDescs_[i][1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][1].InstanceDataStepRate = 0;

			inputElementDescs_[i][2].SemanticName = "NORMAL";
			inputElementDescs_[i][2].SemanticIndex = 0;
			inputElementDescs_[i][2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElementDescs_[i][2].InputSlot = 0;
			inputElementDescs_[i][2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][2].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][2].InstanceDataStepRate = 0;
		}
		else {
			inputElementDescs_[i][0].SemanticName = "POSITION";
			inputElementDescs_[i][0].SemanticIndex = 0;
			inputElementDescs_[i][0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElementDescs_[i][0].InputSlot = 0;
			inputElementDescs_[i][0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][0].InstanceDataStepRate = 0;

			inputElementDescs_[i][1].SemanticName = "TEXCOORD";
			inputElementDescs_[i][1].SemanticIndex = 0;
			inputElementDescs_[i][1].Format = DXGI_FORMAT_R32G32_FLOAT;
			inputElementDescs_[i][1].InputSlot = 0;
			inputElementDescs_[i][1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][1].InstanceDataStepRate = 0;
		}
		

		//skinnning
		if (i == Skinning) {
			inputElementDescs_[i][3].SemanticName = "WEIGHT";
			inputElementDescs_[i][3].SemanticIndex = 0;
			inputElementDescs_[i][3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElementDescs_[i][3].InputSlot = 1;
			inputElementDescs_[i][3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][3].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][3].InstanceDataStepRate = 0;

			inputElementDescs_[i][4].SemanticName = "INDEX";
			inputElementDescs_[i][4].SemanticIndex = 0;
			inputElementDescs_[i][4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
			inputElementDescs_[i][4].InputSlot = 1;
			inputElementDescs_[i][4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs_[i][4].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElementDescs_[i][4].InstanceDataStepRate = 0;
		}
	}
	

	// Set input layout descriptions
	for (int i = 0; i < MAXPSO; i++) {
		// 各PSOの要素数を決定
		UINT numElements = (i == Skinning) ? 5 : 3;  // skinningには5個の要素数
		if (i == Skybox) {
			numElements = 2;
		}
		
		switch (i)
		{
		case CopyImage:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = nullptr;
			inputLayoutDesc_[i].NumElements = 0;
			break;
		case Grayscale:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = nullptr;
			inputLayoutDesc_[i].NumElements = 0;
			break;
		case Vignette:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = nullptr;
			inputLayoutDesc_[i].NumElements = 0;
			break;
		case BoxFilter:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = nullptr;
			inputLayoutDesc_[i].NumElements = 0;
				break;
		case LuminanceBasedOutline:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = nullptr;
			inputLayoutDesc_[i].NumElements = 0;
			break;
		/*case DepthBasedOutline:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = nullptr;
			inputLayoutDesc_[i].NumElements = 0;
			break;*/
		case Skybox:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = inputElementDescs_[i];
			inputLayoutDesc_[i].NumElements = numElements;
			break;
		//postEffect以外（object3d等）
		default:
			inputLayoutDesc_[i] = {};
			inputLayoutDesc_[i].pInputElementDescs = inputElementDescs_[i];
			inputLayoutDesc_[i].NumElements = numElements;
			break;
		}
	}
}

void GraphicsRenderer::BlendState() {
	for (int i = 0; i < MAXPSO; i++) {
		// すべての色要素を書き込む
		blendDesc_[i].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blendDesc_[i].RenderTarget[0].BlendEnable = true;
		blendDesc_[i].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc_[i].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc_[i].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

		// ノーマルブレンドの設定（デフォルト）
		blendDesc_[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_[i].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}

	// ブレンドモードの設定

	//// ノーマル
	//blendDesc_[6].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blendDesc_[6].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//blendDesc_[6].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//// 加算
	//blendDesc_[2].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blendDesc_[2].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//blendDesc_[2].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//// 減算
	//blendDesc_[3].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blendDesc_[3].RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blendDesc_[3].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//// 乗算
	//blendDesc_[4].RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	//blendDesc_[4].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//blendDesc_[4].RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;

	//// スクリーン
	//blendDesc_[5].RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	//blendDesc_[5].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//blendDesc_[5].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	// particle
	blendDesc_[1].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc_[1].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc_[1].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
}

void GraphicsRenderer::ResterizerState() {
	rasterizerDesc_ = {};
	//裏面（時計回り）を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void GraphicsRenderer::BuildShader() {
	//Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"./ShaderFile/Object3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShaderBlob_ != nullptr);
	pixelShaderBlob_ = CompileShader(L"./ShaderFile/Object3d.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShaderBlob_ != nullptr);

	//Particle
	particleVertexShaderBlob_ = CompileShader(L"./ShaderFile/Particle.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(particleVertexShaderBlob_ != nullptr);
	particlePixelShaderBlob_ = CompileShader(L"./ShaderFile/Particle.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(particlePixelShaderBlob_ != nullptr);

	//Skinning
	skinningVertexShaderBlob_ = CompileShader(L"./ShaderFile/SkinningObject3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(skinningVertexShaderBlob_ != nullptr);

	//CopyImage(今は複数のポストエフェクトを使えないのでここのPSを書き換えてるだけ)
	copyImageVertexShaderBlob_ = CompileShader(L"./ShaderFile/CopyImage.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(copyImageVertexShaderBlob_ != nullptr);
	copyImagePixelShaderBlob_ = CompileShader(L"./ShaderFile/CopyImage.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(copyImagePixelShaderBlob_ != nullptr);

	//Grayscale
	grayscalePixelShaderBlob_ = CompileShader(L"./ShaderFile/Grayscale.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(grayscalePixelShaderBlob_ != nullptr);

	//Vignette
	vignettePixelShaderBlob_ = CompileShader(L"./ShaderFile/Vignette.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vignettePixelShaderBlob_ != nullptr);

	//BoxFilter
	boxFilterPixelShaderBlob_ = CompileShader(L"./ShaderFile/BoxFilter.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(boxFilterPixelShaderBlob_ != nullptr);

	//LuminanceBasedOutline
	luminanceBasedOutlinePSBlob_ = CompileShader(L"./ShaderFile/LuminanceBasedOutline.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(luminanceBasedOutlinePSBlob_ != nullptr);

	//DepthBasedOutline
	depthBasedOutlinePSBlob_ = CompileShader(L"./ShaderFile/depthBasedOutline.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(depthBasedOutlinePSBlob_ != nullptr);

	//Shaderをコンパイルする
	SkyboxVSBlob_ = CompileShader(L"./ShaderFile/Skybox.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(SkyboxVSBlob_ != nullptr);
	SkyboxPSBlob_ = CompileShader(L"./ShaderFile/Skybox.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(SkyboxPSBlob_ != nullptr);
}

void GraphicsRenderer::CreatePSO() {
	HRESULT hr;

	for (int i = 0; i < MAXPSO; i++) {
		PipelineManagerStateDesc_[i] = {};
		PipelineManagerStateDesc_[i].pRootSignature = rootSignature_[i].Get();//RootSignature
		PipelineManagerStateDesc_[i].InputLayout = inputLayoutDesc_[i];//InputLayout

		switch (i)
		{
		case Object3d:
			PipelineManagerStateDesc_[i].VS = { vertexShaderBlob_->GetBufferPointer(),
			vertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { pixelShaderBlob_->GetBufferPointer(),
			pixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case Particle:
			PipelineManagerStateDesc_[i].VS = { particleVertexShaderBlob_->GetBufferPointer(),
			particleVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { particlePixelShaderBlob_->GetBufferPointer(),
			particlePixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case Skinning:
			PipelineManagerStateDesc_[i].VS = { skinningVertexShaderBlob_->GetBufferPointer(),
			skinningVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { pixelShaderBlob_->GetBufferPointer(),
			pixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case CopyImage:
			PipelineManagerStateDesc_[i].VS = { copyImageVertexShaderBlob_->GetBufferPointer(),
			copyImageVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { copyImagePixelShaderBlob_->GetBufferPointer(),
			copyImagePixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case Grayscale:
			PipelineManagerStateDesc_[i].VS = { copyImageVertexShaderBlob_->GetBufferPointer(),
			copyImageVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { grayscalePixelShaderBlob_->GetBufferPointer(),
			grayscalePixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case Vignette:
			PipelineManagerStateDesc_[i].VS = { copyImageVertexShaderBlob_->GetBufferPointer(),
			copyImageVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { vignettePixelShaderBlob_->GetBufferPointer(),
			vignettePixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case BoxFilter:
			PipelineManagerStateDesc_[i].VS = { copyImageVertexShaderBlob_->GetBufferPointer(),
			copyImageVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { boxFilterPixelShaderBlob_->GetBufferPointer(),
			boxFilterPixelShaderBlob_->GetBufferSize() };//PixelShader
			break;
		case LuminanceBasedOutline:
			PipelineManagerStateDesc_[i].VS = { copyImageVertexShaderBlob_->GetBufferPointer(),
			copyImageVertexShaderBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { luminanceBasedOutlinePSBlob_->GetBufferPointer(),
			luminanceBasedOutlinePSBlob_->GetBufferSize() };//PixelShader
			break;
		//case DepthBasedOutline:
		//	PipelineManagerStateDesc_[i].VS = { copyImageVertexShaderBlob_->GetBufferPointer(),
		//	copyImageVertexShaderBlob_->GetBufferSize() };//VertexShader
		//	PipelineManagerStateDesc_[i].PS = { depthBasedOutlinePSBlob_->GetBufferPointer(),
		//	depthBasedOutlinePSBlob_->GetBufferSize() };//PixelShader
		//	break;
		case Skybox:
			PipelineManagerStateDesc_[i].VS = { SkyboxVSBlob_->GetBufferPointer(),
			SkyboxVSBlob_->GetBufferSize() };//VertexShader
			PipelineManagerStateDesc_[i].PS = { SkyboxPSBlob_->GetBufferPointer(),
			SkyboxPSBlob_->GetBufferSize() };//PixelShader
			break;
		}
		
		PipelineManagerStateDesc_[i].BlendState = blendDesc_[i];//BlendState
		PipelineManagerStateDesc_[i].RasterizerState = rasterizerDesc_;//RasterizeState
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
		if (i == CopyImage || i == Grayscale || i == Vignette || i == BoxFilter || i == LuminanceBasedOutline/* || i == DepthBasedOutline*/) {
			PipelineManagerStateDesc_[i].DepthStencilState.DepthEnable = false;
		}
		PipelineManagerStateDesc_[i].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		PipelineManagerState_[i] = nullptr;
		hr = DirectX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&PipelineManagerStateDesc_[i],
			IID_PPV_ARGS(&PipelineManagerState_[i]));
		assert(SUCCEEDED(hr));
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

void GraphicsRenderer::Initialize() {
	Dxc();
	CreateRootSignature();
	InputLayout();
	BlendState();
	ResterizerState();
	BuildShader();
	DepthStencilState();
	CreatePSO();
}

void GraphicsRenderer::RSSet() {
	DirectX12::GetInstance()->GetCommandList()->RSSetViewports(1, &viewport_);	//Viewportを設定
	DirectX12::GetInstance()->GetCommandList()->RSSetScissorRects(1, &scissorRect_);	//Scirssorを設定
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GraphicsRenderer::DrawCall()
{
	//ImGui::Begin("PostEffect");
	//
	//ImGui::Checkbox("GrayScale", &isGrayScale);
	//ImGui::Checkbox("Vignette", &isVignette);
	//ImGui::Checkbox("BoxFilte", &isBoxFilte);
	////ImGui::Checkbox("isLuminanceBasedOutline", &isLuminanceBasedOutline);

	//ImGui::End();

	//基本の色で描画（ポストエフェクト無し）
	SetRootSignatureAndPSO(CopyImage);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[CopyImage].Get());
	DirectX12::GetInstance()->GetCommandList()->SetPipelineState(PipelineManagerState_[CopyImage].Get());
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvIndex()));

	if (isGrayScale) 
	{
		SetRootSignatureAndPSO(Grayscale);
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[Grayscale].Get());
		DirectX12::GetInstance()->GetCommandList()->SetPipelineState(PipelineManagerState_[Grayscale].Get());
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvIndex()));
	}
	if (isVignette) 
	{
		SetRootSignatureAndPSO(Vignette);
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[Vignette].Get());
		DirectX12::GetInstance()->GetCommandList()->SetPipelineState(PipelineManagerState_[Vignette].Get());
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvIndex()));
	}
	if (isBoxFilte) 
	{
		SetRootSignatureAndPSO(BoxFilter);
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[BoxFilter].Get());
		DirectX12::GetInstance()->GetCommandList()->SetPipelineState(PipelineManagerState_[BoxFilter].Get());
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvIndex()));
	}
	else if (isLuminanceBasedOutline) 
	{
		SetRootSignatureAndPSO(LuminanceBasedOutline);
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[LuminanceBasedOutline].Get());
		DirectX12::GetInstance()->GetCommandList()->SetPipelineState(PipelineManagerState_[LuminanceBasedOutline].Get());
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvIndex()));
	}
	//頂点3つ描画
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void GraphicsRenderer::SetRootSignatureAndPSO(int n) {
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	wchar_t* name = new wchar_t[512];
	//name.resize(20);
	UINT s = 512;
	rootSignature_[n]->GetPrivateData(WKPDID_D3DDebugObjectNameW, &s, (void*)name);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[n].Get());
	DirectX12::GetInstance()->GetCommandList()->SetPipelineState(PipelineManagerState_[n].Get());	//PSOを設定
}

void GraphicsRenderer::DepthStencilState() {
	depthStencilDesc_ = {};
	//Depthの機能を有効化する
	depthStencilDesc_.DepthEnable = true;
	//書き込みをします
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

