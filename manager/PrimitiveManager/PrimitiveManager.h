#pragma once
#include "../../object/Primitive/Primitive/Primitive.h"
//#include "../../object/Primitive/PrimitiveCommon/PrimitiveCommon.h"
#include "../../object/Camera/Camera.h"
#include "../../object/Model/Model.h"
#include "../../object/Particle/Particle.h"
#include "../../Project/Skydome/Skydome.h"
#include "../../Project/RailCamera/RailCamera.h"
#include "../../Project/Player/Player.h"
#include "../../Project/Enemy/Enemy.h"

#include <memory>


class PrimitiveManager
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	void Draw2D();

	void Draw3D();
private:
	std::unique_ptr<Primitive> primitive_;

	std::unique_ptr<Model> model_;

	std::unique_ptr<Primitive> primitive2_;

	std::unique_ptr<Model> model2_;
	//std::unique_ptr<PrimitiveCommon> primitiveCommon_;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Skydome> dome_;

	std::unique_ptr<Model> domeModel_;

	std::unique_ptr<RailCamera> railCamera_;

	//std::unique_ptr<Particle> particle_{};

	Particle particle_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
};

