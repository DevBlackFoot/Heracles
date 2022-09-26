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
		// 내부적으로 Local TM을 계산해줍니다.
			// SRT 순으로 계산해주며, R의 경우 Z Roll X Pitch Y Yaw 순으로 회전 합니다.
		localTM = MatrixScalingFromVector(localScale) * MatrixRotationFromVector(localRotation) * MatrixTranslationFromVector(localPosition);

		// 로컬이 변경되었기 때문에.. TM 업데이트가 필요하다고 체크 해줍니다.
			// 월드 TM을 변경하고 난 뒤 TM 업데이트가 필요 하지 않다고 해줍니다.
		CheckingNeedTMUpdate();

	}

	void Transform::UpdateWorldTM()
	{
		// 부모와의 관계가 반드시 필요..
		// 부모의 월드TM에 자신의 Local을 곱해준 것 => 자신의 WorldTM
		// 부모의 현재 위치를 자신의 로컬 원점으로 보고.. 거기서 로컬 TM 만큼 옮겨준 것이 자신의 World TM이다.

		// 부모를 얻어와서.. 부모의 월드에 자신의 로컬을 곱해준다.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			// 부모가 존재한다면..
			// 부모의 월드에 자신의 로컬을 곱해줍니다.
			worldTM = localTM * (_parent->GetComponent<Transform>()->GetWorldTM());

			// 월드의 요소를 분리해서 각각의 스케일, 로테이션, 포지션에 넣어줍니다.
			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}
		else
		{
			// 부모가 없는 경우. 로컬TM을 그대로 WorldTM으로 넣어줍니다.
			worldTM = localTM;

			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}

		// look up right를 수정해줍니다.
		// World TM을 기준으로 노말 트랜스폼을 해줍니다..
		/*Vector::TransformNormal(Vector::UnitZ, m_WorldTM, m_Look);
		Vector::TransformNormal(Vector::UnitY, m_WorldTM, m_Up);
		Vector::TransformNormal(Vector::UnitX, m_WorldTM, m_Right);*/
		right = Vector::UnitX * worldTM;
		up = Vector::UnitY * worldTM;
		look = Vector::UnitZ * worldTM;

		// 각 벡터 정규화
		right = Vector3Normalize(right);
		up = Vector3Normalize(up);
		look = Vector3Normalize(look);

		// WorldTM을 변경 했기 때문에 TM Update가 필요하지 않습니다..
		needTMUpdate = false;
	}


	void Transform::CheckingNeedTMUpdate()
	{
		needTMUpdate = true;

		// 이 게임 오브젝트의 모든 자식들의 TM 업데이트가 필요하도록 해줍니다.
		// 현재 변경된 오브젝트의 TM이 변경되었기 때문에 자연스럽게 자식들의 TM도 변경되어야 하기 때문입니다.

		std::vector<std::shared_ptr<GameObject>>& children = this->GetGameObject()->GetChildren();

		// 모든 자식의 트랜스폼에 TM Update가 필요하도록!
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

		// 각 벡터 정규화
		right = Vector3Normalize(right);
		up = Vector3Normalize(up);
		look = Vector3Normalize(look);

		SetRotation(ConvertRotationMatrixToEuler(MatrixInverse(matrix)), Space::LOCAL);
	}

	void Transform::SetLook(const Vector& _look)
	{
		// Look이 바뀌었으니 나머지도 바꿔주자.
		look = _look;
		look = Vector3Normalize(look);

		// look과 월드 업을 이용해서 right를 뽑아준다.
		right = Vector3Cross(Vector::UnitY, look);
		right = Vector3Normalize(right);

		up = Vector3Cross(look, right);
		up = Vector3Normalize(up);
	}

	// 현재 코드에 문제가 있습니다..
		// Up 벡터와 Right벡터로 나머지 기저 벡터를 구하는 것은 무리가 있는가..
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
			// 월드 자체의 포지션을 변경한 경우
			worldPosition = _newPos;

			// 월드 포지션이 변경 되었으니, 부모입장에서 본 로컬 포지션도 변경 되었다.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector _parentPos = Vector::Zero;

			// 부모가 있으면
			if (_parent != nullptr)
			{
				_parentPos = _parent->GetComponent<Transform>()->GetWorldPosition();
			}

			// 현재 월드의 위치에서 부모의 월드 위치를 빼준게, 자신의 로컬 위치가 된다.
			Vector _newLocalPos = _newPos - _parentPos;

			localPosition = _newLocalPos;
		}
		else
		{
			localPosition = _newPos;
		}

		// 로컬 변경후 update
		UpdateLocalTM();
	}

	void Transform::SetRotation(const Vector& _eulerAngle, Space relativeTo)
	{
		// Position과 마찬가지로 로컬과 월드에 따른 연산을 해준다.
		if (relativeTo == Space::WORLD)
		{
			worldRotation = _eulerAngle;

			// 월드를 바꾼 뒤 로컬을 바꿔준다.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector _parentEulerAngle = Vector::Zero;

			// 부모 여부 확인
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

		// 로컬의 특정 스케일, 회전값, 위치가 변경되면 바로 TM을 변경해준다.
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
		// TM Update가 체크 되어있으면.. 업데이트 하고 반환해준다.
		if (needTMUpdate)
		{
			UpdateWorldTM();
		}

		return worldTM;
	}

	void Transform::SetLocalTM(const Matrix& _localTM)
	{
		localTM = _localTM;

		// 로컬을 변경한 후 각각의 크기 각도 위치를 Transpose해줍니다
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		// 로컬이 변경되었으니.. TMUpdate가 필요하다고 체크 해줍니다.
		CheckingNeedTMUpdate();
	}

	void Transform::SetWorldTM(const Matrix& _worldTM)
	{
		worldTM = _worldTM;

		// 월드가 바뀌었으니.. 로컬 값을 변경해줍니다.
			// 로컬은 부모와 연관되어 있기 때문에 부모를 찾아줍니다.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			Matrix _parentWorld = _parent->GetComponent<Transform>()->GetWorldTM();

			// 부모의 월드 역행렬을 자신의 월드에 곱해주면 로컬이 나옵니다.
				// => 자신의 Local * 부모의 World = 자신의 World
				// => 자신의 Local == 자신의 World * 부모의 World의 역행렬
			localTM = worldTM * MatrixInverse(_parentWorld);
		}
		else
		{
			localTM = worldTM;
		}

		// 각각을 Transpose해줍니다.
		DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		CheckingNeedTMUpdate();
	}

}