#pragma once

#include <string>
#include <Windows.h>

std::wstring TrimEnd(std::wstring s);

std::wstring TrimStart(std::wstring s);

void folderForPath(LPSTR *folder, LPSTR *path);

void charStrToWideChar(WCHAR *dest, char *source);

std::string filenameForPath(std::string& path);

std::string folderForPath(std::string& path);