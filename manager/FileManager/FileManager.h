#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <numbers>


#include "json.hpp"
#include "../../math/MyMath.h"
#include "../../object/Model/Model.h"


struct ColliderData {
	std::string type;
	Vector3 center;
	Vector3 size;
};
struct LevelData {

	struct ObjectData {
		std::string type;
		std::string file_name;
		SRT srt;
		std::map<std::string, std::unique_ptr<ObjectData>> children;
	};
	std::map<std::string, std::unique_ptr<ObjectData>> objects;
};
struct LevelObject {
	std::unique_ptr<Model> model;
	WorldTransform transform;
};


/* FileManagerクラス */
class FileManager {

private:

	// コンストラクタ、デストラクタ
	FileManager() = default;
	~FileManager() = default;
	FileManager(const FileManager&) = delete;
	const FileManager& operator=(const FileManager&) = delete;

public:

	// インスタンスの取得
	static FileManager* GetInstance() {
		static FileManager instance;
		return&instance;
	}

	// 初期化処理
	void Initialize();

	// 更新処理　
	void Update();

	// 描画処理
	void Draw();

	// 解放処理
	void Finalize();

	// JSONファイル読み込み
	void LoadJsonFile(const std::string& routeFilePath, const std::string& fileName);


#pragma region Get

	LevelData::ObjectData* GetObjectData(const std::string& key) const {

		if (!levelData_) {
			return nullptr;
		}

		auto it = levelData_->objects.find(key);
		if (it != levelData_->objects.end()) {
			return it->second.get();
		}

		return nullptr;
	}

	SRT GetObjectSrt(const std::string& key) const {

		auto it = levelData_->objects.find(key);
		if (it != levelData_->objects.end()) {
			return it->second.get()->srt;
		}

		return SRT();
	}

#pragma endregion 

#pragma region Set

#pragma endregion 


private:

	// オブジェクトの走査
	void ScanningObjects(nlohmann::json& object, std::map<std::string, std::unique_ptr<LevelData::ObjectData>>& objects);

	// 読み込んだ情報からモデル作成
	void CreateModel();

private:

	// JSONファイルから読み込んだ情報をまとめおく変数
	std::unique_ptr<LevelData> levelData_;

	// 配置するための変数
	std::map<const std::string, std::unique_ptr<LevelObject>> levelObjectMap_;
};

