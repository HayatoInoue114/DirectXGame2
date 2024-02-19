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

	void SetRootSignatureAndPSO(int n);
private:
	DirectX12* directX12_;
	IDxcUtils* dxcUtils_{};
	IDxcCompiler3* dxcCompiler_{};
	IDxcIncludeHandler* includeHandler_{};

	IDxcBlobEncoding* shaderSource_{};
	IDxcResult* shaderResult_{};
	IDxcBlobUtf8* shaderError_{};
	IDxcBlob* shaderBlob_{};

	

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

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineManagerStateDesc_[MAXPSO]{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[MAXPSO][3]{};

	//実際に生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineManagerState_[MAXPSO]{};

	//ビューポート
	D3D12_VIEWPORT viewport_{};
	//シザー矩形
	D3D12_RECT scissorRect_{};

	D3D12_DESCRIPTOR_RANGE descriptorRange_[MAXPSO][1]{};
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing_[1]{};

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[MAXPSO][1]{};

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
};


//#pragma 
//#include "../DirectX12/DirectX12.h"
//#include <wrl.h>
//#include <dxcapi.h>
//class DirectX12;
//
//class GraphicsRenderer
//{
//public:
//	static GraphicsRenderer* GetInstance();
//
//
//	void Initialize(DirectX12* directX12);
//
//	void Dxc();
//
//	IDxcBlob* CompileShader(
//		//CompilerするShaderファイルへのパス
//		const std::wstring& filePath,
//		//Compilerに使用するProfile
//		const wchar_t* profile,
//		//初期化で生成したものを3つ
//		IDxcUtils* dxcUtils,
//		IDxcCompiler3* dxcComiler,
//		IDxcIncludeHandler* includeHandler
//	);
//
//	void CreateRootSignature(DirectX12* directX12);
//
//	void InputLayout();
//
//	void BlendState();
//
//	void ResterizerState();
//
//	void BuildShader();
//
//	void CreatePSO(DirectX12* directX12);
//
//	void DrawCall(DirectX12* directX12);
//
//	void Viewport();
//
//	void ScissorRect();
//
//	void Release();
//
//	void DepthStencilState();
//private:
//	DirectX12* directX12;
//
//	IDxcUtils* dxcUtils;
//	IDxcCompiler3* dxcCompiler;
//	IDxcIncludeHandler* includeHandler;
//
//	IDxcBlobEncoding* shaderSource;
//	IDxcResult* shaderResult;
//	IDxcBlobUtf8* shaderError;
//	IDxcBlob* shaderBlob;
//
//
//	//シリアライズしてバイナリにする
//	ID3DBlob* signatureBlob;
//	ID3DBlob* errorBlob;
//	//バイナリを元に生成
//	ID3D12RootSignature* rootSignature;
//
//	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
//
//	//BlendStateの設定
//	D3D12_BLEND_DESC blendDesc;
//	//RasiterzerStateの設定
//	D3D12_RASTERIZER_DESC rasterizerDesc;
//
//	IDxcBlob* vertexShaderBlob;
//	IDxcBlob* pixelShaderBlob;
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc;
//
//	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3];
//
//	//実際に生成
//	ID3D12PipelineState* graphicsPipelineState;
//
//	//ビューポート
//	D3D12_VIEWPORT viewport;
//	//シザー矩形
//	D3D12_RECT scissorRect;
//
//	D3D12_DESCRIPTOR_RANGE descriptorRange[1];
//
//	D3D12_STATIC_SAMPLER_DESC staticSamplers[1];
//
//	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
//};


