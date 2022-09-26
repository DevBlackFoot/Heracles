#include "pch.h"
#include "BoxCollider.h"

#include "GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include <utility>
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	BoxCollider::BoxCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, size(Vector{ 1.f, 1.f, 1.f })
		, localCenter(Vector::Zero)
		, worldCenter(transform->GetWorldPosition())
	{
		objWorldRotTM = Matrix::Identity;
		worldNormals = std::vector<Vector>(3);
		worldNormals[0] = Vector{ 1.f, 0.f, 0.f };	// Vector3::Right
		worldNormals[1] = Vector{ 0.f, 1.f, 0.f };	// Vector3::Up
		worldNormals[2] = Vector{ 0.f, 0.f, 1.f };	// Vector3::BackWard;

		extents = std::vector<float>(3, size.x * 0.5f);

		// �⺻���� ���ؽ� ���۸� ����ϴ�
			// �ش� ������ ������ ���� ���� TM���� �������ݴϴ�.
			// ���⼭�� �ܼ��� 1.f 1.f 1.f�� ���ڸ� �׷��ݴϴ�.
			// �⺻������ 8���� ���ؽ� (�ؽ��İ� ���� ���� -> �ɰ����� �ʿ䰡 ����..
		Vector vertices[8] =
		{
			// upSide
			{-extents[0], extents[1], extents[2], 1.0f},
			{extents[0], extents[1], extents[2], 1.0f},
			{extents[0], extents[1], -extents[2], 1.0f},
			{-extents[0], extents[1], -extents[2], 1.0f},

			// downside
			{-extents[0], -extents[1], extents[2], 1.0f},
			{extents[0], -extents[1], extents[2], 1.0f},
			{extents[0], -extents[1], -extents[2], 1.0f},
			{-extents[0], -extents[1], -extents[2], 1.0f}
		};

		WORD indices[24] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			0, 4,
			1, 5,
			2, 6,
			3, 7,
			4, 5,
			5, 6,
			6, 7,
			7, 4
		};

		// �׷��Ƚ� �������� ���� ����
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("BoxLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 8);
		factory->CreateIndexBuffer("BoxLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 24);
	}

	BoxCollider::~BoxCollider()
	{
	}

	void BoxCollider::Release()
	{
		__super::Release();
	}

	// OBB�� �̿��Ѵ�. by Comet
	bool BoxCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		const Vector& otherCenter = other->worldCenter;
		// �� ������� �Ÿ��� �޾ƿ´�.
		const std::vector<float>& otherExtents = other->extents;
		// ������ �ʿ��� ����� �޾ƿ´�.
		const std::vector<Vector>& otherNormals = other->worldNormals;

		// ���� ������ ����� �����ϴ� ������.
		float otherCDotPlane = 0.f;
		float myCDotPlane = 0.f;
		float otherDotRadius = 0.f;
		float myDotRadius = 0.f;
		float otherDotMin = 0.f;
		float otherDotMax = 0.f;
		float myDotMin = 0.f;
		float myDotMax = 0.f;

		// �� ���鿡 ���� �˻�.
			// ������ü�� �������� ���� ���� �� ���̱� ������ �� ���� ���ָ� �ȴ�.
		for (size_t i = 0; i < 3; i++)
		{
			const Vector& targetNormal = worldNormals[i];

			// �߽ɿ��� �Ǻ� ���� ���� ����.
			otherCDotPlane = Vector3Dot(otherCenter, targetNormal);
			myCDotPlane = Vector3Dot(worldCenter, targetNormal);

			otherDotRadius = 0.f;

			// �и��� �������� ã�� ����
				// TODO �ش� ������ ��������..
				// �� ���� �븻�� ���̸� ���ؼ����� �༮�� ��� ������ Radius..
				// �ᱹ �Ǻ��࿡���� ���̸� �����ִ� �����̴�.
			for (size_t j = 0; j < 3; j++)
				otherDotRadius += fabsf(Vector3Dot(otherNormals[j], targetNormal)) * otherExtents[j];

			myDotRadius = extents[i];

			otherDotMin = otherCDotPlane - otherDotRadius;
			otherDotMax = otherCDotPlane + otherDotRadius;

			myDotMin = myCDotPlane - myDotRadius;
			myDotMax = myCDotPlane + myDotRadius;

			// �и��� �߰�.
				// �ش� �࿡�� ��ġ�� �� ������..!
			if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
				return false;
		}

		// ����� ��鿡 ���� �˻�
		for (size_t i = 0; i < 3; i++)
		{
			const Vector& targetNormal = otherNormals[i];

			otherCDotPlane = Vector3Dot(otherCenter, targetNormal);
			myCDotPlane = Vector3Dot(worldCenter, targetNormal);

			myDotRadius = 0.f;

			for (size_t j = 0; j < 3; j++)
				myDotRadius += fabsf(Vector3Dot(worldNormals[j], targetNormal)) * extents[j];

			otherDotRadius = otherExtents[i];

			otherDotMin = otherCDotPlane - otherDotRadius;
			otherDotMax = otherCDotPlane + otherDotRadius;

			myDotMin = myCDotPlane - myDotRadius;
			myDotMax = myCDotPlane + myDotRadius;

			// �и��� �߰�.
				// �ش� �࿡�� ��ġ�� �� ������..!
			if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
				return false;
		}

		// �� �𼭸��� �������� �����Ǵ� ��鿡 ���� �˻�.
			// �����θ� ���������� ó���� �ȵǴ� �浹 ������ �˻��ϱ� ����.
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				Vector targetNormal = Vector3Cross(worldNormals[i], otherNormals[j]);

				if (Vector3Equal(targetNormal, Vector::Zero))
					continue;

				targetNormal = Vector3Normalize(targetNormal);

				otherCDotPlane = Vector3Dot(otherCenter, targetNormal);
				myCDotPlane = Vector3Dot(worldCenter, targetNormal);

				myDotRadius = 0.f;

				for (size_t k = 0; k < 3; k++)
					myDotRadius += fabsf(Vector3Dot(worldNormals[k], targetNormal)) * extents[k];

				otherDotRadius = 0.f;

				for (size_t k = 0; k < 3; k++)
					otherDotRadius += fabsf(Vector3Dot(otherNormals[k], targetNormal)) * otherExtents[k];

				otherDotMin = otherCDotPlane - otherDotRadius;
				otherDotMax = otherCDotPlane + otherDotRadius;

				myDotMin = myCDotPlane - myDotRadius;
				myDotMax = myCDotPlane + myDotRadius;

				if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
					return false;
			}
		}

		//DoNotCheckEdge:

		// ��� ��쿡 �и����� ã�� ���ߴٸ�? �̰� �浹�̴�!
		return true;
	}

	bool BoxCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// ������ ���� �� �Ͱ� ����������.. obb�� AABB �����̽��� �ű� ��. �浹 üũ�Ѵ�.
		Matrix obbRotInv = MatrixInverse(objWorldRotTM);

		Vector sphereLocalCenter = other->worldCenter * obbRotInv;
		Vector boxLocalCenter = worldCenter * obbRotInv;

		Vector boxMin = std::move(boxLocalCenter - size * 0.5);
		Vector boxMax = std::move(boxLocalCenter + size * 0.5);

		float minX = std::max<float>(boxMin.x, std::min<float>(sphereLocalCenter.x, boxMax.x));
		float minY = std::max<float>(boxMin.y, std::min<float>(sphereLocalCenter.y, boxMax.y));
		float minZ = std::max<float>(boxMin.z, std::min<float>(sphereLocalCenter.z, boxMax.z));

		Vector minPos = Vector{ minX, minY, minZ };

		float minDist = Vector3Length(minPos - sphereLocalCenter);

		if (minDist <= other->radius)
			return true;
		else
			return false;
	}

	void BoxCollider::SetSize(const Vector& newSize)
	{
		size = newSize;

		extents[0] = size.x * 0.5;
		extents[1] = size.y * 0.5;
		extents[2] = size.z * 0.5;

	}

	bool BoxCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// ���� ĳ���� ����..

		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;
	}

	void BoxCollider::ColliderRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		Matrix worldPos = MatrixTranslationFromVector(worldCenter);
		Matrix worldScale = MatrixScalingFromVector(size);
		Matrix world = worldScale * objWorldRotTM * worldPos;

		renderer->GraphicsDebugBeginEvent("Box Line");
		renderer->DrawLine(resourceManager->GetBuffer("BoxLineVB"), resourceManager->GetBuffer("BoxLineIB"), color, MatrixTranspose(world));
		renderer->GraphicsDebugEndEvent();
	}

	void BoxCollider::LateUpdate(float tick)
	{
		// ���� ���� ������Ʈ
		worldCenter = localCenter + transform->GetWorldPosition();

		Matrix objRotTM = MatrixRotationFromVector(transform->GetWorldRotation());

		// ���� �� �����Ӱ� ���ؼ� �̹� �����ӿ� ȸ�� ���� ��ȭ�� ���� ���.
		if (objWorldRotTM == objRotTM)
			return;

		// �浹 �˻翡 �ʿ��� ������ ĳ��
		objWorldRotTM = objRotTM;

		// Transform
		worldNormals[0] = Vector{ 1.f, 0.f, 0.f };
		worldNormals[0] = worldNormals[0] * objRotTM;
		worldNormals[0] = Vector3Normalize(worldNormals[0]);

		worldNormals[1] = Vector{ 0.f, 1.f, 0.f };
		worldNormals[1] = worldNormals[1] * objRotTM;
		worldNormals[1] = Vector3Normalize(worldNormals[1]);

		worldNormals[2] = Vector{ 0.f, 0.f, 1.f };
		worldNormals[2] = worldNormals[2] * objRotTM;
		worldNormals[2] = Vector3Normalize(worldNormals[2]);
	}
}
