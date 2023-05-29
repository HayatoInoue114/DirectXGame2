#pragma 
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"

#pragma comment(lib,"dxcompiler.lib")

class DirectX12;

class GraphicsRenderer 
{
public:
	void Init(DirectX12* directX12);

	void Update();

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

	void CreateVertexResource(DirectX12* directX12);

	void CreateVertexBufferView();

	void WriteDataToResource();

	void Viewport();
	void ScissorRect();

	void DrawCall();

	void Release();
private:
	HRESULT hr;

	IDxcUtils* dxcUtils;
	IDxcCompiler3* dxcCompiler;
	IDxcIncludeHandler* includeHandler;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptiomnRootSignature{};
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

	DirectX12* directX12;

	//実際に生成
	ID3D12PipelineState* graphicsPipelineState;

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc;

	//実際に頂点リソースを作る
	ID3D12Resource* vertexResource;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む
	Vector4* vertexData;

	//ビューポート
	D3D12_VIEWPORT viewport;
	//シザー矩形
	D3D12_RECT scissorRect;
};

