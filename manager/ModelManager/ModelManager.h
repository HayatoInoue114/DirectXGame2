#pragma once
#include "../../structure/ModelData/ModelData.h"
#include <list>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum ModelName {
	CUBE,
	SPHERE,
	SKYDOME,
	PLAYER,
	ENEMY,

	MODELNUM
};

class ModelManager
{
public:
	// コンストラクタ
	ModelManager() = default;
	// デストラクタ
	~ModelManager() = default;

	// シングルトン
	static ModelManager* GetInstance();

	// 初期化
	void Initialize();
public:
	// Getter
	ModelData* GetModelData() { return modelData_; }

private:
	// objの読み込み
	ModelData LoadObj(const std::string& filename);
	// objファイルの読み込み
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	ModelData modelData_[MODELNUM];
};