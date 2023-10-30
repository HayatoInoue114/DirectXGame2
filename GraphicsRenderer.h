#pragma 
#include "DirectX12.h"
#include "Triangle.h"

class DirectX12;

class GraphicsRenderer
{
public:

	void Initialize(DirectX12* directX12);

	void Dxc();

	IDxcBlob* CompileShader(
		//CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcComiler,
		IDxcIncludeHandler* includeHandler
	);

	void CreateRootSignature(DirectX12* directX12);

	void InputLayout();

	void BlendState();

	void ResterizerState();

	void BuildShader();

	void CreatePSO(DirectX12* directX12);

	void DrawCall(DirectX12* directX12);

	void Viewport();

	void ScissorRect();

	void Release();

	void DepthStencilState();
private:
	DirectX12* directX12;
	Triangle* triangle;

	IDxcUtils* dxcUtils;
	IDxcCompiler3* dxcCompiler;
	IDxcIncludeHandler* includeHandler;

	IDxcBlobEncoding* shaderSource;
	IDxcResult* shaderResult;
	IDxcBlobUtf8* shaderError;
	IDxcBlob* shaderBlob;


	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob;
	ID3DBlob* errorBlob;
	//バイナリを元に生成
	ID3D12RootSignature* rootSignature;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc;
	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc;

	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3];

	//実際に生成
	ID3D12PipelineState* graphicsPipelineState;

	//ビューポート
	D3D12_VIEWPORT viewport;
	//シザー矩形
	D3D12_RECT scissorRect;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1];

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1];

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
};

