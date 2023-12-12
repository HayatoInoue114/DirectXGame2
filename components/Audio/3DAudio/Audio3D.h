#pragma once
#include <wrl.h>
#include <xaudio2.h>

class Audio3D
{
public:
	void Initialize();

	void Update();

	HRESULT InitializeCOM();
};

