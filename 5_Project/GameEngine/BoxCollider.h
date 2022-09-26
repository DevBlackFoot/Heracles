#pragma once
#include "ColliderBase.h"

namespace GameEngineSpace
{

	class SphereCollider;

	/**
	 * \brief �и��� �̷��� �̿�, OBB�� �ڽ� �浹�� �˻��ϴ� �ݶ��̴� Ŭ����
	 *
	 * ref by CometEngine
	 *
	 * 2022.08.04(��) Dev.BlackFoot
	 */
	class BoxCollider : public ColliderBase
	{
	private:
		// �⺻������ ���� �������� ���� ��ġ�� �˱� ���ؼ� �����̼� ����� �ʿ��ϴ�.
			// ȸ���� ������ü�� �浹�� �˻��ϱ� ����.
		Matrix objWorldRotTM;
		// ���ð� ������ ����
		Vector localCenter;
		Vector worldCenter;

		// �и������ ���� => �˻縦 ������ ����� �븻 ����.
		std::vector<Vector> worldNormals;

		// �ڽ� �ϳ��� ������ �ִ� ������ == radius
		Vector size;

		// �� ���� ������ �ִ� ������ ũ�� => �� ���Ϳ��� �󸶸�ŭ �Ÿ��� �������ִ°�..
		std::vector<float> extents;

	public:
		BoxCollider(std::weak_ptr<GameObject> gameObj);
		virtual ~BoxCollider();

		void Release() override;

	private:
		// �켱 �ڽ����� �浹��...
		bool CheckForBox(std::shared_ptr<BoxCollider> other);

		// ���Ŀ� �ڵ� �����ؼ� �� �浹��..
		bool CheckForSphere(std::shared_ptr<SphereCollider> other);

	public:
		// Getter Setter
		const Vector& GetLocalCenter() { return localCenter; }
		const Vector& GetWorldCenter() { return worldCenter; }
		const Vector& GetSize() { return size; }

		void SetLocalCenter(const Vector& newCenter) { localCenter = newCenter; }
		void SetSize(const Vector& newSize);

		// �浹 üũ
		bool CheckCollision(std::shared_ptr<ColliderBase> other) override;

		// �ݶ��̴� ����
		void ColliderRender() override;

		void LateUpdate(float tick) override;

		friend class PhysicsManager;

		friend class SphereCollider;
		friend class RayCollider;
	};

}
