#pragma once
#include <string.h>
#include <format>
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")


std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);
void Log(const std::string& message);