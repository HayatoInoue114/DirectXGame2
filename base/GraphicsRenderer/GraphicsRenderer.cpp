#include "GraphicsRenderer.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <stdexcept>
#include <vector>

// Singletonインスタンスの取得
GraphicsRenderer* GraphicsRenderer::GetInstance() {
    static GraphicsRenderer instance;
    return &instance;
}

// 初期化
void GraphicsRenderer::Initialize() {
    InitializeDxc();
    CreateRootSignatures();
    DefineInputLayout();
    ConfigureBlendState();
    ConfigureRasterizerState();
    BuildShaders();
    CreatePipelineStateObjects();
    ConfigureDepthStencilState();
}

// Dxcの初期化
void GraphicsRenderer::InitializeDxc() {
    HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DXC Utils");
    }

    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DXC Compiler");
    }

    hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create default include handler");
    }
}

// シェーダーのコンパイル
IDxcBlob* GraphicsRenderer::CompileShader(
    const std::wstring& filePath,
    const wchar_t* profile,
    IDxcUtils* dxcUtils,
    IDxcCompiler3* dxcCompiler,
    IDxcIncludeHandler* includeHandler
) {
    // シェーダーファイルの読み込み
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource;
    HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to load shader file");
    }

    DxcBuffer sourceBuffer = { shaderSource->GetBufferPointer(), shaderSource->GetBufferSize(), DXC_CP_UTF8 };

    // コンパイル
    Microsoft::WRL::ComPtr<IDxcResult> shaderResult;
    hr = dxcCompiler->Compile(&sourceBuffer, nullptr, 0, nullptr, IID_PPV_ARGS(&shaderResult));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to compile shader");
    }

    shaderResult->GetStatus(&hr);
    if (FAILED(hr)) {
        throw std::runtime_error("Shader compilation failed");
    }

    Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob;
    hr = shaderResult->GetResult(&shaderBlob);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get shader blob");
    }

    return shaderBlob.Detach();
}

void InitializeShaderConfig(ShaderConfig& config, bool isDefault) {
    config.descriptorRanges[0] = { D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };

    D3D12_ROOT_PARAMETER param = {};
    param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    param.Descriptor.ShaderRegister = 0;
    config.rootParameters[0] = param;

    param.Descriptor.ShaderRegister = 1;
    config.rootParameters[1] = param;

    if (isDefault) {
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        param.Descriptor.ShaderRegister = 0;
        config.rootParameters[2] = param;

        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        param.DescriptorTable = { 1, config.descriptorRanges.data() };
        config.rootParameters[3] = param;

        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        param.Descriptor.ShaderRegister = 1;
        config.rootParameters[4] = param;
    }
    else {
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        param.DescriptorTable = { 1, config.descriptorRanges.data() };
        config.rootParameters[2] = param;

        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        param.DescriptorTable = { 1, config.descriptorRanges.data() };
        config.rootParameters[3] = param;

        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        param.Descriptor.ShaderRegister = 1;
        config.rootParameters[4] = param;
    }

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = static_cast<UINT>(config.rootParameters.size());
    rootSignatureDesc.pParameters = config.rootParameters.data();
    rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(config.staticSamplers.size());
    rootSignatureDesc.pStaticSamplers = config.staticSamplers.data();
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    D3D12_RENDER_TARGET_BLEND_DESC defaultBlendDesc = {};
    defaultBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    config.blendDesc.RenderTarget[0] = defaultBlendDesc;
    if (!isDefault) {
        config.blendDesc.RenderTarget[0].BlendEnable = true;
        config.blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        config.blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        config.blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        config.blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
        config.blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
        config.blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    }

    D3D12_RASTERIZER_DESC defaultRasterizerDesc = {};
    defaultRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    defaultRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    config.rasterizerDesc = defaultRasterizerDesc;

    config.inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    config.inputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    config.inputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
}

void GraphicsRenderer::CreateRootSignatures() {
    HRESULT hr;

    std::array<ShaderConfig, MAXPSO> shaderConfigs;
    for (int i = 0; i < MAXPSO; ++i) {
        bool isDefault = (i == 0);
        InitializeShaderConfig(shaderConfigs[i], isDefault);

        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.NumParameters = static_cast<UINT>(shaderConfigs[i].rootParameters.size());
        rootSignatureDesc.pParameters = shaderConfigs[i].rootParameters.data();
        rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(shaderConfigs[i].staticSamplers.size());
        rootSignatureDesc.pStaticSamplers = shaderConfigs[i].staticSamplers.data();
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlobs_[i], &errorBlobs_[i]);
        if (FAILED(hr)) {
            Log(reinterpret_cast<char*>(errorBlobs_[i]->GetBufferPointer()));
            assert(false);
        }

        hr = DirectX12::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlobs_[i]->GetBufferPointer(), signatureBlobs_[i]->GetBufferSize(), IID_PPV_ARGS(&rootSignatures_[i]));
        assert(SUCCEEDED(hr));
    }
}

