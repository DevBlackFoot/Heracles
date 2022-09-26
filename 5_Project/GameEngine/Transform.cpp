#include "pch.h"
#include "Transform.h"

#include "GameObject.h"
#include "Camera.h"

namespace GameEngineSpace
{
	Transform::Transform(std::weak_ptr<GameObject> _gameObject) :
		ComponentBase(_gameObject, ComponentType::TRANSFORM)
		, localTM(Matrix::Identity)
		, worldTM(Matrix::Identity)
		, localPosition(Vector::Zero)
		, localRotation(Vector::Zero)
		, localScale(Vector(1.0f, 1.0f, 1.0f, 0.0f))
		, worldPosition(Vector::Zero)
		, worldRotation(Vector::Zero)
		, worldScale(Vector(1.0f, 1.0f, 1.0f, 0.0f))
		, look(Vector::UnitZ)
		, up(Vector::UnitY)
		, right(Vector::UnitX)
		, needTMUpdate(true)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Release()
	{
		__super::Release();
	}

	void Transform::Update(float tick)
	{
		this->UpdateLocalTM();
		this->UpdateWorldTM();
	}

	void Transform::UpdateLocalTM()
	{
		// ���������� Local TM�� ������ݴϴ�.
			// SRT ������ ������ָ�, R�� ��� Z Roll X Pitch Y Yaw ������ ȸ�� �մϴ�.
		localTM = MatrixScalingFromVector(localScale) * MatrixRotationFromVector(localRotation) * MatrixTranslationFromVector(localPosition);

		// ������ ����Ǿ��� ������.. TM ������Ʈ�� �ʿ��ϴٰ� üũ ���ݴϴ�.
			// ���� TM�� �����ϰ� �� �� TM ������Ʈ�� �ʿ� ���� �ʴٰ� ���ݴϴ�.
		CheckingNeedTMUpdate();

	}

	void Transform::UpdateWorldTM()
	{
		// �θ���� ���谡 �ݵ�� �ʿ�..
		// �θ��� ����TM�� �ڽ��� Local�� ������ �� => �ڽ��� WorldTM
		// �θ��� ���� ��ġ�� �ڽ��� ���� �������� ����.. �ű⼭ ���� TM ��ŭ �Ű��� ���� �ڽ��� World TM�̴�.

		// �θ� ���ͼ�.. �θ��� ���忡 �ڽ��� ������ �����ش�.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			// �θ� �����Ѵٸ�..
			// �θ��� ���忡 �ڽ��� ������ �����ݴϴ�.
			worldTM = localTM * (_parent->GetComponent<Transform>()->GetWorldTM());

			// ������ ��Ҹ� �и��ؼ� ������ ������, �����̼�, �����ǿ� �־��ݴϴ�.
			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}
		else
		{
			// �θ� ���� ���. ����TM�� �״�� WorldTM���� �־��ݴϴ�.
			worldTM = localTM;

			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}

		// look up right�� �������ݴϴ�.
		// World TM�� �������� �븻 Ʈ�������� ���ݴϴ�..
		/*Vector::TransformNormal(Vector::UnitZ, m_WorldTM, m_Look);
		Vector::TransformNormal(Vector::UnitY, m_WorldTM, m_Up);
		Vector::TransformNormal(Vector::UnitX, m_WorldTM, m_Right);*/
		right = Vector::UnitX * worldTM;
		up = Vector::UnitY * worldTM;
		look = Vector::UnitZ * worldTM;

		// �� ���� ����ȭ
		right = Vector3Normalize(right);
		up = Vector3Normalize(up);
		look = Vector3Normalize(look);

