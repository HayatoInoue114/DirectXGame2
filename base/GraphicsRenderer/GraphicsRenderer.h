#pragma 
#include "../DirectX12/DirectX12.h"
#include <wrl.h>
#include <dxcapi.h>

#define MAXPSO 2

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
	DirectX12* directX12_;
	IDxcUtils* dxcUtils_{};
	IDxcCompiler3* dxcCompiler_{};
	IDxcIncludeHandler* includeHandler_{};

	IDxcBlobEncoding* shaderSource_{};
	IDxcResult* shaderResult_{};
	IDxcBlobUtf8* shaderError_{};
	IDxcBlob* shaderBlob_{};

	D3D12_ROOT_PARAMETER rootParameters_[MAXPSO][6] = {};

	//シリアライズしてバイナリにする
	//Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_[MAXPSO]{};
	//Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_[MAXPSO]{};
	ID3DBlob* signatureBlob_[MAXPSO]{};
	ID3DBlob* errorBlob_[MAXPSO]{};
	//バイナリを元に生成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_[MAXPSO]{};

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_[MAXPSO];

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_[MAXPSO]{};

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_[MAXPSO]{};
	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_[MAXPSO]{};

	IDxcBlob* vertexShaderBlob_{};
	IDxcBlob* pixelShaderBlob_{};

	IDxcBlob* particleVertexShaderBlob_{};
	IDxcBlob* particlePixelShaderBlob_{};

	/*Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob_{};
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob_{};

	Microsoft::WRL::ComPtr<ID3DBlob> particleVertexShaderBlob_{};
	Microsoft::WRL::ComPtr<ID3DBlob> particlePixelShaderBlob_{};*/

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_[MAXPSO]{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3]{};

	//実際に生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_[MAXPSO]{};

	//ビューポート
	D3D12_VIEWPORT viewport_{};
	//シザー矩形
	D3D12_RECT scissorRect_{};

	D3D12_DESCRIPTOR_RANGE descriptorRange_[1]{};
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing_[1]{};

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[MAXPSO][1]{};

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
};

