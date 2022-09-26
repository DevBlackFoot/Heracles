#include "pch.h"
#include "JsonDoc.h"
#include "./rapidjson/prettywriter.h"		// for stringify JSON
#include "./rapidjson/filereadstream.h"	// fopen입출력을 위해..
#include "./rapidjson/filewritestream.h"
#include "./rapidjson/error/error.h"		// 에러판단을 위해서
#include "./rapidjson/error/en.h"
#include "./rapidjson/encodedstream.h"    // AutoUTFInputStream Json파일 인코딩 해석을 위해서..
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

	// 보통 쓰는 json파일의 형식이 rapid 기본과 다르게 헤더에 BOM이 들어간다. 그걸 처리해줌.
	AutoUTFInputStream<unsigned, FileReadStream> eis(readStream);  

	ParseResult result = m_document.ParseStream<0, AutoUTF<unsigned> >(eis);

	if (!result)
	{
		printf("JSON parse error: %s (%u)\n", GetParseError_En(result.Code()), (UINT)result.Offset());
		exit(1);
		return false;
	}

	// 문서가 2차원 스프레드시트형일 경우, 여기서 포인터를 컨테이너에 넣어둠
	if (m_document.IsArray())
	{
		int _id;
		// 순서대로 오브젝트를 가져와서..
		for (SizeType i = 0; i < m_document.Size(); i++)
		{
			// ID와 각각 오브젝트의 json포인터를 가져와 컨테이너에 넣어준다.
			_id = m_document[i]["ID"].GetInt();
			// RapidJson의 Pointer는 /키값, 혹은index 형태의 문자열이다.
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
