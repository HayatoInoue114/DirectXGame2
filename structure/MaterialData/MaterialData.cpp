#include "MaterialData.h"
#include <assert.h>

MaterialData LoadMaterialTemplateFile(const std::string& filename) {
	MaterialData materialData; // 構築するMaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file("resources/" + filename); // ファイルを開く
	assert(file.is_open());

	while (std::getline(file,line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = "resources/" + textureFilename;
		}
		
	}
	return materialData;
}