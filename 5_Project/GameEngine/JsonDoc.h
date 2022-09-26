#pragma once
#include "rapidjson/document.h"			// rapidjson's DOM-style API
#include "StringHelper.h"

using namespace rapidjson;
namespace GameEngineSpace
{
	/// <summary>	
	/// Json������ �ܺ������� �ҷ��ͼ� ���ڿ� ���·� �ν��Ͻ�ȭ�ϰ�,
	/// ���Ŀ� �´� �����͸� �ҷ��� �� �ְ� ������ Ŭ����
	/// 
	/// 2022-08-01. �۾��� : Almond
	/// </summary>
	class JsonDoc
	{
	private:
		FILE* fp_ = nullptr;							// ����������
		Document m_document;							// json������ �ø��������� ������ (���ڿ������� ���)
		std::map<int, Value*> m_pArrayedJsonValues;		// ID�� �̿��� Ű����� �����ص� "��� ������Ʈ" ������ �����̳�
		bool m_isArray = false;

	public:
		// ������ / �⺻�Ҹ���, �̵�, ���������
		JsonDoc() {}
		virtual ~JsonDoc() { m_pArrayedJsonValues.clear(); }
		JsonDoc(const JsonDoc& other) = default;
		JsonDoc& operator =(const JsonDoc& other) = default;
		JsonDoc(JsonDoc&& other) = default;

	public:
		// json������ �ҷ����� �Լ�
		bool LoadJsonFromFile(std::string const& file);

		Document& GetDocument();

		inline int GetSize() { return m_pArrayedJsonValues.size(); }

		// Ű ��Ʈ���� ���� ��ġ�� �޴� �Լ�.
		template <typename T>
		T GetValue(std::string const& keystring, int indexOrId = 0, bool valueIsId = true)
		{
			if (m_isArray == true && valueIsId == true)   // ������ 2�����迭�̰�, ID�� ã�´ٸ�..
			{
				Value::ConstMemberIterator itr = m_pArrayedJsonValues[indexOrId]->FindMember(keystring.c_str());
				if (itr != m_pArrayedJsonValues[indexOrId]->MemberEnd())
				{
					if constexpr (std::is_same<int, T>::value)
						return (T)itr->value.GetInt();
					else if constexpr (std::is_same<float, T>::value)
						return (T)itr->value.GetFloat();
					else if constexpr (std::is_same<bool, T>::value)
						return (T)itr->value.GetBool();
					else if constexpr (std::is_same<double, T>::value)
						return (T)itr->value.GetDouble();
					else if constexpr (std::is_same<std::string, T>::value)
						return (T)itr->value.GetString();
					else if constexpr (std::is_same<std::wstring, T>::value)
						return StringHelper::UTF8ToUnicode(itr->value.GetString());
				}
				else
					return NULL;
			}
			else if (m_isArray == true && valueIsId == false) // ������ 2�����迭�̰�, index�� ã�´ٸ�..
			{
				if constexpr (std::is_same<int, T>::value)
					return (T)m_document[indexOrId][keystring.c_str()].GetInt();
				else if constexpr (std::is_same<float, T>::value)
					return (T)m_document[indexOrId][keystring.c_str()].GetFloat();
				else if constexpr (std::is_same<bool, T>::value)
					return (T)m_document[indexOrId][keystring.c_str()].GetBool();
				else if constexpr (std::is_same<double, T>::value)
					return (T)m_document[indexOrId][keystring.c_str()].GetDouble();
				else if constexpr (std::is_same<std::string, T>::value)
					return (T)m_document[indexOrId][keystring.c_str()].GetString();
				else if constexpr (std::is_same<std::wstring, T>::value)
					return StringHelper::UTF8ToUnicode(m_document[indexOrId][keystring.c_str()].GetString());
				else
					return NULL;
			}
			else
			{
				// �������� DOM���� json. ��ť��Ʈ���� �ٷ� ���� ã�� ����
				if constexpr (std::is_same<int, T>::value)
					return (T)m_document[keystring.c_str()].GetInt();
				else if constexpr (std::is_same<float, T>::value)
					return (T)m_document[keystring.c_str()].GetFloat();
				else if constexpr (std::is_same<bool, T>::value)
					return (T)m_document[keystring.c_str()].GetBool();
				else if constexpr (std::is_same<double, T>::value)
					return (T)m_document[keystring.c_str()].GetDouble();
				else if constexpr (std::is_same<std::string, T>::value)
					return (T)m_document[keystring.c_str()].GetString();
				else if constexpr (std::is_same<std::wstring, T>::value)
					return StringHelper::UTF8ToUnicode(m_document[keystring.c_str()].GetString());
				else
					return NULL;
			}
		}

