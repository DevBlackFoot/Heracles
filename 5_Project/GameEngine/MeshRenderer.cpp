#include "pch.h"
#include "MeshRenderer.h"

#include "Camera.h"
#include "GameObject.h"
#include "GraphicsManager.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Transform.h"

GameEngineSpace::MeshRenderer::MeshRenderer(std::weak_ptr<GameObject> object)
	: RendererBase(object)
	, isPBR(true)
{
}

void GameEngineSpace::MeshRenderer::Init(const std::string& modelName, const std::string& path, ModelType type, bool PBR)
{
	// PBR ����
	isPBR = PBR;

	// ���⼭ �������� �������ش�..!
	using namespace GraphicsEngineSpace;

	// ���⼭ �޾ƿ� �̸��� ���ӿ�����Ʈ�� �̸����� ���ش�. => json �Ľ��� ���� �� �κ����� �ű�.
	/*tstring tstr;

	tstr.assign(modelName.begin(), modelName.end());

	GetGameObject()->SetName(tstr);*/


	// �� �κп� ���Ǵ� ������ ������ ���� �����Ǵ� ������ ������ �ƴ϶�
	// ���� �ִ� ������ ������ �����ͼ� ����ϴ� ���̱� ������.. ����Ʈ �����͸� ����ϸ� �ȵȴ�.
	Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

	ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

	ModelBase* model = resourceManager->GetModel(modelName);

	if (model == nullptr)
		model = factory->CreateModelFromASEFile(modelName, path);

	if (isPBR == true)
	{
		PBRModel = resourceManager->GetPBRModel(modelName + "PBR");

		if (PBRModel == nullptr)
		{
			switch (type)
			{
			case ModelType::ALBEDO_ONLY: PBRModel = factory->CreateAlbedoModel(modelName + "PBR", model); break;
			case ModelType::NONE_RMA: PBRModel = factory->CreateAlbedoNormalModel(modelName + "PBR", model); break;
			case ModelType::NONE_AO: PBRModel = factory->CreateWithoutAOStaticModel(modelName + "PBR", model); break;
			case ModelType::ALL: PBRModel = factory->CreateAllTextureStaticModel(modelName + "PBR", model); break;
			}
		}

		PBRModel->SetAlpha(1.0f);
		PBRModel->SetMetallic(0.0f);
		PBRModel->SetRoughness(0.7f);
		prefab = PBRModel->GetPrefab();
	}
	else
	{
		prefab = resourceManager->GetPrefab(modelName + "LegacyPrefab");

		if (prefab == nullptr)
			prefab = factory->CreatePrefab(modelName + "LegacyPrefab", model, resourceManager->GetBuffer("MatrixCB"), 0);

		prefab->SetVertexShader(resourceManager->GetShader("LegacyModelVS"));
		prefab->SetPixelShader(resourceManager->GetShader("LegacyModelPS"));
		prefab->SetMaterialBuffer({ resourceManager->GetBuffer("LegacyMaterialCB"), 2, &material, ShaderType::PIXEL });

		struct LegacyVertex
		{
			HeraclesMath::Vector position;
			HeraclesMath::Vector normal;
			HeraclesMath::Vector texCoord;
			HeraclesMath::Vector tangent;
		};

		prefab->CreateVertexBuffer<LegacyVertex>(factory, [](const GraphicsEngineSpace::VertexData& data)-> LegacyVertex
			{
				LegacyVertex temp;
				temp.position = data.position;
				temp.normal = data.normal;
				temp.texCoord = { data.texCoord.x, data.texCoord.y, 0.f, 0.f };
				temp.tangent = data.tangent;

				return temp;
			});
	}
}

