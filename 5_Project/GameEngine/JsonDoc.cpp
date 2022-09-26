#include "pch.h"
#include "JsonDoc.h"
#include "./rapidjson/prettywriter.h"		// for stringify JSON
#include "./rapidjson/filereadstream.h"	// fopen������� ����..
#include "./rapidjson/filewritestream.h"
#include "./rapidjson/error/error.h"		// �����Ǵ��� ���ؼ�
#include "./rapidjson/error/en.h"
#include "./rapidjson/encodedstream.h"    // AutoUTFInputStream Json���� ���ڵ� �ؼ��� ���ؼ�..
#include "./rapidjson/pointer.h"

using namespace GameEngineSpace;
using namespace rapidjson;

const int kReadBufferSize = 15000;

bool JsonDoc::LoadJsonFromFile(std::string const& file)
{
	errno_t err = fopen_s(&fp_, file.c_str(), "rb");
	if (err) {
		return false;
	}
	char readBuffer[kReadBufferSize];

	FileReadStream readStream{ fp_, readBuffer, sizeof(readBuffer) };

	// ���� ���� json������ ������ rapid �⺻�� �ٸ��� ����� BOM�� ����. �װ� ó������.
	AutoUTFInputStream<unsigned, FileReadStream> eis(readStream);  

	ParseResult result = m_document.ParseStream<0, AutoUTF<unsigned> >(eis);

	if (!result)
	{
		printf("JSON parse error: %s (%u)\n", GetParseError_En(result.Code()), (UINT)result.Offset());
		exit(1);
		return false;
	}

	// ������ 2���� ���������Ʈ���� ���, ���⼭ �����͸� �����̳ʿ� �־��
	if (m_document.IsArray())
	{
		int _id;
		// ������� ������Ʈ�� �����ͼ�..
		for (SizeType i = 0; i < m_document.Size(); i++)
		{
			// ID�� ���� ������Ʈ�� json�����͸� ������ �����̳ʿ� �־��ش�.
			_id = m_document[i]["ID"].GetInt();
			// RapidJson�� Pointer�� /Ű��, Ȥ��index ������ ���ڿ��̴�.
			std::string _pointerToken = "/" + std::to_string(i);
			Value* pObj = Pointer(_pointerToken.c_str()).Get(m_document);
			m_pArrayedJsonValues.insert({ _id, pObj });
		}
		m_isArray = true;
	}

	fclose(fp_);

	return true;
}

Document& JsonDoc::GetDocument()
{
	return m_document;
}
