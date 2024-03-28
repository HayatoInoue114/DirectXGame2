#pragma once
#include "../object/Model/Model.h"
#include "../manager/PrimitiveManager/PrimitiveManager.h"
#include "../object/Particle/Particle.h"

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

	PrimitiveManager primitiveManager_{};

	//std::unique_ptr<Particle> particle_{};
};
