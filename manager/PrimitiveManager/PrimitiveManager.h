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
	void Draw2D();

	void Draw3D();

	
private:

	
};

