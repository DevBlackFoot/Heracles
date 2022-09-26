#pragma once
// ��� ������Ʈ�� ������Ʈ ���̽��� ��ӹ޽��ϴ�.
#include "ComponentBase.h"

namespace GameEngineSpace
{
	class GameObject;

	enum class Space
	{
		WORLD,
		LOCAL
	};

	/**
	 * \brief ��� ���� ������Ʈ�� �ݵ�� ������ �ִ� Ʈ������ ������Ʈ �Դϴ�.
	 *
	 * ������Ʈ�� ��ġ, ����, ������ ������ ������ �ֽ��ϴ�.
	 *
	 * 2022.07.22(��) Dev.BlackFoot
	 */
	class Transform : public ComponentBase
	{
		// ���� ��ġ ���� ������
		// Transform Matrix
			// Local TM == �θ��� �������� �θ� ���������� �ڱ� ��ġ�� �̵���Ű�� TM / LocalTM
			// World TM == ���� �������� World ��ġ�� ���� TM
		Matrix localTM;
		Matrix worldTM;

		// �⺻������ ���������� Vector�� w���� ���� ����ִ�..
		Vector localPosition;
		Vector localRotation;
		Vector localScale;

		Vector worldPosition;
		Vector worldRotation;
		Vector worldScale;

		// ������������, ��, �� ������ ����
		Vector look;
		Vector up;
		Vector right;

		// TM ������Ʈ�� ���� ���� ���� �� �Լ�.
		// ���� �ڽ��� TM�� ���� ������ϴ°�.
		bool needTMUpdate;

	public:
		Transform(std::weak_ptr<GameObject> _gameObject);

		virtual ~Transform();

		virtual void Release() override;

		virtual void Update(float tick) override;

		// ������ ��ġ ������, ����� �Ǵ� ������ �������ݴϴ�.
		void SetPosition(const Vector& _newPos, Space relativeTo = Space::WORLD);
		void SetRotation(const Vector& _eulerAngle, Space relativeTo = Space::WORLD);
		void AddRotation(const Vector& _eulerAngle, Space relativeTo = Space::WORLD);
		// Scale�� ��� ���带 ��������� �ʽ��ϴ�..
		void SetScale(const Vector& _newLocalScale);

		// ������ Private ���� ����.
		const Vector& GetLocalPosition() { return localPosition; }
		const Vector& GetLocalRotation() { return localRotation; }
		const Vector& GetLocalScale() { return localScale; }

		const Vector& GetWorldPosition() { return worldPosition; }
		const Vector& GetWorldRotation() { return worldRotation; }
		const Vector& GetWorldScale() { return worldScale; }


		void LookAt(const Vector& targetPos);

		const Vector& GetLook() { return look; }
		void SetLook(const Vector& _look);
		const Vector& GetUp() { return up; }
		void SetUp(const Vector& _up);
		const Vector& GetRight() { return right; }
		void SetRight(const Vector& _right);

		const Matrix& GetLocalTM();
		const Matrix& GetWorldTM();

		void SetLocalTM(const Matrix& _localTM);
		void SetWorldTM(const Matrix& _worldTM);

	private:
		// ���� TM�� ����
		void UpdateLocalTM();
		// WorldTM�� ����
		void UpdateWorldTM();
		// needTM UPdate ���� True�� ����
		void CheckingNeedTMUpdate();

		// Decompose
		void DecomposeMatrix(Vector& scale, Vector& rotation, Vector& position, Matrix matrix);
	};
}

