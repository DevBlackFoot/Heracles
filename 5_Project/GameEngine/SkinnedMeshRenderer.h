#pragma once
#include "RendererBase.h"

namespace GameEngineSpace
{

	/**
	 * \brief 스킨 매쉬를 입힌 오브젝트를 화면에 띄우는 컴포넌트
	 *
	 * 현재는 애니메이션을 바꿀 수 없지만 내일 쯤에 지훈이형의 모델 베이스를 래핑해서 애니메이션을 바꾸는 것을 할 것
	 *
	 * 2022.08.04(목) Dev.BlackFoot
	 */
	class SkinnedMeshRenderer : public RendererBase
	{
	private:
		// 렌더 시 사용되는 머테리얼 정보를 담고 있는 머테리얼 변수
		struct MaterialCB
		{
			Vector ambient;
			Vector diffuse;
			Vector specular;
		} material;

		// 애니메이션 전환을 위해서는 매시 정보에 해당하는 모델이 필요하다.
			// 이후에 모델 -> SetAnimation을 통해서 가져와 준다.
		// 그래픽스 엔진에 해당하는 포인터는 그래픽스 엔진 내부의 리소스 매니저에 의해
		// 관리가 되기 때문에 따로 스마트 포인터를 사용하지 않는다,.
		GraphicsEngineSpace::ModelBase* model;

		// PBR용 모델
		GraphicsEngineSpace::PBRModel* PBRModel;

		// PBR 여부
		bool isPBR;

		// 현재 틱을 계산해서 현재 어떤 애니메이션을 뽑아내야 하는지를 확인합니다.
			// 애니메이터에.. 들어 있으면 좋겠지만
			// 애니메이터에서는 본 업데이트를 하지 못함
			// => 즉 여기서 한번, 애니메이터에서 한번 총 두번 업데이트를 하게 됨
			// => 최초로 원했던 속도로 애니메이션을 재생할 수 없음
			// => 게임 오브젝트 하나에 한번만 totolAnimationTime을 계산해주기 위해서..
		float totalAnimationTime;

	public:
		SkinnedMeshRenderer(std::weak_ptr<GameObject> gameObj);
		virtual ~SkinnedMeshRenderer() = default;

	public:
		// 초기화 때 모델 이름과 ASE Path, 초기 애니메이션 키 값을 넣어준다.
		/**
		 * \brief
		 * \param modelName 추가할 혹은 불러올 모델 이름
		 * \param path 모델이 있는 파일 path, 불러온 ASE 파일
		 * \param initAnimKey 초창기에 지정할 Animation의 키값.
		 */
		void Init(const std::string& modelName, const std::string& path, ModelType type, bool PBR = true);

		void SetMetallic(float metallic);
		void SetRoughness(float roughness);

		// 본 매트릭스 업데이트를 위한 prepareRender
		void PrepareRender(float tick) override;

		// 내부에서 프리팹 업데이트 후 렌더를 해준다.
		void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine> graphicsEngine) override;
		void BoundingRender() override;

		// 이 부분 검증 필요.
		GraphicsEngineSpace::ModelBase* const GetModel() { return model; }
	};

}