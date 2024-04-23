#pragma once
#include "../../structure/ModelData/ModelData.h"
#include <list>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

enum ModelName {
	CUBE,
	SPHERE,
	SKYDOME,
	PLAYER,
	ENEMY,
	PLANE,

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

	void LoadModel(const std::string& filePath);
	ModelData GetModel(const std::string& filePath);
public:
	// Getter
	ModelData* GetModelData() { return modelData_; }

private:
	// objの読み込み
	ModelData LoadObj(const std::string& filename);
	// objファイルの読み込み
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	//gltfファイルの読み込み
	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

	

private:
	std::map<std::string, ModelData> modelDatas;
	ModelData modelData_[MODELNUM];
};