// 入力レイアウトの定義
void GraphicsRenderer::DefineInputLayout() {
    for (int i = 0; i < MAXPSO; ++i) {
        inputElementDescs_[i] = {
            D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            D3D12_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        inputLayoutDescs_[i] = { inputElementDescs_[i].data(), static_cast<UINT>(inputElementDescs_[i].size()) };
    }
}

// 入力レイアウトの定義
void GraphicsRenderer::DefineInputLayout() {
    for (int i = 0; i < MAXPSO; ++i) {
        inputElementDescs_[i] = {
            D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            D3D12_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        inputLayoutDescs_[i] = { inputElementDescs_[i].data(), static_cast<UINT>(inputElementDescs_[i].size()) };
    }
}

void GraphicsRenderer::ConfigureBlendState() {
    // デフォルトのブレンドステートを定義
    D3D12_RENDER_TARGET_BLEND_DESC defaultBlendDesc = {};
    defaultBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // デフォルト設定を全ブレンドディスクリプタに適用
    for (auto& blendDesc : blendDescs_) {
        blendDesc.RenderTarget[0] = defaultBlendDesc;
    }

    // 特定のPSOに対するブレンド設定の調整
    for (int i = 1; i < MAXPSO; ++i) {
        blendDescs_[i].RenderTarget[0].BlendEnable = true;
        blendDescs_[i].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        blendDescs_[i].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        blendDescs_[i].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

        if (i == 1) {
            blendDescs_[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
            blendDescs_[i].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
            blendDescs_[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        }
    }
}


void GraphicsRenderer::CreateRasterizerStates() {
    D3D12_RASTERIZER_DESC defaultRasterizerDesc = {};
    defaultRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    defaultRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    for (int i = 0; i < MAXPSO; ++i) {
        rasterizerDesc_[i] = defaultRasterizerDesc;
    }
}

void GraphicsRenderer::CompileShaders() {
    vertexShaderBlob_ = CompileShader(L"./ShaderFile/Object3d.VS.hlsl", L"vs_6_0");
    pixelShaderBlob_ = CompileShader(L"./ShaderFile/Object3d.PS.hlsl", L"ps_6_0");
    particleVertexShaderBlob_ = CompileShader(L"./ShaderFile/Particle.VS.hlsl", L"vs_6_0");
    particlePixelShaderBlob_ = CompileShader(L"./ShaderFile/Particle.PS.hlsl", L"ps_6_0");
}

void GraphicsRenderer::CreatePSOs() {
    HRESULT hr;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() };
    psoDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.RasterizerState = rasterizerDesc_[0];
    psoDesc.BlendState = blendDesc_[0];
    psoDesc.DepthStencilState.DepthEnable = true;
    psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    psoDesc.DepthStencilState.StencilEnable = false;
    psoDesc.InputLayout = inputLayoutDesc_[0];
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.pRootSignature = rootSignature_[0];

    hr = DirectX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_[0]));
    assert(SUCCEEDED(hr));

    psoDesc.VS = { particleVertexShaderBlob_->GetBufferPointer(), particleVertexShaderBlob_->GetBufferSize() };
    psoDesc.PS = { particlePixelShaderBlob_->GetBufferPointer(), particlePixelShaderBlob_->GetBufferSize() };
    psoDesc.RasterizerState = rasterizerDesc_[1];
    psoDesc.BlendState = blendDesc_[1];
    psoDesc.InputLayout = inputLayoutDesc_[1];
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
    psoDesc.pRootSignature = rootSignature_[1];

    hr = DirectX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_[1]));
    assert(SUCCEEDED(hr));
}

void GraphicsRenderer::CreateDepthStencilState() {
    depthStencilDesc_.DepthEnable = true;
    depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthStencilDesc_.StencilEnable = false;
}

void GraphicsRenderer::ConfigureViewport() {
    viewport_ = { 0.0f, 0.0f, static_cast<float>(windowWidth_), static_cast<float>(windowHeight_), 0.0f, 1.0f };
}

void GraphicsRenderer::ConfigureScissorRect() {
    scissorRect_ = { 0, 0, windowWidth_, windowHeight_ };
}

void GraphicsRenderer::SetPipelineState(PipelineStateObject pso) {
    assert(pso < MAXPSO);
    auto* cmdList = DirectX12::GetInstance()->GetCommandList();
    cmdList->SetPipelineState(pipelineState_[pso].Get());
    cmdList->SetGraphicsRootSignature(rootSignature_[pso].Get());
    cmdList->RSSetViewports(1, &viewport_);
    cmdList->RSSetScissorRects(1, &scissorRect_);
    cmdList->IASetPrimitiveTopology(pso == PARTICLE ? D3D_PRIMITIVE_TOPOLOGY_POINTLIST : D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->OMSetBlendFactor(blendFactor_);
}

void GraphicsRenderer::StartDraw() {
    auto* dx12 = DirectX12::GetInstance();
    auto* cmdList = dx12->GetCommandList();
    auto* rtvHandle = dx12->GetRTVHandle();
    auto* dsvHandle = dx12->GetDSVHandle();
    auto* swapChain = dx12->GetSwapChain();

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dx12->GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    cmdList->ResourceBarrier(1, &barrier);
    cmdList->OMSetRenderTargets(1, rtvHandle, false, dsvHandle);

    FLOAT clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };
    cmdList->ClearRenderTargetView(*rtvHandle, clearColor, 0, nullptr);
    cmdList->ClearDepthStencilView(*dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void GraphicsRenderer::FinishDraw() {
    auto* dx12 = DirectX12::GetInstance();
    auto* cmdList = dx12->GetCommandList();
    auto* swapChain = dx12->GetSwapChain();

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dx12->GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    cmdList->ResourceBarrier(1, &barrier);
    swapChain->Present(1, 0);
}

void GraphicsRenderer::Log(const std::string& message) {
    std::ofstream logFile("GraphicsRenderer.log", std::ios_base::app);
    logFile << message;
}
