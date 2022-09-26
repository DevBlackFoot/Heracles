#pragma once
#include "RendererBase.h"

namespace GameEngineSpace
{
	/**
	 * \brief 레거시 모델을 활용한 렌더러 컴포넌트
	 *
	 * 버텍스 정보 // 포지션, 노말, TexCoord, Tangent가 들어감
	 *
	 * 2022.08.02(화) Dev.BlackFoot
	 */
	class MeshRenderer : public RendererBase
	{
	private:
		// 렌더에서 사용하는 머테리얼 정보를 담고 있는 머테리얼 변수
		struct MaterialCB
		{
			HeraclesMath::Vector ambient;
			HeraclesMath::Vector diffuse;
			HeraclesMath::Vector specular;
		} material;

		GraphicsEngineSpace::PBRModel* PBRModel;

		bool isPBR;

	public:
		MeshRenderer(std::weak_ptr<GameObject> object);

		virtual ~MeshRenderer() {}

	public:
		// 여기서 초기화 할 때. 오브젝트 이름과 ASE Path를 넣어준다.
		void Init(const std::string& modelName, const std::string& path, ModelType type, bool PBR = true);

		// 플레인으로 초기화 할 수 있게 만들어보자.
			// 스프라이트와 비슷하게 텍스쳐를 넣어주자.
			// TODO transform 지정.
		void InitPlane(bool PBR = true);

		void SetMetallic(float metallic);
		void SetRoughness(float roughness);

		void PrepareRender(float tick) override;
		void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine>graphicsEngine) override;
	};
}

