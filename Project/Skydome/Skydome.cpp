#include "Skydome.h"

void Skydome::Initialize(Particle* model) {
	model_ = model;
	worldTransform_.Initialize();
}

void Skydome::Update() {}

void Skydome::Draw() {/* model_->Draw(worldTransform_, viewProjection, SKYDOMETEX);*/ }