		template <typename T>
		std::vector<T> GetValueArray(std::string const& keystring, int indexOrId = 0, bool valueIsId = true)
		{
			std::vector<T> resultValueArray;

			if (m_isArray == true && valueIsId == true)   // ������ 2�����迭�̰�, ID�� ã�´ٸ�..
			{
				Value::ConstMemberIterator itr = m_pArrayedJsonValues[indexOrId]->FindMember(keystring.c_str());
				if (itr != m_pArrayedJsonValues[indexOrId]->MemberEnd())
				{
					auto valueArray = itr->value.GetArray();										

					if constexpr (std::is_same<int, T>::value)
					{
						for (auto& iter : valueArray)
							resultValueArray.push_back(iter.GetInt());
					}
					else if constexpr (std::is_same<float, T>::value)
					{
						for (auto& iter : valueArray)
							resultValueArray.push_back(iter.GetFloat());
					}
					else if constexpr (std::is_same<bool, T>::value)
					{
						for (auto& iter : valueArray)
							resultValueArray.push_back(iter.GetBool());
					}
					else if constexpr (std::is_same<double, T>::value)
					{
						for (auto& iter : valueArray)						
							resultValueArray.push_back(iter.GetDouble());
						
					}
					else if constexpr (std::is_same<std::string, T>::value)
					{
						for (auto& iter : valueArray)
							resultValueArray.push_back(iter.GetString());
					}
					else if constexpr (std::is_same<std::wstring, T>::value)
					{
						for (auto& iter : valueArray)
							resultValueArray.push_back(StringHelper.UTF8ToUnicode(iter.GetString()));
					}
				}
			}
			else if (m_isArray == true && valueIsId == false) // ������ 2�����迭�̰�, index�� ã�´ٸ�..
			{
				auto valueArray = m_document[indexOrId][keystring.c_str()].GetArray();
				if constexpr (std::is_same<int, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetInt());
				}

				else if constexpr (std::is_same<float, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetFloat());
				}
				else if constexpr (std::is_same<bool, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetBool());
				}
				else if constexpr (std::is_same<double, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetDouble());
				}
				else if constexpr (std::is_same<std::string, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetString());
				}
				else if constexpr (std::is_same<std::wstring, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(StringHelper::UTF8ToUnicode(iter.GetString()));
				}
			}
			else
			{
				auto valueArray = m_document[keystring.c_str()].GetArray();
				if constexpr (std::is_same<int, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetInt());
				}

				else if constexpr (std::is_same<float, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetFloat());
				}
				else if constexpr (std::is_same<bool, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetBool());
				}
				else if constexpr (std::is_same<double, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetDouble());
				}
				else if constexpr (std::is_same<std::string, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(iter.GetString());
				}
				else if constexpr (std::is_same<std::wstring, T>::value)
				{
					for (auto& iter : valueArray)
						resultValueArray.push_back(StringHelper::UTF8ToUnicode(iter.GetString()));
				}
			}
			return resultValueArray;
		}


		template <typename T>
		std::vector<std::vector<T>> GetValue2DArray(std::string const& keystring, int indexOrId = 0, bool valueIsId = true)
		{
			std::vector<std::vector<T>> result2DArray;

			if (m_isArray == true && valueIsId == true)   // ������ 2�����迭�̰�, ID�� ã�´ٸ�..
			{
				Value::ConstMemberIterator itr = m_pArrayedJsonValues[indexOrId]->FindMember(keystring.c_str());
				if (itr != m_pArrayedJsonValues[indexOrId]->MemberEnd())
				{
					auto twoDArray = itr->value.GetArray();

					for (auto& oneDArray : twoDArray)
					{
						auto iterOneDArray = oneDArray.GetArray();
						std::vector<T> resultinter;
						if constexpr (std::is_same<int, T>::value)
						{
							for (auto& iter : iterOneDArray)
								resultinter.push_back(iter.GetInt());
						}
						else if constexpr (std::is_same<float, T>::value)
						{
							for (auto& iter : iterOneDArray)
								resultinter.push_back(iter.GetFloat());
						}
						else if constexpr (std::is_same<bool, T>::value)
						{
							for (auto& iter : iterOneDArray)
								resultinter.push_back(iter.GetBool());
						}
						else if constexpr (std::is_same<double, T>::value)
						{
							for (auto& iter : iterOneDArray)
								resultinter.push_back(iter.GetDouble());

						}
						else if constexpr (std::is_same<std::string, T>::value)
						{
							for (auto& iter : iterOneDArray)
								resultinter.push_back(iter.GetString());
						}
						else if constexpr (std::is_same<std::wstring, T>::value)
						{
							for (auto& iter : iterOneDArray)
								resultinter.push_back(StringHelper::UTF8ToUnicode(iter.GetString()));
						}
						// ������. ����.
						result2DArray.push_back(resultinter);
					}
				}
			}
			else if (m_isArray == true && valueIsId == false) // ������ 2�����迭�̰�, index�� ã�´ٸ�..
			{
				auto valueArray = m_document[indexOrId][keystring.c_str()].GetArray();

				for (auto& oneDArray : valueArray)
				{
					auto iterOneDArray = oneDArray.GetArray();
					std::vector<T> resultinter;
					if constexpr (std::is_same<int, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetInt());
					}

					else if constexpr (std::is_same<float, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetFloat());
					}
					else if constexpr (std::is_same<bool, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetBool());
					}
					else if constexpr (std::is_same<double, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetDouble());
					}
					else if constexpr (std::is_same<std::string, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetString());
					}
					else if constexpr (std::is_same<std::wstring, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(StringHelper::UTF8ToUnicode(iter.GetString()));
					}
					result2DArray.push_back(resultinter);
				}				
			}
			else
			{
				auto valueArray = m_document[keystring.c_str()].GetArray();
				for (auto& oneDArray : valueArray)
				{
					auto iterOneDArray = oneDArray.GetArray();
					std::vector<T> resultinter;
					if constexpr (std::is_same<int, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetInt());
					}

					else if constexpr (std::is_same<float, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetFloat());
					}
					else if constexpr (std::is_same<bool, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetBool());
					}
					else if constexpr (std::is_same<double, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetDouble());
					}
					else if constexpr (std::is_same<std::string, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(iter.GetString());
					}
					else if constexpr (std::is_same<std::wstring, T>::value)
					{
						for (auto& iter : iterOneDArray)
							resultinter.push_back(StringHelper::UTF8ToUnicode(iter.GetString()));
					}
					result2DArray.push_back(resultinter);
				}				
			}
			return result2DArray;

		}


	};
}
