//#pragma once
//#include "../../base/DirectX12/DirectX12.h"
//#include "../../base/GraphicsRenderer/GraphicsRenderer.h"
//#include "../../base/WindowsAPI/WindowsAPI.h"
//#include "../../object/Traiangle/Triangle.h"
//#include "../../manager/ImGuiManager/ImGuiManager.h"
//#include "../../object/Sprite/Sprite.h"
//#include "../../object/Sphere/Sphere.h"
//#include "../../components/Light/Light.h"
//#include "../../object/Model/Model.h"
//#include "../../manager/TextureManager/TextureManager.h"
//#include "../../manager/ModelManager/ModelManager.h"
//
//#define MAXTRIANGLE 1 //三角形の最大数
//#define MAXSPRITE 1 //スプライトの最大数
//#define MAXSPHERE 1 //球の最大数
//
//class GameManager
//{
//public:
//	
//	
//
//	/*void PreInit(GraphicsRenderer* graphicsRenderer);*/
//	void Initialize();
//	void Update();
//	void Release();
//
//	void BeginFrame();
//
//	void EndFrame();
//
//	void Finalize();
//
//	void Draw();
//
//	void Draw2D();
//
//	void Draw3D();
//
//	void VariableInit();
//private:
//	TriangleData triangleData[MAXTRIANGLE] = {};
//
//	Model* model_ = new Model;
//
//	TextureManager* textureManager_{};
//	ModelManager* modelManager_{};
//
//	DirectX12* directX12_{};
//	GraphicsRenderer* graphicsRenderer_{};
//	Triangle** triangle_ = new Triangle* [MAXTRIANGLE];
//	//Sprite** sprite_ = new Sprite * [MAXSPRITE];
//	Sprite* sprite_ = new Sprite;
//	//Sphere** sphere_ = new Sphere* [MAXSPHERE];
//	Sphere* sphere_ = new Sphere;
//
//	float colorVolume_[3] = {};
//	Vector4 color_ = {1.0f,0.0f,0.0f,1.0f};
//
//	Transform transform_ = {};
//	float scale[3] = { 1.0f,1.0f,0.0f };
//	float rotate_[3] = { 0.0f,0.0f,0.0f };
//	float translate_[3] = { 0.0f,0.0f,0.0f };
//
//	Light* light_{};
//};
//

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
//#include "../../object/Particle/Particle.h"
#include "../../Input/Input.h"
#include "../../ViewProjection/ViewProjection.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../ModelManager/ModelManager.h"

#include "../../Project/Player/Player.h"
#include "../../Project/Enemy/Enemy.h"
#include "../../Project/TimedCall/TimedCall.h"

#include "../../scene/IScene.h"
#include "../../scene/TitleScene.h"
#include "../../scene/GameScene.h"
#include "../../scene/GameClearScene.h"

#define MAXTRIANGLE 1 //三角形の最大数
#define MAXSPRITE 1 //スプライトの最大数
#define MAXSPHERE 1 //球の最大数

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Initialize();

	void Update();

	void Release();

	void BeginFrame();

	void EndFrame();

	void Finalize();

	/*void Draw();

	void Draw2D();
	
	void Draw3D();*/

	void VariableInit();

	/*enum Scene
	{
		TITLESCENE,
		GAMESCENE,
		GAMECLEARSCENE
	};*/

private:
	std::unique_ptr<Model> model_{};

	//std::unique_ptr<Particle> particle_{};

	Sprite* sprite_{};

	TextureManager* textureManager_{};

	ModelManager* modelManager_{};

	GameScene* gameScene_{};

	TitleScene* titleScene_{};

	GameClearScene* gameClearScene_{};

	DirectX12* directX12_{};

	GraphicsRenderer* graphicsRenderer_{};

	Light* light_{};

	IScene* sceneArr_[4]{};

	Input* input_{};

	int sceneNum_{};

	int preSceneNum_{};

	Sprite* titleSprite_{};

	Sprite* clearSprite_{};

	WorldTransform transform_{};
};


