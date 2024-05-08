#pragma once
#include <string>
//拡張子を取得する関数
std::string GetExtention(std::string filePath);
//ファイル名を取得する関数
std::string GetFileNameWithoutExtension(const std::string& filePath);