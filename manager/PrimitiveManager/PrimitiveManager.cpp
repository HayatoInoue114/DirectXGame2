#include "PrimitiveManager.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

void PrimitiveManager::Initialize() {
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();


	/*primitiveCommon_ = std::make_unique<PrimitiveCommon>();
	primitiveCommon_->Initialize();*/

	primitive_ = std::make_unique<Primitive>();
	primitive_->SetCamera(camera_.get());
	primitive_->Initialize();

	model_ = model_->CreateModelFromObjPtr(PLAYER);

	primitive_->SetModel(model_.get());

	particle_ = std::make_unique<Particle>();
	particle_->Initialize();
}

void PrimitiveManager::Update() {
	camera_->Update();
	primitive_->SetCamera(camera_.get());
	primitive_->Update();

	particle_->SetCamera(camera_.get());
	particle_->Update();
}

void PrimitiveManager::Draw() {
	
	Draw2D();
	Draw3D();
}

void PrimitiveManager::Draw2D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	primitive_->Draw();
}

void PrimitiveManager::Draw3D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	particle_->Draw(CIRCLE);
}
