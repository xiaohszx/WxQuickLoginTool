#pragma once
#include <string>


std::wstring stringToWstring(const std::string &str);

std::string wstringToString(const std::wstring & wstr);

std::string wstringToUtf8(const std::wstring& str);

std::wstring utf8ToWstring(const std::string& str);