#include "Skydome.h"

void Skydome::Initialize(Particle* model) {
	model_ = model;
	worldTransform_.Initialize();
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection viewProjection) {/* model_->Draw(worldTransform_, viewProjection, SKYDOMETEX);*/ }