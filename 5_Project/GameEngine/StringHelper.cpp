

#include "pch.h"
#include "StringHelper.h"

using namespace GameEngineSpace;

std::wstring StringHelper::StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

std::string StringHelper::WideToString(std::wstring wstr)
{
	std::string std_string;
	std_string.assign(wstr.begin(), wstr.end());

	return std_string;
}

std::string StringHelper::GetDirectoryFromPath(const std::string& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');
	if (off1 == std::string::npos && off2 == std::string::npos) //If no slash or backslash in path?
	{
		return "";
	}
	if (off1 == std::string::npos)
	{
		return filepath.substr(0, off2);
	}
	if (off2 == std::string::npos)
	{
		return filepath.substr(0, off1);
	}
	//If both exists, need to use the greater offset
	return filepath.substr(0, std::max(off1, off2));
}

std::string StringHelper::GetFileExtension(const std::string& filename)
{
	size_t off = filename.find_last_of('.');
	if (off == std::string::npos)
	{
		return {};
	}
	return std::string(filename.substr(off + 1));
}

// 250글자의 문장만 된다. 더 길이 긴 건 없겠지. 있으면 나눠써야지 그게 이상한거.
std::wstring StringHelper::UTF8ToUnicode(std::string str)
{
	char strUTF8[500] = {};
	wchar_t strUnicode[252] = {};

	strcpy_s(strUTF8, 500, str.c_str());

	int nLen = MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		NULL,
		NULL
	);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		strUnicode,
		nLen
	);

	std::wstring _returnStr(strUnicode);
	return _returnStr;
}
