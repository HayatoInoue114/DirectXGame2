#include "TestScene.h"
#include "../base/GraphicsRenderer/GraphicsRenderer.h"

void TestScene::Initialize() {
	// 入力
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	
	model_ = std::make_unique<Model>();
	model_ = Model::CreateModelPtr("walk.gltf");
	object_ = std::make_unique<Object3d>();
	object_->Init(model_.get(), camera_.get());
	/*object_->LoadAnimation("walk/walk.gltf");
	object_->StartAnimation(true);
	object_->SetAnimationSpeed(1.0f);*/

	/*model1_ = std::make_unique<Model>();
	model1_ = Model::CreateModelFromObjPtr("walk","sneakWalk.gltf");
	object1_ = std::make_unique<Object3d>();
	object1_->Init(model1_.get(), camera_.get());
	object1_->LoadAnimation("walk/sneakWalk.gltf");
	object1_->StartAnimation(true);
	object1_->SetAnimationSpeed(2.0f);

	model2_ = std::make_unique<Model>();
	model2_ = Model::CreateModelFromObjPtr("simpleSkin.gltf");
	object2_ = std::make_unique<Object3d>();
	object2_->Init(model2_.get(), camera_.get());
	object2_->LoadAnimation("simpleSkin/simpleSkin.gltf");
	object2_->StartAnimation(true);
	object2_->SetAnimationSpeed(3.0f);*/

	domeModel_ = std::make_unique<Model>();
	domeModel_ = Model::CreateModelPtr("skydome.obj");

	dome_ = std::make_unique<Object3d>();
	dome_->Init(domeModel_.get(), camera_.get());

	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize();
	skybox_->SetCamera(camera_.get());
	skybox_->SetScale({ 1000,1000,1000 });
}

void TestScene::Update() {
	camera_->Update();
	object_->UpdateAnimation();
	/*object1_->UpdateAnimation();
	object2_->UpdateAnimation();

	object1_->SetTranslate({ -5,0,0 });
	object2_->SetTranslate({ 5,0,0 });
	object2_->SetRotate({ 0,3.14f,0 });*/
}

void TestScene::Draw() {
	//dome_->Draw();
	object_->Draw();
	skybox_->Draw();
	/*object1_->Draw();
	object2_->Draw();*/
}

void TestScene::Finalize() {
	//primitiveManager_.Finalize();
}