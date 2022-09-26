#include "pch.h"
#include "SkinnedMeshRenderer.h"

#include "Animator.h"
#include "Camera.h"
#include "GameObject.h"
#include "GraphicsManager.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Transform.h"

namespace GameEngineSpace
{

	SkinnedMeshRenderer::SkinnedMeshRenderer(std::weak_ptr<GameObject> gameObj)
		: RendererBase(gameObj)
		, model(nullptr)
		, PBRModel(nullptr)
		, isPBR(true)
		, totalAnimationTime(0.0f)
	{

	}

	void SkinnedMeshRenderer::Init(const std::string& modelName, const std::string& path, ModelType type, bool PBR)
	{
		// PBR ����
		isPBR = PBR;

		// ������ ����.
		using namespace GraphicsEngineSpace;

		// ���⼭ �޾ƿ� �̸��� ���� ������Ʈ�� �̸����� �������ش�. => json �Ľ̿��� ���ش�.
		/*tstring tstr;
		tstr.assign(modelName.begin(), modelName.end());

		GetGameObject()->SetName(tstr);*/

		// �׷��Ƚ� �������� ���丮�� ���ҽ� �Ŵ����� �����ɴϴ�.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		// �� �ߺ� üũ
		model = resourceManager->GetModel(modelName);
		// ���� ������ ������ݴϴ�
		if (model == nullptr)
		{
			model = factory->CreateModelFromASEFile(modelName, path);
			type = ModelType::NONE_RMA;
		}

		// �� �������� PBR �б�.
		if (isPBR == true)
		{
			switch (type)
			{
			case ModelType::ALBEDO_ONLY: PBRModel = factory->CreateSkinningAlbedoModel(modelName + "PBR", model); break;
			case ModelType::NONE_RMA: PBRModel = factory->CreateSkinningAlbedoNormalModel(modelName + "PBR", model); break;
			case ModelType::NONE_AO: PBRModel = factory->CreateWithoutAOModel(modelName + "PBR", model); break;
			case ModelType::ALL: PBRModel = factory->CreateAllTextureModel(modelName + "PBR", model); break;
			}

			PBRModel->SetAlpha(1.0f);
			PBRModel->SetMetallic(0.1f);
			PBRModel->SetRoughness(0.8f);

			prefab = PBRModel->GetPrefab();
		}
		else
		{
			// ������ �ߺ� üũ. ������ ������ݴϴ�.
			prefab = resourceManager->GetPrefab(modelName + "SkinPrefab");
			if (prefab == nullptr)
				prefab = factory->CreatePrefab(modelName + "SkinPrefab", model, resourceManager->GetBuffer("MatrixCB"), 0);

			// ���ؽ� ���̴��� �ȼ� ���̴��� �������ݴϴ�.
				// ���ؽ� ���̴��� ��Ű�� ���̴�, �ȼ� ���̴��� ���Ž� ���̴��� �״�� ����մϴ�.
			prefab->SetVertexShader(resourceManager->GetShader("SkinningModelVS"));
			prefab->SetPixelShader(resourceManager->GetShader("LegacyModelPS"));
			// ���׸��� ���۸� �����մϴ�. ���׸��� ���� ���� ���Žø� �״�� ����մϴ�.
			prefab->SetMaterialBuffer({ resourceManager->GetBuffer("LegacyMaterialCB"), 2, &material, ShaderType::PIXEL });

			// �� ��Ʈ������ �ʱ�ȭ ���� �ݴϴ�.
			ConstantBufferSetting boneMatrixElements;
			boneMatrixElements.buffer = resourceManager->GetBuffer("BoneMatrixCB");
			boneMatrixElements.data = model->GetBoneMatrix();
			boneMatrixElements.slot = 3;
			boneMatrixElements.type = ShaderType::VERTEX;
			prefab->AddOnceBuffer(boneMatrixElements);

			// ��Ű�� ���ؽ� ����ü�� ������ݴϴ�.
			struct SkinnedVertex
			{
				Vector position;
				Vector normal;
				Vector texCoord;
				Vector tangent;
				Vector weights1;
				Vector weights2;
				unsigned int weightsIndex1;
				unsigned int weightsIndex2;
			};

			// ���ؽ� ���� ����
			prefab->CreateVertexBuffer<SkinnedVertex>(factory, [](const VertexData& data)-> SkinnedVertex
				{
					SkinnedVertex temp;
					temp.position = data.position;
					temp.normal = data.normal;
					temp.texCoord = { data.texCoord.x, data.texCoord.y, 0.f, 0.f };
					temp.tangent = data.tangent;
					temp.weights1 = data.weights1;
					temp.weights2 = data.weights2;
					temp.weightsIndex1 = data.weightIndex1;
					temp.weightsIndex2 = data.weightIndex2;

					return temp;
				});
		}


		// ����׿� �� �׸�.
		Vector vertices[8] =
		{
			{-100.0f, 200.0f, +100.0f, 1.0f},
			{+100.0f, 200.0f, +100.0f, 1.0f},
			{+100.0f, 200.0f, -100.0f, 1.0f},
			{-100.0f, 200.0f, -100.0f, 1.0f},

			{-100.0f, 0.0f, +100.0f, 1.0f},
			{+100.0f, 0.0f, +100.0f, 1.0f},
			{+100.0f, 0.0f, -100.0f, 1.0f},
			{-100.0f, 0.0f, -100.0f, 1.0f},
		};

		WORD indices[24] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			0, 4,
			1, 5,
			2, 6,
			3, 7,
			4, 5,
			5, 6,
			6, 7,
			7, 4
		};