void GameEngineSpace::MeshRenderer::InitPlane(bool PBR)
{
	// PBR ����
	isPBR = PBR;

	// ���⼭ �������� �������ش�..!
	using namespace GraphicsEngineSpace;

	// �� �κп� ���Ǵ� ������ ������ ���� �����Ǵ� ������ ������ �ƴ϶�
	// ���� �ִ� ������ ������ �����ͼ� ����ϴ� ���̱� ������.. ����Ʈ �����͸� ����ϸ� �ȵȴ�.
	Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

	ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

	// ���� �⺻������ ť��� �Ѵ�.
	ModelBase* model = resourceManager->GetModel("PlaneCube");

	if (model == nullptr)
		model = factory->CreateModelFromASEFile("PlaneCube", "Resources/Model/PlaneCube.ase");

	if (isPBR == true)
	{
		PBRModel = resourceManager->GetPBRModel("PlaneCubePBR");

		if (PBRModel == nullptr)
			PBRModel = factory->CreateAlbedoModel("PlaneCubePBR", model);

		PBRModel->SetAlpha(1.0f);
		PBRModel->SetMetallic(0.1f);
		PBRModel->SetRoughness(1.0f);
		prefab = PBRModel->GetPrefab();
	}
	else
	{
		prefab = resourceManager->GetPrefab("PlaneCubeLegacyPrefab");

		if (prefab == nullptr)
			prefab = factory->CreatePrefab("PlaneCubeLegacyPrefab", model, resourceManager->GetBuffer("MatrixCB"), 0);

		prefab->SetVertexShader(resourceManager->GetShader("LegacyModelVS"));
		prefab->SetPixelShader(resourceManager->GetShader("LegacyModelPS"));
		prefab->SetMaterialBuffer({ resourceManager->GetBuffer("LegacyMaterialCB"), 2, &material, ShaderType::PIXEL });

		struct LegacyVertex
		{
			HeraclesMath::Vector position;
			HeraclesMath::Vector normal;
			HeraclesMath::Vector texCoord;
			HeraclesMath::Vector tangent;
		};

		prefab->CreateVertexBuffer<LegacyVertex>(factory, [](const GraphicsEngineSpace::VertexData& data)-> LegacyVertex
			{
				LegacyVertex temp;
				temp.position = data.position;
				temp.normal = data.normal;
				temp.texCoord = { data.texCoord.x, data.texCoord.y, 0.f, 0.f };
				temp.tangent = data.tangent;

				return temp;
			});
	}

	// �ش� �κ� �ٲ��� �� ������ �ٲٱ�.. => ���̽����� ����������..
	/*transform->SetPosition({0.f, -1.f, 0.f});
	transform->SetScale({100.f, 1.0f, 100.f});*/
}

void GameEngineSpace::MeshRenderer::SetMetallic(float metallic)
{
	if (PBRModel == nullptr)
		return;

	PBRModel->SetMetallic(metallic);
}

void GameEngineSpace::MeshRenderer::SetRoughness(float roughness)
{
	if (PBRModel == nullptr)
		return;

	PBRModel->SetRoughness(roughness);
}

void GameEngineSpace::MeshRenderer::PrepareRender(float tick)
{
	prefab->PrepareRender(transform->GetWorldTM(), tick);
}

void GameEngineSpace::MeshRenderer::Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine> graphicsEngine)
{
	using namespace GraphicsEngineSpace;

	// ���ҽ��Ŵ��� ����
	/*std::unique_ptr<ResourceManager> resourceManager;
	resourceManager.reset(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager());*/

	// ���ҽ� �Ŵ��� ���� ���ư��� �ȵǱ� ������ �Ϲ� �������� ������ ����!
	ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

	std::shared_ptr<Camera> mainCamera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
	// ViewProjection
	GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugBeginEvent("ViewProjection");
	Matrix viewProjection = MatrixTranspose(mainCamera->GetViewMatrix() * mainCamera->GetProjMatrix());
	resourceManager->GetBuffer("ViewProjectionCB")->SetUpBuffer(1, &viewProjection, ShaderType::VERTEX);
	GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugEndEvent();

	// View World
	struct viewPos
	{
		Vector viewPosition;
	} cbViewPos;

	cbViewPos.viewPosition = mainCamera->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

	resourceManager->GetBuffer("ViewWorldPosCB")->SetUpBuffer(0, &cbViewPos, ShaderType::PIXEL);

	// Light
	GraphicsManager::GetInstance()->GetDirectionalLight()->SetUpBuffer(1, ShaderType::PIXEL);

	// Sampler;
	GraphicsManager::GetInstance()->GetDirectionalLight()->SetUpBuffer(1, ShaderType::PIXEL);
	resourceManager->GetSampler("LinearSampler")->SetUpSampler(0, ShaderType::PIXEL);

	auto materials = prefab->GetMaterials();

	for (auto iter : materials)
	{
		auto materialData = iter.second->GetMaterialData();
		auto maps = materialData.maps;

		material.ambient = materialData.ambient;
		material.diffuse = materialData.diffuse;
		material.specular = materialData.specular;

		for (int i = 0; i < maps.size(); i++)
		{
			if (maps[i].type == MapType::DIFFUSE)
				maps[i].map->SetUpTexture(0, ShaderType::PIXEL);
			else if (maps[i].type == MapType::NORMAL)
				maps[i].map->SetUpTexture(1, ShaderType::PIXEL);
		}
		break;
	}

	std::string objName;

	objName.assign(GetGameObject()->GetName().begin(), GetGameObject()->GetName().end());

	GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugBeginEvent(objName);
	//prefab->Render(graphicsEngine.get(), transform->GetWorldTM(), Timer::GetInstance()->DeltaTime());
	prefab->Render(graphicsEngine.get());
	GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugEndEvent();
}
