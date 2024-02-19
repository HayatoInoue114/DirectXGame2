#pragma once
#include "../../base/DirectX12/DirectX12.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../base/WindowsAPI/WindowsAPI.h"
#include "../../object/Traiangle/Triangle.h"
#include "../../manager/ImGuiManager/ImGuiManager.h"
#include "../../object/Sprite/Sprite.h"
#include "../../object/Sphere/Sphere.h"
#include "../../components/Light/Light.h"
#include "../../object/Model/Model.h"
#include "../../manager/TextureManager/TextureManager.h"

#define MAXTRIANGLE 1 //三角形の最大数
#define MAXSPRITE 1 //スプライトの最大数
#define MAXSPHERE 1 //球の最大数

class GameManager
{
public:
	
	

	/*void PreInit(GraphicsRenderer* graphicsRenderer);*/
	void Initialize();
	void Update();
	void Release();

	void BeginFrame();

	void EndFrame();

	void Finalize();

	void Draw();

	void Draw2D();

	void Draw3D();

	void VariableInit();
private:
	TriangleData triangleData[MAXTRIANGLE] = {};

	Model* model_ = new Model;

	TextureManager* textureManager_{};

	DirectX12* directX12_{};
	GraphicsRenderer* graphicsRenderer_{};
	Triangle** triangle_ = new Triangle* [MAXTRIANGLE];
	//Sprite** sprite_ = new Sprite * [MAXSPRITE];
	Sprite* sprite_ = new Sprite;
	//Sphere** sphere_ = new Sphere* [MAXSPHERE];
	Sphere* sphere_ = new Sphere;

	float colorVolume_[3] = {};
	Vector4 color_ = {1.0f,0.0f,0.0f,1.0f};

	Transform transform_ = {};
	float scale_[3] = { 1.0f,1.0f,0.0f };
	float rotate_[3] = { 0.0f,0.0f,0.0f };
	float translate_[3] = { 0.0f,0.0f,0.0f };

	Light* light_{};
};

