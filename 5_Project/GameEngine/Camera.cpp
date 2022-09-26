#include "pch.h"
#include "Camera.h"

#include "GameObject.h"
#include "WindowManager.h"

#include "../HeraclesMath/HeraclesMath.h"

namespace GameEngineSpace
{

	Camera::Camera(std::weak_ptr<GameObject> _obj) :
		ComponentBase(_obj, ComponentType::CAMERA)
		, projType(ProjType::PERSPECTIVE)
		, shakingIntensity(0.0f)
		, originalZ(0.0f)
	{
		// 트랜스폼 캐싱
		transform = GetGameObject()->GetComponent<Transform>();

		// 카메라 갱신시 종횡비가 1.0이었기 때문에 찌그러졌었다.
		float _aspect = static_cast<float>(WindowManager::GetInstance()->GetClientWidth()) / WindowManager::GetInstance()->GetClientHeight();

		// 프러스텀 생성
		viewFrustum = std::make_shared<BoundingFrustum>();

		// 각각의 정보는 SetLens로..
		SetLens(0.25f * HeraclesMath::PI, _aspect, 1.0f, 500.0f);

		UpdateViewMatrix();

	}

	Camera::~Camera()
	{
	}

	void Camera::Release()
	{
		transform.reset();
		viewFrustum.reset();

		__super::Release();
	}

	void Camera::UpdateViewMatrix()
	{
		// ViewMatrix는 결국.. 카메라의 월드 TM의 역행렬이다.
			// 카메라의 WorldTM => 카메라를 월드의 위치로 이동시켜주는 행렬(view => World로)
			// 카메라의 WorldTM의 역행렬 => 특정 오브젝트에 곱해주면.. 카메라를 중심으로 한 좌표계로 옮겨줄 수 있따.
		//m_Transform->GetWorldTM().Invert(m_View);

		//view = ViewMatrix(GetGameObject()->GetComponent<Transform>()->GetWorldPosition(), GetGameObject()->GetComponent<Transform>()->GetWorldRotation());

		// 캐싱 트랜스폼 사용
		view = ViewMatrix(transform->GetWorldPosition(), transform->GetWorldRotation());

		// 트랜스폼 look right up
		transform->SetLook(view * Vector::UnitZ);
		//transform->SetRight(view * Vector::UnitX);
		//transform->SetUp(view * Vector::UnitY);
	}

