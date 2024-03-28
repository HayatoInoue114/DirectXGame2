#include "ParticleManager.h"

void ParticleManager::Initialize() {
	worldTransform_.Initialize();
	//this->camera_ = primitiveCommon_->GetDefaultCamera();
}

ParticleData ParticleManager::MakeNewParticle(const Vector3& translate) {
	ParticleData particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0,0,0 };
	Vector3 randomTranslate{};

	Scope trans = { -1.0f,1.0f };
	ScopeVec3 translateVec3 = { trans,trans,trans };
	randomTranslate = RandomGenerator::getRandom(translateVec3);
	particle.transform.translate = Add(translate, randomTranslate);
	
	//velocity
	Scope vel = { -1.0f,1.0f };
	ScopeVec3 randomVec3 = { vel,vel,vel };
	particle.velocity = RandomGenerator::getRandom(randomVec3);

	//color
	Scope color = { 0,255 };
	ScopeVec4 colorVec4 = { color,color,color,{255,255} };
	particle.color = RandomGenerator::getColorRandom(colorVec4);

	//lifeTime
	Scope time = { 1.0f,3.0f };
	particle.lifeTime = RandomGenerator::getRandom(time);
	particle.currentTime = 0;
	return particle;
}

std::list<ParticleData> ParticleManager::Emission(const Emitter& emitter)
{
	std::list<ParticleData> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle(emitter_.transform.translate));
	}
	return particles;
}

void ParticleManager::Update() {
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix{};
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	wvpData_.WVP = worldViewProjectionMatrix;
	wvpData_.World = worldMatrix;




	//particle_.SetCamera(camera_);
}

void ParticleManager::Draw() {
	//GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	//particle_.Draw(CIRCLE);
}