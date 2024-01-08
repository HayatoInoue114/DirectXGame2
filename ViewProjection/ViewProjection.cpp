#include "ViewProjection.h"

void ViewProjection::Initialize() {

	matView = MakeIdentity4x4();
	matProjection = MakeIdentity4x4();
	sMatProjection = MakeIdentity4x4();
}

void ViewProjection::UpdateMatrix() {

	matView = Inverse(MakeAffineMatrix(scale, rotate, translate));
	matProjection = MakePerspectiveFovMatrix(fov, aspectRatio, nearZ, farZ);

	sMatView = MakeIdentity4x4();
	sMatProjection = MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);

}