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
		// Ʈ������ ĳ��
		transform = GetGameObject()->GetComponent<Transform>();

		// ī�޶� ���Ž� ��Ⱦ�� 1.0�̾��� ������ ��׷�������.
		float _aspect = static_cast<float>(WindowManager::GetInstance()->GetClientWidth()) / WindowManager::GetInstance()->GetClientHeight();

		// �������� ����
		viewFrustum = std::make_shared<BoundingFrustum>();

		// ������ ������ SetLens��..
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
		// ViewMatrix�� �ᱹ.. ī�޶��� ���� TM�� ������̴�.
			// ī�޶��� WorldTM => ī�޶� ������ ��ġ�� �̵������ִ� ���(view => World��)
			// ī�޶��� WorldTM�� ����� => Ư�� ������Ʈ�� �����ָ�.. ī�޶� �߽����� �� ��ǥ��� �Ű��� �� �ֵ�.
		//m_Transform->GetWorldTM().Invert(m_View);

		//view = ViewMatrix(GetGameObject()->GetComponent<Transform>()->GetWorldPosition(), GetGameObject()->GetComponent<Transform>()->GetWorldRotation());

		// ĳ�� Ʈ������ ���
		view = ViewMatrix(transform->GetWorldPosition(), transform->GetWorldRotation());

		// Ʈ������ look right up
		transform->SetLook(view * Vector::UnitZ);
		//transform->SetRight(view * Vector::UnitX);
		//transform->SetUp(view * Vector::UnitY);
	}

	bool Camera::ViewFrustumCulling(std::shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix)
	{
		// �������� �и��� �̷��� �̿��߽��ϴ�.

		// �ٿ�� ������ ������ �״�� �׸��ϴ�.
		if (boundingCube == nullptr)
			return true;

		// �и����� ã���� -> �浹�Ѱ� �ƴϴϱ� -> �� �ؾ��Ѵ�.
			// ã�� ���ϸ� -> �浹 �ߴ� -> �������� �ȿ� �ְų� ��迡 �ִ� -> �׷����Ѵ�.

		float nearHalfH = tan(fovY / 2) * nearZ;
		float nearHalfW = (aspect * 2 * nearHalfH) / 2;

		Vector* localPoint = boundingCube->localPoint;
		Vector cubePoint[8];

		const Matrix worldViewMatrix = worldMatrix * view;

		// ���� �ø� �Ǻ� ����� ���� ���ؽ��� ���� view �����̽��� �Ű��ݴϴ�.
		for (int i = 0; i < 8; i++)
			cubePoint[i] = localPoint[i] * worldViewMatrix;

		// ��
		Vector axis[3] = { cubePoint[1] - cubePoint[0], cubePoint[2] - cubePoint[0], cubePoint[3] - cubePoint[0] };

		// ����
		float extents[3] = { Vector3Length(axis[0]), Vector3Length(axis[1]), Vector3Length(axis[2]) };

		// ����ȭ ���ݴϴ�.
		for (int i = 0; i < 3; i++)
		{
			axis[i] /= extents[i];

			// ����ȭ ��, half���̷� �������ݴϴ�.
			extents[i] *= 0.5f;
		}

		// ���� ��ǥ�� ���� ���� ������ �Ű��ݴϴ�.
		Vector center = boundingCube->localCenter * worldViewMatrix;

#pragma region FrustumPlane
		{
			// ������ �ߺ��� �������� �������� �и�.
			BoundingPlane* viewPlanes = viewFrustum->planes;

			// 0 near / 1 far / 2 left / 3 bottom / 4 right / 5 top
			for (int i = 0; i < 6; i++)
			{
				// �븻 ���Ͱ� near�� ����.. far�� ���� �ʾƵ� �ȴ�.
				if (i == 1)
					continue;

				float MDotX = fabsf(viewPlanes[i].normal.x);
				float MDotY = fabsf(viewPlanes[i].normal.y);
				float MDotZ = viewPlanes[i].normal.z;

				float MDotC = Vector3Dot(viewPlanes[i].normal, center);

				float obbRadius = 0.f;

				// ����ȭ
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

				// �������� proj
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

				// �������� proj
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

				// �������� proj
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

					// �������� proj
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

		// ���� ��� �ൿ���� �и����� ã�� �� ������. ȭ��� ������ �Ѵ�..
		return true;
	}

	void Camera::UpdateProjMatrix()
	{
		// ���������� XMMatrixPerspectiveFovRH�� ����Ѵ�. ����!
		//m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect, m_NearZ, m_FarZ);

		proj = PerspectiveMatrix(fovY, aspect, nearZ, farZ);

		// �������� ������Ʈ.
		UpdateViewFrustum();
	}

	void Camera::UpdateViewFrustum()
	{
		// near�� far�� ����, �ʺ� ���ݾ��� ���Ѵ�.

		// ���� ������.. y�� �þ߰� ������ tan�� nearZ ���� ���Ѵ�..
		float nearHalfH = tan(fovY / 2) * nearZ;
		// ��Ⱦ��� �ʺ� �����ش�.
		float nearHalfW = (aspect * 2 * nearHalfH) / 2;

		// ���� ������� farplane�� ���� �ʺ��� �ݵ� �����ش�.
		float farHalfH = tan(fovY / 2) * farZ;
		float farHalfW = (aspect * 2 * farHalfH) / 2;

		// ����� ���ϴµ� �ʿ��� �� 8�� (���� near�� far�� ��ġ�� �� 8��)
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

		// far�� �ڳʸ� �־��ش�
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
		// �Ű� ������ ���� ������ ��� ���� �ʱ�ȭ
		fovY = _fovY;
		aspect = _aspect;
		nearZ = _nearZ;
		farZ = _farZ;

		// Proj ������Ʈ.
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