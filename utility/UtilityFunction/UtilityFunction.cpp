#include "UtilityFunction.h"
#include <filesystem>

std::string GetExtention(const std::string& filePath)
{
	std::filesystem::path path(filePath);
	return path.extension().string();
}

std::string GetFileNameWithoutExtension(const std::string& filePath)
{
	std::filesystem::path path(filePath);
	return path.stem().string();
}