	bool Camera::ViewFrustumCulling(std::shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix)
	{
		// 혜성이의 분리축 이론을 이용했습니다.

		// 바운딩 볼륨이 없으면 그대로 그립니다.
		if (boundingCube == nullptr)
			return true;

		// 분리축을 찾으면 -> 충돌한게 아니니까 -> 컬 해야한다.
			// 찾지 못하면 -> 충돌 했다 -> 프러스텀 안에 있거나 경계에 있다 -> 그려야한다.

		float nearHalfH = tan(fovY / 2) * nearZ;
		float nearHalfW = (aspect * 2 * nearHalfH) / 2;

		Vector* localPoint = boundingCube->localPoint;
		Vector cubePoint[8];

		const Matrix worldViewMatrix = worldMatrix * view;

		// 현재 컬링 판별 대상의 로컬 버텍스를 월드 view 스페이스로 옮겨줍니다.
		for (int i = 0; i < 8; i++)
			cubePoint[i] = localPoint[i] * worldViewMatrix;

		// 축
		Vector axis[3] = { cubePoint[1] - cubePoint[0], cubePoint[2] - cubePoint[0], cubePoint[3] - cubePoint[0] };

		// 길이
		float extents[3] = { Vector3Length(axis[0]), Vector3Length(axis[1]), Vector3Length(axis[2]) };

		// 정규화 해줍니다.
		for (int i = 0; i < 3; i++)
		{
			axis[i] /= extents[i];

			// 정규화 뒤, half길이로 세팅해줍니다.
			extents[i] *= 0.5f;
		}

		// 센터 좌표도 같은 공간 상으로 옮겨줍니다.
		Vector center = boundingCube->localCenter * worldViewMatrix;

#pragma region FrustumPlane
		{
			// 변수명 중복을 막기위해 스코프로 분리.
			BoundingPlane* viewPlanes = viewFrustum->planes;

			// 0 near / 1 far / 2 left / 3 bottom / 4 right / 5 top
			for (int i = 0; i < 6; i++)
			{
				// 노말 벡터가 near와 평행.. far는 하지 않아도 된다.
				if (i == 1)
					continue;

				float MDotX = fabsf(viewPlanes[i].normal.x);
				float MDotY = fabsf(viewPlanes[i].normal.y);
				float MDotZ = viewPlanes[i].normal.z;

				float MDotC = Vector3Dot(viewPlanes[i].normal, center);

				float obbRadius = 0.f;

				// 정규화
				for (size_t j = 0; j < 3; j++)
					obbRadius += fabsf(Vector3Dot(viewPlanes[i].normal, axis[i]) * extents[i]);

				float obbMin = MDotC - obbRadius;
				float obbMax = MDotC + obbRadius;

				float p = nearHalfW * MDotX + nearHalfH * MDotY;

				float tau0 = nearZ * MDotZ - p;
				float tau1 = nearZ * MDotZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region OBB Plane
		{
			for (size_t i = 0; i < 3; i++)
			{
				const Vector& M = axis[i];

				float MoX = fabsf(M.x);
				float MoY = fabsf(M.y);
				float MoZ = M.z;

				float MoC = Vector3Dot(M, center);

				float obbRadius = extents[i];

				float obbMin = MoC - obbRadius;
				float obbMax = MoC + obbRadius;

				// 프러스텀 proj
				float p = nearHalfW * MoX + nearHalfH * MoY;
				float tau0 = nearZ * MoZ - p;
				float tau1 = nearZ * MoZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region Edge R x A_i
		{
			for (size_t m = 0; m < 3; m++)
			{
				const Vector M = { 0.f, -axis[m].z, axis[m].y };

				float MoX = 0.f;
				float MoY = fabsf(M.y);
				float MoZ = M.z;
				float MoC = M.y * center.y + M.z * center.z;

				float obbRadius = 0.f;

				for (size_t i = 0; i < 3; i++)
					obbRadius += fabsf(Vector3Dot(axis[i], M) * extents[i]);

				float obbMin = MoC - obbRadius;
				float obbMax = MoC + obbRadius;

				// 프러스텀 proj
				float p = nearHalfW * MoX + nearHalfH * MoY;
				float tau0 = nearZ * MoZ - p;
				float tau1 = nearZ * MoZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region Edge U x A_i
		{
			for (size_t m = 0; m < 3; m++)
			{
				const Vector M = { axis[m].z, 0.f, axis[m].y };

				float MoX = fabsf(M.x);
				float MoY = 0.f;
				float MoZ = M.z;
				float MoC = M.x * center.x + M.z * center.z;

				float obbRadius = 0.f;

				for (size_t i = 0; i < 3; i++)
					obbRadius += fabsf(Vector3Dot(axis[i], M) * extents[i]);

				float obbMin = MoC - obbRadius;
				float obbMax = MoC + obbRadius;

				// 프러스텀 proj
				float p = nearHalfW * MoX + nearHalfH * MoY;
				float tau0 = nearZ * MoZ - p;
				float tau1 = nearZ * MoZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region Frustum Edge x A_i
		{
			for (size_t obbEdgeIdx = 0; obbEdgeIdx < 3; obbEdgeIdx++)
			{
				const Vector M[] =
				{
					Vector3Cross(axis[obbEdgeIdx], Vector{-nearHalfW, 0.f, nearZ}),		// Left Plane
					Vector3Cross(axis[obbEdgeIdx], Vector{nearHalfW, 0.f, nearZ}),		// Right Plane
					Vector3Cross(axis[obbEdgeIdx], Vector{0.f, nearHalfH, nearZ}),		// Top Plane
					Vector3Cross(axis[obbEdgeIdx], Vector{0.f, -nearHalfH, nearZ})		// Bottom Plane
				};

				for (size_t m = 0; m < 4; m++)
				{
					float MoX = fabsf(M[m].x);
					float MoY = fabsf(M[m].y);
					float MoZ = M[m].z;

					const float epsilon = 0.000f;
					if(MoX < epsilon && MoY < epsilon && fabsf(MoZ) < epsilon)
						continue;

					float MoC = Vector3Dot(M[m], center);

					float obbRadius = 0.f;

					for (size_t i = 0; i < 3; i++)
						obbRadius += fabsf(Vector3Dot(axis[i], M[m]) * extents[i]);

					float obbMin = MoC - obbRadius;
					float obbMax = MoC + obbRadius;

					// 프러스텀 proj
					float p = nearHalfW * MoX + nearHalfH * MoY;
					float tau0 = nearZ * MoZ - p;
					float tau1 = nearZ * MoZ + p;

					if (tau0 < 0.0f)
						tau0 *= farZ / nearZ;
					if (tau1 > 0.0f)
						tau1 *= farZ / nearZ;

					if (obbMin > tau1 || obbMax < tau0)
						return false;
				}
			}
		}
#pragma endregion

		// 위의 모든 행동에도 분리축을 찾을 수 없으면. 화면상에 보여야 한다..
		return true;
	}

	void Camera::UpdateProjMatrix()
	{
		// 내부적으로 XMMatrixPerspectiveFovRH를 사용한다. 참고!
		//m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect, m_NearZ, m_FarZ);

		proj = PerspectiveMatrix(fovY, aspect, nearZ, farZ);

		// 프러스텀 업데이트.
		UpdateViewFrustum();
	}

	void Camera::UpdateViewFrustum()
	{
		// near와 far의 길이, 너비 절반씩을 구한다.

		// 높이 절반은.. y축 시야각 절반의 tan에 nearZ 값을 곱한다..
		float nearHalfH = tan(fovY / 2) * nearZ;
		// 종횡비로 너비를 구해준다.
		float nearHalfW = (aspect * 2 * nearHalfH) / 2;

		// 같은 방식으로 farplane의 길이 너비의 반도 구해준다.
		float farHalfH = tan(fovY / 2) * farZ;
		float farHalfW = (aspect * 2 * farHalfH) / 2;

		// 평면을 구하는데 필요한 점 8개 (각각 near와 far에 위치한 점 8개)
		Vector nearLeftTop = Vector(-nearHalfW, nearHalfH, nearZ);
		Vector nearLeftBottom = Vector(-nearHalfW, -nearHalfH, nearZ);
		Vector nearRightTop = Vector(nearHalfW, nearHalfH, nearZ);
		Vector nearRightBottom = Vector(nearHalfW, -nearHalfH, nearZ);

		Vector farLeftTop = Vector(-farHalfW, farHalfH, farZ);
		Vector farLeftBottom = Vector(-farHalfW, -farHalfH, farZ);
		Vector farRightTop = Vector(farHalfW, farHalfH, farZ);
		Vector farRightBottom = Vector(farHalfW, -farHalfH, farZ);

		// Near Plane
		viewFrustum->planes[0] = BoundingPlane(nearLeftTop, nearRightTop, nearLeftBottom);
		// Far Plane
		viewFrustum->planes[1] = BoundingPlane(farRightTop, farLeftTop, farRightBottom);
		// Left Plane
		viewFrustum->planes[2] = BoundingPlane(farLeftTop, nearLeftTop, farLeftBottom);
		// Bottom Plane
		viewFrustum->planes[3] = BoundingPlane(nearLeftBottom, nearRightBottom, farLeftBottom);
		// Right Plane
		viewFrustum->planes[4] = BoundingPlane(nearRightTop, farRightTop, nearRightBottom);
		// Top Plane
		viewFrustum->planes[5] = BoundingPlane(nearRightTop, farRightTop, nearLeftTop);

		// far의 코너를 넣어준다
		// leftTop
		viewFrustum->frustumCorner[0] = Vector(farLeftTop.x, farLeftTop.y, farLeftTop.z, 1.0f);
		// rightTop
		viewFrustum->frustumCorner[1] = Vector(farRightTop.x, farRightTop.y, farRightTop.z, 1.0f);
		// leftBottom
		viewFrustum->frustumCorner[2] = Vector(farLeftBottom.x, farLeftBottom.y, farLeftBottom.z, 1.0f);
		// rightBottom
		viewFrustum->frustumCorner[3] = Vector(farRightBottom.x, farRightBottom.y, farRightBottom.z, 1.0f);

	}

	const Matrix& Camera::GetViewMatrix()
	{
		return view;
	}

	const Matrix& Camera::GetProjMatrix()
	{
		return proj;
	}

	void Camera::Update(float tick)
	{
		UpdateViewMatrix();

		if (shakingIntensity > 5.0f)
		{
			static bool flag = true;

			Vector rotation = transform->GetWorldRotation();

			if (flag == true)
			{
				rotation.z = originalZ + shakingIntensity * tick * 10.0f;
				flag = false;
			}
			else
			{
				flag = true;
				rotation.z = originalZ - shakingIntensity * tick * 10.0f;
			}

			shakingIntensity *= (1.0f - 1.0f * tick * 10.0f);

			transform->SetRotation(rotation, Space::WORLD);
		}
		else
		{
			Vector rotation = transform->GetWorldRotation();

			rotation.z = originalZ;

			transform->SetRotation(rotation, Space::WORLD);

			shakingIntensity = 0.0f;
		}
	}

	void Camera::SetLens(float _fovY, float _aspect, float _nearZ, float _farZ)
	{
		// 매개 변수로 받은 값으로 멤버 변수 초기화
		fovY = _fovY;
		aspect = _aspect;
		nearZ = _nearZ;
		farZ = _farZ;

		// Proj 업데이트.
		UpdateProjMatrix();
	}

	void Camera::SetShaking(float intensity)
	{
		if (shakingIntensity > 0.0f)
		{
			Vector rotation = transform->GetWorldRotation();

			rotation.z = originalZ;

			transform->SetRotation(rotation, Space::WORLD);
		}

		shakingIntensity = intensity;
		originalZ = transform->GetWorldRotation().z;
	}
}