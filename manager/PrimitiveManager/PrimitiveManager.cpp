#include "PrimitiveManager.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

void PrimitiveManager::Initialize() {


}

void PrimitiveManager::Update() {	

}

void PrimitiveManager::Draw() 
{
	Draw2D();
	Draw3D();
}

void PrimitiveManager::Draw2D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);


}

void PrimitiveManager::Draw3D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
}
