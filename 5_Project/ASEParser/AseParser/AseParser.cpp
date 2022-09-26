/********************************
*								*
*	ASE Pareser.cpp				*
*								*
*	Created : 2022/07/06		*
*	Updated : 2022/07/06		*
*********************************/

#include "ASEParser.h"

ASEParser::ASEParser()
{

}

ASEParser::~ASEParser()
{
	if (file.is_open())
		file.close();
}

bool ASEParser::LoadASEFile(const std::string& fileName)
{
	file.open(fileName, std::ios_base::in);

	if (file.is_open() != true)
		return false;

	GetDataFromFile();
}

const SceneInfo& ASEParser::GetSceneInfo()
{
	return sceneInfo;
}

const map<int, MaterialInfo>& ASEParser::GetMaterials()
{
	return materialListInfo.materials;
}

const vector<GeometryInfo>& ASEParser::GetGeoMetries()
{
	return geometryList;
}

const vector<HelperInfo>& ASEParser::GetHelpers()
{
	return helperList;
}

const vector<ShapeInfo>& ASEParser::GetShapes()
{
	return shapeList;
}

void ASEParser::GetDataFromFile()
{
	while (file.eof() != true)
	{
		string temp;

		file >> temp;

		AseToken tokenNum = AseToken::TOKENR_END;

		if (temp[0] == '*')
			tokenNum = GetToken(temp);

		switch (tokenNum)
		{
			// File Info
		case AseToken::TOKENR_3DSMAX_ASCIIEXPORT:
		case AseToken::TOKENR_COMMENT:
		{
			fileInfo.SetData(file, tokenNum);
			break;
		}

		// Scene Info
		case AseToken::TOKENR_SCENE:
		{
			sceneInfo.SetData(file);
			break;
		}

		// Material Info
		case AseToken::TOKENR_MATERIAL_LIST:
		{
			materialListInfo.SetData(file);
			break;
		}

		// Geometry Info
		case AseToken::TOKENR_GEOMOBJECT:
		{
			GeometryInfo temp;

			temp.SetData(file);

			geometryList.push_back(temp);

			break;
		}

		// Helper Info
		case AseToken::TOKENR_HELPEROBJECT:
		{
			HelperInfo temp;

			temp.SetData(file);

			helperList.push_back(temp);

			break;
		}

		case AseToken::TOKENR_SHAPEOBJECT:
		{
			ShapeInfo temp;

			temp.SetData(file);

			shapeList.push_back(temp);

			break;
		}

		}
	}

	file.close();

	// ���ؽ� ����
	SplitVertex();
}

void ASEParser::SplitVertex()
{
	for (int k = 0; k < geometryList.size(); k++)
	{
		FaceInfo& face = geometryList[k].meshInfo.faceInfo;
		VertexInfo& vertex = geometryList[k].meshInfo.vertexInfo;
		NormalInfo& normal = geometryList[k].meshInfo.normalInfo;
		TextureInfo& texture = geometryList[k].meshInfo.textureInfo;

		for (unsigned int i = 0; i < face.facesSize; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int index = face.faces[i].index[j + 1];

				// ó�� ���Ǵ� ���ؽ��� ��
				if (vertex.vertices[index].next == -1)
				{
					// key���� -2�� �����Ͽ� ���� ���ؽ��� ���� ǥ��
					vertex.vertices[index].next = -2;

					// �ش� ���ؽ����� ����� �븻, �ؽ��� ����
					if (normal.normals.size() > i)
					{
						for (int l = 0; l < 4; l++)
						{
							vertex.vertices[index].normal[l] = normal.normals[i].vertexNormal[j][l];
							vertex.vertices[index].weightIndex[l] = 0;
							vertex.vertices[index].weightIndex[l + 4] = 0;
							vertex.vertices[index].weightIndex[l + 8] = 0;
							vertex.vertices[index].weightIndex[l + 12] = 0;
							vertex.vertices[index].weights[l] = 0.0f;
							vertex.vertices[index].weights[l + 4] = 0.0f;
							vertex.vertices[index].weights[l + 8] = 0.0f;
							vertex.vertices[index].weights[l + 12] = 0.0f;
						}
					}
					else
					{
						for (int l = 0; l < 4; l++)
						{
							vertex.vertices[index].normal[l] = 0.0f;
							vertex.vertices[index].weightIndex[l] = 0;
							vertex.vertices[index].weightIndex[l + 4] = 0;
							vertex.vertices[index].weightIndex[l + 8] = 0;
							vertex.vertices[index].weightIndex[l + 12] = 0;
							vertex.vertices[index].weights[l] = 0.0f;
							vertex.vertices[index].weights[l + 4] = 0.0f;
							vertex.vertices[index].weights[l + 8] = 0.0f;
							vertex.vertices[index].weights[l + 12] = 0.0f;
						}
					}

					vertex.vertices[index].textureIndex = face.faces[i].tIndex[j + 1];
					vertex.vertices[index].materialID = face.faces[i].materialID;

					// �ش� ���ؽ����� ����� ����ġ ����
					vector<WeightInfo>& weightList = geometryList[k].boneInfo.weightList;
					if (weightList.size() > index)
					{
						for (int k = 0; k < weightList[index].weights.size(); k++)
						{
							vertex.vertices[index].weightIndex[k] = weightList[index].weights[k].index;
							vertex.vertices[index].weights[k] = weightList[index].weights[k].weight;
						}
					}

					continue;
				}

				// ���ؽ� �߰��� �ʿ����� Ȯ���� �÷���
				bool flag = true;
				float prev = index;

			Retry:
				while (index != -2)
				{
					prev = index;
					// ���ؽ� ��ȸ�ؼ� �ؽ�ó �ߺ� Ȯ��
					if (vertex.vertices[index].textureIndex != face.faces[i].tIndex[j + 1])
					{
						index = vertex.vertices[index].next;

						continue;
					}

					// ���ؽ� ��ȸ�ؼ� �븻�� Ȯ��
					for (int h = 1; h < 4; h++)
					{
						if (vertex.vertices[index].normal[h] != normal.normals[i].vertexNormal[j][h])
						{
							index = vertex.vertices[index].next;

							goto Retry;
						}
					}

					if (vertex.vertices[index].materialID != face.faces[i].materialID)
					{
						index = vertex.vertices[index].next;

						goto Retry;
					}

					// ���⿡ ���´ٸ� �ؽ��Ŀ� �븻�� ���� ���ؽ��� �ִٴ� ��
					flag = false;

					break;
				}

				// �ߺ����� �ʴ� ���ؽ��� ��� ���� �߰�
				if (flag == true)
				{
					vertex.vertices.push_back(vertex.vertices[prev]);

					if (normal.normals.size() > i)
					{
						for (int l = 0; l < 4; l++)
							vertex.vertices.back().normal[l] = normal.normals[i].vertexNormal[j][l];
					}
					else
					{
						for (int l = 0; l < 4; l++)
							vertex.vertices.back().normal[l] = 0.0f;
					}
					vertex.vertices.back().textureIndex = face.faces[i].tIndex[j + 1];

					vertex.vertices[prev].next = static_cast<unsigned int>(vertex.vertices.size()) - 1;
					face.faces[i].index[j + 1] = static_cast<unsigned int>(vertex.vertices.size()) - 1;
				}
				else
				{
					face.faces[i].index[j + 1] = prev;
				}
			}
		}

		// ���ؽ� ũ�� ����
		vertex.verticesSize = static_cast<unsigned int>(vertex.vertices.size());
	}
}

ASEParserDeclSpec ASEParser* CreateASEParser()
{
	return new ASEParser;
}