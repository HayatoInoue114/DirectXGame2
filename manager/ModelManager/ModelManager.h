#pragma once
#include "../../structure/ModelData/ModelData.h"
#include <list>

enum ModelName {
	CUBE,
	SPHERE,
	SKYDOME,
	PLAYER,
	ENEMY,
	TITLE,

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

	// objファイルの読み込み
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	// Getter
	ModelData* GetModelData() { return modelData_; }

private:
	ModelData modelData_[MODELNUM];
};