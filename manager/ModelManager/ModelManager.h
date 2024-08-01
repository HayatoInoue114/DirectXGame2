#pragma once
#include "../../structure/ModelData/ModelData.h"
#include <list>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "../../utility/UtilityFunction/UtilityFunction.h"
#include "../../base/DirectX12/DirectX12.h"

class ModelManager
{
public:
	// コンストラクタ
	ModelManager() = default;
	// デストラクタ
	~ModelManager() = default;

	// シングルトン
	static ModelManager* GetInstance();

	void LoadModel(const std::string& filePath);
	void LoadModel(const std::string& directoryPath, const std::string& filePath);
	ModelData GetModel(const std::string& filePath);
	
public:
	

private:
	// objの読み込み
	ModelData LoadObj(const std::string& filename);
	// objファイルの読み込み
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	//gltfファイルの読み込み
	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);
	ModelData LoadFile(const std::string& filename);
	//拡張子まで入れて読み込む
	ModelData LoadFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

	

private:
	std::map<std::string, ModelData> modelDatas;
};