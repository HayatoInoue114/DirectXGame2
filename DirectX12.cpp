#include "DirectX12.h"
std::wstring ConvertString(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

//wstring->string
std::string ConvertString(const std::wstring& str)
{
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}




DirectX12::DirectX12()
{
	hr = {};
	device = {};
	dxgiFactory = nullptr;
	useAdapter = nullptr;

}

DirectX12::~DirectX12()
{
}

void DirectX12::DXGIFactory() {
	
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
}

void DirectX12::Adapter() {
	//良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプター情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));//取得できないのは一大事
		//ソフトウェアアダプタでなければ採用!
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;//ソフトウェアアダプタの場合は見なかったことにする
	}

	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);
}

void DirectX12::D3D12Device() {
	
		//機能レベルとログ出力用の文字列
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
		};
		const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
		//高い順の生成できるか試していく
		for (size_t i = 0; i < _countof(featureLevels); ++i) {
			//採用したアダプターでデバイスを生成
			hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
			//指定した機能レベルでデバイスが生成できたかを確認
			if (SUCCEEDED(hr)) {
				//生成できたのでログ出力を行ってループを抜ける
				Log(std::format("featureLevel : {}\n",featureLevelStrings[i]));
				break;
			}
		}
	
		//デバイスの生成がうまくいかなかったので起動できない
		assert(device != nullptr);
		Log("Complete create D3D12Device!!!\n");//初期化完了のログを出す
}

void DirectX12::Init() {
	DXGIFactory();
	Adapter();
	D3D12Device();
}
