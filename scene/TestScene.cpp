#include "TestScene.h"
#include "../base/GraphicsRenderer/GraphicsRenderer.h"

void TestScene::Initialize() {
	// 入力
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	
	model_ = std::make_unique<Model>();
	model_ = Model::CreateModelFromObjPtr("walk.gltf");

	object_ = std::make_unique<Object3d>();
	object_->Init(model_.get(),camera_.get());
	object_->LoadAnimation("walk/walk.gltf");
	object_->StartAnimation(true);
	//object_->SetAnimationSpeed(10.0f);

	/*domeModel_ = std::make_unique<Model>();
	domeModel_ = Model::CreateModelFromObjPtr("skydome.obj");

	dome_ = std::make_unique<Object3d>();
	dome_->Init(domeModel_.get(),camera_.get());*/
}

void TestScene::Update() {
	camera_->Update();
	object_->UpdateAnimation();
}

void TestScene::Draw() {
	//dome_->Draw();
	object_->Draw();
}

void TestScene::Finalize() {
	//primitiveManager_.Finalize();
}