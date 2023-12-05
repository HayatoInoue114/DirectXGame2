#pragma 
#include "../DirectX12/DirectX12.h"
#include <wrl.h>
#include <dxcapi.h>

class DirectX12;

class GraphicsRenderer
{
public:
	static GraphicsRenderer* GetInstance();

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

	void CreateRootSignature();

	void InputLayout();

	void BlendState();

	void ResterizerState();

	void BuildShader();

	void CreatePSO();

	void DrawCall();

	void Viewport();

	void ScissorRect();

	void Release();

	void DepthStencilState();
private:
	DirectX12* directX12_{};

	IDxcUtils* dxcUtils_{};
	IDxcCompiler3* dxcCompiler_{};
	IDxcIncludeHandler* includeHandler_{};

	IDxcBlobEncoding* shaderSource_{};
	IDxcResult* shaderResult_{};
	IDxcBlobUtf8* shaderError_{};
	IDxcBlob* shaderBlob_{};


	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob_{};
	ID3DBlob* errorBlob_{};
	//バイナリを元に生成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_{};

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_{};
	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	IDxcBlob* vertexShaderBlob_{};
	IDxcBlob* pixelShaderBlob_{};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3]{};

	//実際に生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_{};

	//ビューポート
	D3D12_VIEWPORT viewport_{};
	//シザー矩形
	D3D12_RECT scissorRect_{};

	D3D12_DESCRIPTOR_RANGE descriptorRange_[1]{};

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1]{};

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	descriptorRangeForInstancing_
};

