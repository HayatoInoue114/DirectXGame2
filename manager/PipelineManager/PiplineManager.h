//#pragma once
//
//#include "../../base/DirectX12/DirectX12.h"
//#include "../../utility/FormatString/FormatString.h"
////#include "CreateResource.h"
//
//// 共通で使うやつ
//struct Property {
//	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
//	Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineManagerState_ = nullptr;;
//	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_ = nullptr;
//	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;
//};
//
//// pipelineState
//struct PipelineState {
//	Property Object3D;
//	Property Sprite2D;
//};
//
//// BlendMode
//enum BlendMode {
//	BlendNormal,
//};
//
//class PiplineManager {
//public:
//
//	// シングルトンインスタンスの取得
//	static PiplineManager* GetInstance();
//
//	/// <summary>
//	/// 初期化
//	/// </summary>
//	static void Initialize();
//
//	/// <summary>
//	/// psoのgetter
//	/// </summary>
//	/// <returns></returns>
//	PipelineState GetPSO() { return  pso; }
//
//
//private:
//	/// <summary>
//	/// rootSignature作成
//	/// </summary>
//	/// <param name="device"></param>
//	/// <param name="descriptionRootSignature"></param>
//	/// <param name="property"></param>
//	static void CreateRootSignature(Microsoft::WRL::ComPtr <ID3D12Device> device, D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature, Property& property);
//
//	/// <summary>
//	/// blendModeの設定
//	/// </summary>
//	/// <param name="blendDesc"></param>
//	static void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode);
//
//	/// <summary>
//	/// パイプラインを実際に生成
//	/// </summary>
//	/// <param name="pso"></param>
//	static void CreatePipeline(PipelineState& pso);
//
//	/// <summary>
//	///  パイプラインの種類
//	/// </summary>
//	/// <param name="device"></param>
//	/// <param name="shaderName"></param>
//	/// <returns></returns>
//	static Property CreateObject3D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);
//
//	static Property CreateSprite2D(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);
//
//	static Property CreatePointLight(Microsoft::WRL::ComPtr <ID3D12Device> device, const std::wstring& shaderName);
//
//	PipelineState pso = {};
//
//};
