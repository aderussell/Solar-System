#include "String_Additions.h"
#include <algorithm>
#include <functional>
#include <cctype>
#include <cstring>

std::wstring TrimStart(std::wstring s) 
{
        s.erase(s.begin(), std::find_if(s.begin(), 
			    s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

std::wstring TrimEnd(std::wstring s) 
{
        s.erase(std::find_if(s.rbegin(), s.rend(), 
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

void folderForPath(LPSTR *folder, LPSTR *path)
{
	char *pch = strrchr(*path, '\\' );
	int position = pch - *path;

	strncpy(*folder, *path, position+1);
}


std::string folderForPath(std::string& path)
{
	unsigned found = path.find_last_of("/\\");
	return path.substr(0, found+1);
}

std::string filenameForPath(std::string& path)
{
	unsigned found = path.find_last_of("/\\");
	return path.substr(found+2);
}


//**************************************************************************//
// Convert an old chracter (char *) string to a WCHAR * string.  There must//
// be something built into Visual Studio to do this for me, but I can't		//
// find it - Nigel.															//
//**************************************************************************//
void charStrToWideChar(WCHAR *dest, char *source)
{
	int length = strlen(source);
	for (int i = 0; i <= length; i++)
		dest[i] = (WCHAR) source[i];
}