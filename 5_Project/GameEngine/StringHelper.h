#pragma once
#include<algorithm>

namespace GameEngineSpace
{
	/// <summary>
	/// ���ڿ��� ���� �� ������ �ִ� �Լ� ����.	
	/// 2022.08.01  �۾��� : Almond
	/// </summary>
	class StringHelper
	{
	public:
		static std::wstring StringToWide(std::string str);
		static std::string WideToString(std::wstring wstr);
		static std::string GetDirectoryFromPath(const std::string& filepath);
		static std::string GetFileExtension(const std::string& filename);
		static std::wstring UTF8ToUnicode(std::string str);
	};
}
