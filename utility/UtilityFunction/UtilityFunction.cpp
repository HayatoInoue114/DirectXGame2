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

void DrawMatrix4x4(const Matrix4x4& matrix) {
#ifdef _DEBUG
    ImGui::Text("Matrix4x4:");

    for (int i = 0; i < 4; ++i) {
        ImGui::Text("%.3f, %.3f, %.3f, %.3f",
            matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
    }
#else
    (void)matrix;
#endif // DEBUG
}

void DrawMatrix4x4(const char* title, const Matrix4x4& matrix) {
#ifdef _DEBUG
    ImGui::Begin(title);
    ImGui::Text("Matrix4x4:");

    for (int i = 0; i < 4; ++i) {
        ImGui::Text("%.3f, %.3f, %.3f, %.3f",
            matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
    }
    ImGui::End();
#else
    (void)title;
    (void)matrix;
#endif // DEBUG
}

void DrawVector3(const Vector3& v) {
#ifdef _DEBUG
    ImGui::Begin("test");
    ImGui::Text("Vector3 : %d, %d, %d", v.x, v.y, v.z);
    ImGui::End();
#else
    (void)matrix;
#endif // DEBUG
}