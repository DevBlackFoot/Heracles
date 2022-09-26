#pragma once
#include "ComponentBase.h"
#include "../GraphicsEngine/GraphicsEngineFramework.h"
#include "BoundingVolumes.h"

class IDXObject;

namespace GameEngineSpace
{
	class GameObject;
	class Transform;

	/**
	 * \brief 렌더링 가능한 게임 오브젝트들에 붙는 렌더러 컴포넌트들의 기본이 되는 렌더러 베이스 컴포넌트 입니다.
	 *
	 * Foot 엔진에서는 빌더의 정보를 가지고 있으면 될 것 같습니다..
	 *
	 * 2022.07.26(화) Dev.BlackFoot
	 */
	class RendererBase : public ComponentBase, public std::enable_shared_from_this<RendererBase>
	{
	public:
		enum class ModelType
		{
			ALBEDO_ONLY,
			NONE_RMA,
			NONE_AO,
			ALL
		};

	protected:
		// 그래픽스 엔진측으로 보내줄 정보
			// => 어떤 셰이더를 사용할 것인지, 버텍스 정보, 인덱스 정보 => ASE 파싱한 데이터 그 자체..?
			// 혹은 ASE 파일 path. 그래픽스 내부에서 ASE를 파싱해서 가지고 있는 쪽으로..
		// 프리팹데이터를 가지고 있자
		// 실질적으로 그림에 쓰이는 요소..! 프리팹.
		// 우리의 그래픽스 레퍼런스는 지훈이 형이 관리한다.
		GraphicsEngineSpace::PrefabBase* prefab;

		// 트랜스폼
		std::shared_ptr<Transform> transform;

		// 프러스텀 컬링을 위한 BoundingBox..
		std::shared_ptr<BoundingCube> boundingCube;

	public:
		RendererBase(std::weak_ptr<GameObject> _object);

		virtual ~RendererBase();

		void Release() override;
		
		void Awake() override;

		//virtual void Render() abstract;
		virtual GraphicsEngineSpace::PrefabBase* GetPrefab() { return prefab; }
		virtual void PrepareRender(float tick) abstract;
		virtual void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine>graphicsEngine) abstract;
		virtual void BoundingRender() {}
	};
}

