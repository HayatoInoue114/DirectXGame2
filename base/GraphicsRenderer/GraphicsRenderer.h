#pragma 
#include "../DirectX12/DirectX12.h"
#include <wrl.h>
#include <dxcapi.h>

#define MAXPSO GraphicsRenderer::MaxPSO

class GraphicsRenderer
{
public:
	static GraphicsRenderer* GetInstance();

	void Initialize();

	void RSSet();

	void DrawCall();

	void Viewport();

	void ScissorRect();

	void SetRootSignatureAndPSO(int n);

	enum PSOName
	{
		Object3d,
		Particle,
		Skinning,
		CopyImage,
		Grayscale,
		Vignette,
		BoxFilter,


		MaxPSO
	};

private:
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

	void DepthStencilState();

private:

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_{};
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_{};
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_{};

	IDxcBlobEncoding* shaderSource_{};
	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError_{};

	IDxcResult* shaderResult_{};
	IDxcBlob* shaderBlob_{};

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_[MAXPSO]{};
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_[MAXPSO]{};

	//バイナリを元に生成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_[MAXPSO]{};

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_[MAXPSO];

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_[MAXPSO]{};

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_[MAXPSO]{};
	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> skinningVertexShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> copyImageVertexShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> copyImagePixelShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> grayscalePixelShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> boxFilterPixelShaderBlob_{};
	Microsoft::WRL::ComPtr<IDxcBlob> vignettePixelShaderBlob_{};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineManagerStateDesc_[MAXPSO]{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[MAXPSO][5]{};

	//実際に生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineManagerState_[MAXPSO]{};

	//ビューポート
	D3D12_VIEWPORT viewport_{};
	//シザー矩形
	D3D12_RECT scissorRect_{};

	D3D12_DESCRIPTOR_RANGE descriptorRange_[1]{};
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing_[1]{};
	D3D12_DESCRIPTOR_RANGE descriptorRangeForPalette_[1]{};

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[MAXPSO][1]{};

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
};
