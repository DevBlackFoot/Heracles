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
		// PBR 세팅
		isPBR = PBR;

		// 프리팹 세팅.
		using namespace GraphicsEngineSpace;

		// 여기서 받아온 이름을 게임 오브젝트의 이름으로 설정해준다. => json 파싱에서 해준다.
		/*tstring tstr;
		tstr.assign(modelName.begin(), modelName.end());

		GetGameObject()->SetName(tstr);*/

		// 그래픽스 엔진에서 팩토리와 리소스 매니저를 가져옵니다.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		// 모델 중복 체크
		model = resourceManager->GetModel(modelName);
		// 모델이 없으면 만들어줍니다
		if (model == nullptr)
		{
			model = factory->CreateModelFromASEFile(modelName, path);
			type = ModelType::NONE_RMA;
		}

		// 이 시점에서 PBR 분기.
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
			// 프리팹 중복 체크. 없으면 만들어줍니다.
			prefab = resourceManager->GetPrefab(modelName + "SkinPrefab");
			if (prefab == nullptr)
				prefab = factory->CreatePrefab(modelName + "SkinPrefab", model, resourceManager->GetBuffer("MatrixCB"), 0);

			// 버텍스 셰이더와 픽셀 셰이더를 설정해줍니다.
				// 버텍스 셰이더는 스키닝 셰이더, 픽셀 셰이더는 레거시 셰이더를 그대로 사용합니다.
			prefab->SetVertexShader(resourceManager->GetShader("SkinningModelVS"));
			prefab->SetPixelShader(resourceManager->GetShader("LegacyModelPS"));
			// 머테리얼 버퍼를 설정합니다. 머테리얼 버퍼 역시 레거시를 그대로 사용합니다.
			prefab->SetMaterialBuffer({ resourceManager->GetBuffer("LegacyMaterialCB"), 2, &material, ShaderType::PIXEL });

			// 본 매트릭스를 초기화 시켜 줍니다.
			ConstantBufferSetting boneMatrixElements;
			boneMatrixElements.buffer = resourceManager->GetBuffer("BoneMatrixCB");
			boneMatrixElements.data = model->GetBoneMatrix();
			boneMatrixElements.slot = 3;
			boneMatrixElements.type = ShaderType::VERTEX;
			prefab->AddOnceBuffer(boneMatrixElements);

			// 스키닝 버텍스 구조체를 만들어줍니다.
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

			// 버텍스 버퍼 생성
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


		// 디버그용 선 그림.
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

		// 애니메이션 타임을 관리해줍니다.
			// 애니메이터를 캐싱해줍니다.
		assert(GetGameObject()->GetComponent<Animator>() != nullptr);

		std::shared_ptr<Animator> animator = GetGameObject()->GetComponent<Animator>();

		// 애니메이터에서 시간을 가져옵니다.
		float nowTime = animator->GetTotalAnimationTime();

		// 프리팹 PreRender
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
		// 본들의 업데이트가 필요하기 때문에 여기 넣어준다.
		// 프리팹 업데이트를 해주자.

		// 모델 공`유 사용시의 이슈로 인해
		// Render 안쪽으로 수정 - JKim

		// prefab->Update(transform->GetWorldTM(), tick);

		//// 본 업데이트
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

		// 프리팹을 렌더합니다.

		// 그래픽스 디버깅을 위해 오브젝트 이름을 받아옵니다.
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
