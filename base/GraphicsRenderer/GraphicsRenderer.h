#pragma once

#include "../DirectX12/DirectX12.h"

#include <dxcapi.h>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <array>

#define MAXPSO 3

class DirectX12;

// シェーダーごとの設定を保持する構造体
struct ShaderConfig {
    std::vector<D3D12_ROOT_PARAMETER> rootParameters;
    std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRanges;
    std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
    D3D12_BLEND_DESC blendDesc;
    D3D12_RASTERIZER_DESC rasterizerDesc;
};

enum class ShaderFileNumber : uint32_t
{
    
};

class GraphicsRenderer {
public:
    static GraphicsRenderer* GetInstance();

    void Initialize();
    void Viewport();
    void ScissorRect();
    void DrawCall();
    void SetRootSignatureAndPSO(int n);

private:
    void InitializeDxc();
    IDxcBlob* CompileShader(
        const std::wstring& filePath,
        const wchar_t* profile,
        IDxcUtils* dxcUtils,
        IDxcCompiler3* dxcCompiler,
        IDxcIncludeHandler* includeHandler
    );

    void InitializeShaderConfig(ShaderConfig& config, bool isDefault);
    void CreateRootSignatures();
    void DefineInputLayout();
    void ConfigureBlendState();
    void ConfigureRasterizerState();
    void BuildShaders();
    void CreatePipelineStateObjects();
    void ConfigureDepthStencilState();

private:
    Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
    Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
    Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;

    std::array<Microsoft::WRL::ComPtr<ID3DBlob>, MAXPSO> signatureBlobs_;
    std::array<Microsoft::WRL::ComPtr<ID3DBlob>, MAXPSO> errorBlobs_;
    std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, MAXPSO> rootSignatures_;
    std::array<D3D12_ROOT_SIGNATURE_DESC, MAXPSO> rootSignatureDescs_;

    std::array<D3D12_INPUT_LAYOUT_DESC, MAXPSO> inputLayoutDescs_;
    std::array<std::array<D3D12_INPUT_ELEMENT_DESC, 3>, MAXPSO> inputElementDescs_;

    std::array<D3D12_BLEND_DESC, MAXPSO> blendDescs_;
    std::array<D3D12_RASTERIZER_DESC, MAXPSO> rasterizerDescs_;

    Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_;
    Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_;
    Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob_;
    Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob_;

    std::array<D3D12_GRAPHICS_PIPELINE_STATE_DESC, MAXPSO> pipelineStateDescs_;
    std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, MAXPSO> pipelineStates_;

    D3D12_VIEWPORT viewport_;
    D3D12_RECT scissorRect_;

    std::array<std::array<D3D12_DESCRIPTOR_RANGE, 1>, MAXPSO> descriptorRanges_;
    std::array<D3D12_DESCRIPTOR_RANGE, 1> descriptorRangeForInstancing_;

    std::array<std::array<D3D12_STATIC_SAMPLER_DESC, 1>, MAXPSO> staticSamplers_;

    D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;
};
