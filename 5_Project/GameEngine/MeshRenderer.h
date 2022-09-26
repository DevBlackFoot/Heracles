#pragma once
#include "RendererBase.h"

namespace GameEngineSpace
{
	/**
	 * \brief ���Ž� ���� Ȱ���� ������ ������Ʈ
	 *
	 * ���ؽ� ���� // ������, �븻, TexCoord, Tangent�� ��
	 *
	 * 2022.08.02(ȭ) Dev.BlackFoot
	 */
	class MeshRenderer : public RendererBase
	{
	private:
		// �������� ����ϴ� ���׸��� ������ ��� �ִ� ���׸��� ����
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
		// ���⼭ �ʱ�ȭ �� ��. ������Ʈ �̸��� ASE Path�� �־��ش�.
		void Init(const std::string& modelName, const std::string& path, ModelType type, bool PBR = true);

		// �÷������� �ʱ�ȭ �� �� �ְ� ������.
			// ��������Ʈ�� ����ϰ� �ؽ��ĸ� �־�����.
			// TODO transform ����.
		void InitPlane(bool PBR = true);

		void SetMetallic(float metallic);
		void SetRoughness(float roughness);

		void PrepareRender(float tick) override;
		void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine>graphicsEngine) override;
	};
}

