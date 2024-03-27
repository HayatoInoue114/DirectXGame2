#pragma once
#include "../../object/Particle/Particle.h"

class ParticleManager
{
public:
	void Initialize();

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { this->camera_ = camera; }

private:
	WorldTransform worldTransform_;

	TransformationMatrix wvpData_{};

	Camera* camera_ = nullptr;

	Particle particle_;
};