		// WorldTM�� ���� �߱� ������ TM Update�� �ʿ����� �ʽ��ϴ�..
		needTMUpdate = false;
	}


	void Transform::CheckingNeedTMUpdate()
	{
		needTMUpdate = true;

		// �� ���� ������Ʈ�� ��� �ڽĵ��� TM ������Ʈ�� �ʿ��ϵ��� ���ݴϴ�.
		// ���� ����� ������Ʈ�� TM�� ����Ǿ��� ������ �ڿ������� �ڽĵ��� TM�� ����Ǿ�� �ϱ� �����Դϴ�.

		std::vector<std::shared_ptr<GameObject>>& children = this->GetGameObject()->GetChildren();

		// ��� �ڽ��� Ʈ�������� TM Update�� �ʿ��ϵ���!
		for (auto& child : children)
		{
			child->GetComponent<Transform>()->CheckingNeedTMUpdate();
		}
	}

	void Transform::LookAt(const Vector& targetPos)
	{
		if (Vector3Equal(targetPos - GetWorldTM().r[3], Vector::Zero) == true)
			return;

		Matrix matrix = MatrixLookAtLH(GetWorldTM().r[3], targetPos, Vector::UnitY);

		right = { matrix[0][0], matrix[1][0], matrix[2][0], 0.0f };
		up = -Vector{ matrix[0][1], matrix[1][1], matrix[2][1], 0.0f };
		look = { matrix[0][2], matrix[1][2], matrix[2][2], 0.0f };

		// �� ���� ����ȭ
		right = Vector3Normalize(right);
		up = Vector3Normalize(up);
		look = Vector3Normalize(look);

		SetRotation(ConvertRotationMatrixToEuler(MatrixInverse(matrix)), Space::LOCAL);
	}

	void Transform::SetLook(const Vector& _look)
	{
		// Look�� �ٲ������ �������� �ٲ�����.
		look = _look;
		look = Vector3Normalize(look);

		// look�� ���� ���� �̿��ؼ� right�� �̾��ش�.
		right = Vector3Cross(Vector::UnitY, look);
		right = Vector3Normalize(right);

		up = Vector3Cross(look, right);
		up = Vector3Normalize(up);
	}

	// ���� �ڵ忡 ������ �ֽ��ϴ�..
		// Up ���Ϳ� Right���ͷ� ������ ���� ���͸� ���ϴ� ���� ������ �ִ°�..
	void Transform::SetUp(const Vector& _up)
	{
		up = _up;
		up = Vector3Normalize(up);

		right = Vector3Cross(up, Vector::UnitZ);
		right = Vector3Normalize(right);

		look = Vector3Cross(right, up);
		look = Vector3Normalize(look);
		
	}

	void Transform::SetRight(const Vector& _right)
	{
		right = _right;
		right = Vector3Normalize(right);

		up = Vector3Cross(Vector::UnitZ, right);
		up = Vector3Normalize(up);

		look = Vector3Cross(right, up);
		look = Vector3Normalize(look);
	}

	void Transform::DecomposeMatrix(Vector& scale, Vector& rotation, Vector& position, Matrix matrix)
	{
		// Position, Scale
		for (int i = 0; i < 3; i++)
		{
			position[i] = matrix[3][i];
			scale[i] = sqrtf(powf(matrix[0][i], 2.0f) + powf(matrix[1][i], 2.0f) + powf(matrix[2][i], 2.0f));
		}
		position[3] = 1.0f;
		scale[3] = 0.0f;

		// Negative Scale
		if (Vector3Dot(Vector3Cross(matrix[0], matrix[1]), matrix[2]) < 0.0f)
		{
			scale *= -1.0f;
		}

		// Rotation
		for (int i = 0; i < 3; i++)
		{
			matrix[i] /= scale[i];
		}

		rotation = ConvertRotationMatrixToEuler(matrix);
	}

	void Transform::SetPosition(const Vector& _newPos, Space relativeTo)
	{
		if (relativeTo == Space::WORLD)
		{
			// ���� ��ü�� �������� ������ ���
			worldPosition = _newPos;

			// ���� �������� ���� �Ǿ�����, �θ����忡�� �� ���� �����ǵ� ���� �Ǿ���.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector _parentPos = Vector::Zero;

			// �θ� ������
			if (_parent != nullptr)
			{
				_parentPos = _parent->GetComponent<Transform>()->GetWorldPosition();
			}

			// ���� ������ ��ġ���� �θ��� ���� ��ġ�� ���ذ�, �ڽ��� ���� ��ġ�� �ȴ�.
			Vector _newLocalPos = _newPos - _parentPos;

			localPosition = _newLocalPos;
		}
		else
		{
			localPosition = _newPos;
		}

		// ���� ������ update
		UpdateLocalTM();
	}

	void Transform::SetRotation(const Vector& _eulerAngle, Space relativeTo)
	{
		// Position�� ���������� ���ð� ���忡 ���� ������ ���ش�.
		if (relativeTo == Space::WORLD)
		{
			worldRotation = _eulerAngle;

			// ���带 �ٲ� �� ������ �ٲ��ش�.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector _parentEulerAngle = Vector::Zero;

			// �θ� ���� Ȯ��
			if (_parent != nullptr)
			{
				_parentEulerAngle = _parent->GetComponent<Transform>()->GetWorldRotation();
			}

			Vector _newLocalRot = _eulerAngle - _parentEulerAngle;

			localRotation = _newLocalRot;
		}
		else
		{
			localRotation = _eulerAngle;
		}

		// ������ Ư�� ������, ȸ����, ��ġ�� ����Ǹ� �ٷ� TM�� �������ش�.
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::AddRotation(const Vector& _eulerAngle, Space relativeTo)
	{
		this->SetRotation(this->GetLocalRotation() + _eulerAngle);
	}

	void Transform::SetScale(const Vector& _newLocalScale)
	{
		localScale = _newLocalScale;

		UpdateLocalTM();
	}

	const Matrix& Transform::GetLocalTM()
	{
		return localTM;
	}

	const Matrix& Transform::GetWorldTM()
	{
		// TM Update�� üũ �Ǿ�������.. ������Ʈ �ϰ� ��ȯ���ش�.
		if (needTMUpdate)
		{
			UpdateWorldTM();
		}

		return worldTM;
	}

	void Transform::SetLocalTM(const Matrix& _localTM)
	{
		localTM = _localTM;

		// ������ ������ �� ������ ũ�� ���� ��ġ�� Transpose���ݴϴ�
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		// ������ ����Ǿ�����.. TMUpdate�� �ʿ��ϴٰ� üũ ���ݴϴ�.
		CheckingNeedTMUpdate();
	}

	void Transform::SetWorldTM(const Matrix& _worldTM)
	{
		worldTM = _worldTM;

		// ���尡 �ٲ������.. ���� ���� �������ݴϴ�.
			// ������ �θ�� �����Ǿ� �ֱ� ������ �θ� ã���ݴϴ�.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			Matrix _parentWorld = _parent->GetComponent<Transform>()->GetWorldTM();

			// �θ��� ���� ������� �ڽ��� ���忡 �����ָ� ������ ���ɴϴ�.
				// => �ڽ��� Local * �θ��� World = �ڽ��� World
				// => �ڽ��� Local == �ڽ��� World * �θ��� World�� �����
			localTM = worldTM * MatrixInverse(_parentWorld);
		}
		else
		{
			localTM = worldTM;
		}

		// ������ Transpose���ݴϴ�.
		DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		CheckingNeedTMUpdate();
	}

}