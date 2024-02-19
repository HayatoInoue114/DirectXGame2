#pragma once
#include "../../structure/ModelData/ModelData.h"
#include <list>

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

	// Getter
	ModelData* GetModelData() { return modelData_; }

private:
	// objファイルの読み込み
	ModelData LoadObjFile(const std::string& filename);

private:
	ModelData modelData_[MODELNUM];
};