		factory->CreateVertexBuffer("ObjLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 8);
		factory->CreateIndexBuffer("ObjLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 24);
	}

	void GameEngineSpace::SkinnedMeshRenderer::SetMetallic(float metallic)
	{
		if (PBRModel == nullptr)
			return;

		PBRModel->SetMetallic(metallic);
	}

	void GameEngineSpace::SkinnedMeshRenderer::SetRoughness(float roughness)
	{
		if (PBRModel == nullptr)
			return;

		PBRModel->SetRoughness(roughness);
	}

	void SkinnedMeshRenderer::PrepareRender(float tick)
	{

		// �ִϸ��̼� Ÿ���� �������ݴϴ�.
			// �ִϸ����͸� ĳ�����ݴϴ�.
		assert(GetGameObject()->GetComponent<Animator>() != nullptr);

		std::shared_ptr<Animator> animator = GetGameObject()->GetComponent<Animator>();

		// �ִϸ����Ϳ��� �ð��� �����ɴϴ�.
		float nowTime = animator->GetTotalAnimationTime();

		// ������ PreRender
		if (prefab->PrepareRender(transform->GetWorldTM(), nowTime) == true)
		{
			if (animator->IsLoop() == true)
				animator->Play();
			else
				animator->Pause();
		}
	}

	/*void SkinnedMeshRenderer::Update(float tick)
	{
		// ������ ������Ʈ�� �ʿ��ϱ� ������ ���� �־��ش�.
		// ������ ������Ʈ�� ������.

		// �� ��`�� ������ �̽��� ����
		// Render �������� ���� - JKim

		// prefab->Update(transform->GetWorldTM(), tick);

		//// �� ������Ʈ
		//auto bones = model->GetBones();

		//for(int i = 0; i < bones.size(); i++)
		//	boneMatrix[i] = MatrixTranspose(bones[i]->GetBoneMatrix());
	}*/

	void SkinnedMeshRenderer::Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine> graphicsEngine)
	{
		using namespace GraphicsEngineSpace;

		// ViewProj
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
		resourceManager->GetSampler("LinearSampler")->SetUpSampler(0, ShaderType::PIXEL);

		// �������� �����մϴ�.

		// �׷��Ƚ� ������� ���� ������Ʈ �̸��� �޾ƿɴϴ�.
		std::string objName;

		objName.assign(GetGameObject()->GetName().begin(), GetGameObject()->GetName().end());

		GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugBeginEvent(objName);
		//prefab->Render(graphicsEngine.get(), transform->GetWorldTM(), Timer::GetInstance()->DeltaTime());
		prefab->Render(graphicsEngine.get());
		GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugEndEvent();

	}

	void SkinnedMeshRenderer::BoundingRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		/*Matrix worldPos = MatrixTranslationFromVector(transform->GetWorldPosition());
		Matrix worldRot = MatrixRotationFromVector(transform->GetWorldRotation());
		Matrix world = worldRot * worldPos;*/

		renderer->GraphicsDebugBeginEvent("Obj Line");
		renderer->DrawLine(resourceManager->GetBuffer("ObjLineVB"), resourceManager->GetBuffer("ObjLineIB"), { 1.0f, 1.0f, 0.0f }, MatrixTranspose(transform->GetWorldTM()));
		renderer->GraphicsDebugEndEvent();
	}
}
