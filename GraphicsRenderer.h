#pragma 
#include "DirectX12.h"
#include "Triangle.h"

class DirectX12;

class GraphicsRenderer 
{
public:
	/*void Init(DirectX12* directX12);*/

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

	
	void DrawCall(DirectX12 *directX12);
	

	void Viewport();
	void ScissorRect();

	void RootParameter();

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	void CreateMaterialResource();

	void Release();
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

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
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

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1];

	//実際に生成
	ID3D12PipelineState* graphicsPipelineState;

	//ビューポート
	D3D12_VIEWPORT viewport;
	//シザー矩形
	D3D12_RECT scissorRect;

	//RootParameter作成。
	D3D12_ROOT_PARAMETER rootParameters[1];

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	ID3D12Resource* materialResource;
	//マテリアルにデータを書き込む
	Vector4* materialData;
};

