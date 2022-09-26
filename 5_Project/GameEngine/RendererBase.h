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
	 * \brief ������ ������ ���� ������Ʈ�鿡 �ٴ� ������ ������Ʈ���� �⺻�� �Ǵ� ������ ���̽� ������Ʈ �Դϴ�.
	 *
	 * Foot ���������� ������ ������ ������ ������ �� �� �����ϴ�..
	 *
	 * 2022.07.26(ȭ) Dev.BlackFoot
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
		// �׷��Ƚ� ���������� ������ ����
			// => � ���̴��� ����� ������, ���ؽ� ����, �ε��� ���� => ASE �Ľ��� ������ �� ��ü..?
			// Ȥ�� ASE ���� path. �׷��Ƚ� ���ο��� ASE�� �Ľ��ؼ� ������ �ִ� ������..
		// �����յ����͸� ������ ����
		// ���������� �׸��� ���̴� ���..! ������.
		// �츮�� �׷��Ƚ� ���۷����� ������ ���� �����Ѵ�.
		GraphicsEngineSpace::PrefabBase* prefab;

		// Ʈ������
		std::shared_ptr<Transform> transform;

		// �������� �ø��� ���� BoundingBox..
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

