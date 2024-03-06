#pragma once
#include "../../object/Primitive/Primitive/Primitive.h"
//#include "../../object/Primitive/PrimitiveCommon/PrimitiveCommon.h"
#include "../../object/Camera/Camera.h"
#include "../../object/Model/Model.h"

#include <memory>


class PrimitiveManager
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	std::unique_ptr<Primitive> primitive_;
	//std::unique_ptr<PrimitiveCommon> primitiveCommon_;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Model> model_;
};

