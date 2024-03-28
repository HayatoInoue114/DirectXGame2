#pragma once
#include "../../object/Particle/Particle.h"
#include "../../structure/Emitter/Emitter.h"

class ParticleManager
{
public:
	void Initialize();

	ParticleData MakeNewParticle(const Vector3& translate);

	std::list<ParticleData> Emission(const Emitter& emitter);

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { this->camera_ = camera; }

private:
	WorldTransform worldTransform_;

	TransformationMatrix wvpData_{};

	Camera* camera_ = nullptr;

	//Particle particle_;

	std::list<Particle> particle_;

	// エミッタ
	Emitter emitter_;
};