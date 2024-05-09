#pragma once
#include <string>
#include "../../math/MyMath.h"
//拡張子を取得する関数
std::string GetExtention(const std::string& filePath);
//ファイル名を取得する関数
std::string GetFileNameWithoutExtension(const std::string& filePath);

void DrawMatrix4x4(const Matrix4x4& matrix);

void DrawMatrix4x4(const char* title, const Matrix4x4& matrix);