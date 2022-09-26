#pragma once
#include "RendererBase.h"

namespace GameEngineSpace
{

	/**
	 * \brief ��Ų �Ž��� ���� ������Ʈ�� ȭ�鿡 ���� ������Ʈ
	 *
	 * ����� �ִϸ��̼��� �ٲ� �� ������ ���� �뿡 ���������� �� ���̽��� �����ؼ� �ִϸ��̼��� �ٲٴ� ���� �� ��
	 *
	 * 2022.08.04(��) Dev.BlackFoot
	 */
	class SkinnedMeshRenderer : public RendererBase
	{
	private:
		// ���� �� ���Ǵ� ���׸��� ������ ��� �ִ� ���׸��� ����
		struct MaterialCB
		{
			Vector ambient;
			Vector diffuse;
			Vector specular;
		} material;

		// �ִϸ��̼� ��ȯ�� ���ؼ��� �Ž� ������ �ش��ϴ� ���� �ʿ��ϴ�.
			// ���Ŀ� �� -> SetAnimation�� ���ؼ� ������ �ش�.
		// �׷��Ƚ� ������ �ش��ϴ� �����ʹ� �׷��Ƚ� ���� ������ ���ҽ� �Ŵ����� ����
		// ������ �Ǳ� ������ ���� ����Ʈ �����͸� ������� �ʴ´�,.
		GraphicsEngineSpace::ModelBase* model;

		// PBR�� ��
		GraphicsEngineSpace::PBRModel* PBRModel;

		// PBR ����
		bool isPBR;

		// ���� ƽ�� ����ؼ� ���� � �ִϸ��̼��� �̾Ƴ��� �ϴ����� Ȯ���մϴ�.
			// �ִϸ����Ϳ�.. ��� ������ ��������
			// �ִϸ����Ϳ����� �� ������Ʈ�� ���� ����
			// => �� ���⼭ �ѹ�, �ִϸ����Ϳ��� �ѹ� �� �ι� ������Ʈ�� �ϰ� ��
			// => ���ʷ� ���ߴ� �ӵ��� �ִϸ��̼��� ����� �� ����
			// => ���� ������Ʈ �ϳ��� �ѹ��� totolAnimationTime�� ������ֱ� ���ؼ�..
		float totalAnimationTime;

	public:
		SkinnedMeshRenderer(std::weak_ptr<GameObject> gameObj);
		virtual ~SkinnedMeshRenderer() = default;

	public:
		// �ʱ�ȭ �� �� �̸��� ASE Path, �ʱ� �ִϸ��̼� Ű ���� �־��ش�.
		/**
		 * \brief
		 * \param modelName �߰��� Ȥ�� �ҷ��� �� �̸�
		 * \param path ���� �ִ� ���� path, �ҷ��� ASE ����
		 * \param initAnimKey ��â�⿡ ������ Animation�� Ű��.
		 */
		void Init(const std::string& modelName, const std::string& path, ModelType type, bool PBR = true);

		void SetMetallic(float metallic);
		void SetRoughness(float roughness);

		// �� ��Ʈ���� ������Ʈ�� ���� prepareRender
		void PrepareRender(float tick) override;

		// ���ο��� ������ ������Ʈ �� ������ ���ش�.
		void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine> graphicsEngine) override;
		void BoundingRender() override;

		// �� �κ� ���� �ʿ�.
		GraphicsEngineSpace::ModelBase* const GetModel() { return model; }
	};

}