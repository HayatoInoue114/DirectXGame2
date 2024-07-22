#pragma once
#include "../object/Model/Model.h"
#include "../manager/PrimitiveManager/PrimitiveManager.h"
#include "../object/Particle/Particle.h"

#include "../object/Object3d/Object3d.h"
#include "../object/Camera/Camera.h"
#include "../object/Sphere/Sphere.h"
#include "../object/Skybox/Skybox.h"

#include "IScene.h"

class TestScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;

private:
	Input* input_{};

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Object3d> object_;
	std::unique_ptr<Model> model_;

	std::unique_ptr<Object3d> object1_;
	std::unique_ptr<Model> model1_;

	std::unique_ptr<Object3d> object2_;
	std::unique_ptr<Model> model2_;

	std::unique_ptr<Object3d> dome_;

	std::unique_ptr<Model> domeModel_;

	std::unique_ptr<Skybox> skybox_;
};

