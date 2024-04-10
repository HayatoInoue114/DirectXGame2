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

	primitive2_ = std::make_unique<Primitive>();
	primitive2_->SetCamera(camera_.get());
	primitive2_->Initialize();
	model2_ = model2_->CreateModelFromObjPtr(ENEMY);
	primitive2_->SetModel(model2_.get());

	domeModel_ = std::make_unique<Model>();
	domeModel_ = domeModel_->CreateModelFromObjPtr(SKYDOME);

	dome_ = std::make_unique<Skydome>();
	dome_->Initialize(domeModel_.get(),camera_.get());

	particle_.Initialize();
}

void PrimitiveManager::Update() {
	camera_->Update();
	primitive_->SetCamera(camera_.get());
	primitive_->Update();
	primitive2_->SetCamera(camera_.get());
	primitive2_->Update();
	particle_.Update();
	particle_.ImGuiAdjustParameter();
}

void PrimitiveManager::Draw() {
	Draw2D();
	Draw3D();
}

void PrimitiveManager::Draw2D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	primitive_->Draw();
	primitive2_->Draw();
	dome_->Draw();
}

void PrimitiveManager::Draw3D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	particle_.Draw(camera_.get(), UVCHECKER);
}
