#pragma once


class ResourceLeakCheck
{
public:
	static void Check();

private:
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug_{};
};

