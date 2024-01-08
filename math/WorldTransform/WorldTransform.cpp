#include "WorldTransform.h"

void WorldTransform::Initialize() {

	matWorld = MakeIdentity4x4();

}

void WorldTransform::TransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, ViewProjection& view) {

	TransformationMatrix* wvp = {};
	matWorld = Multiply(matWorld, Multiply(view.matView, view.matProjection));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
	wvp->WVP = matWorld;
	wvp->World = worldMatrix;

}

void WorldTransform::STransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, ViewProjection& view)
{
	TransformationMatrix* wvp = {};
	sMatWorld = Multiply(matWorld, Multiply(view.sMatView, view.sMatProjection));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
	wvp->WVP = sMatWorld;
	wvp->World = worldMatrix;
}

void WorldTransform::UpdateMatrix() {

	matWorld = MakeAffineMatrix(scale, rotate, translate);
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	if (parent) {
		matWorld = Multiply(matWorld, parent->matWorld);
	}

}