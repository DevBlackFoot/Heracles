#include "pch.h"
#include "SphereCollider.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	SphereCollider::SphereCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, localCenter(Vector::Zero)
		, worldCenter(transform->GetWorldPosition())
		, radius(1.f)
	{
		// �ϴ� ���� �ݶ��̴���..
		std::vector<Vector> vertices;
		std::vector<WORD> indices;

		Vector topVertex = { 0.f, radius, 0.f, 1.0f };

		vertices.push_back(topVertex);

		uint32 stackCount = 50;
		uint32 sliceCount = 50;

		float phiStep = HeraclesMath::PI / stackCount;
		float thetaStep = 2.0f * HeraclesMath::PI / sliceCount;

		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vector v;

				v.x = radius * sinf(phi) * cosf(theta);
				v.y = abs(radius * cosf(phi));
				v.z = radius * sinf(phi) * sinf(theta);
				v.w = 1.0f;

				vertices.push_back(v);
			}
		}

		for (UINT i = 1; i <= sliceCount; i++)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		UINT baseIndex = 1;

		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}

		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("SphereLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices.data(), vertices.size());
		factory->CreateIndexBuffer("SphereLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices.data(), indices.size());
	}

	SphereCollider::~SphereCollider()
	{
	}

	bool SphereCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// ���� �浹��.. ���ͻ����� �Ÿ��� �������� �պ��� ������ �浹�� ��..
		Vector otherWorldCenter = other->GetWorldCenter();
		float otherRadius = other->radius;

		if (
			(Vector3Length((worldCenter - otherWorldCenter))
				<=
				std::abs(radius) + std::abs(otherRadius))
			== true)
			return true;
		else
			return false;
	}

	// Comet Engine ����
	bool SphereCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		Matrix obbRotInv = MatrixInverse(other->objWorldRotTM);

		//Matrix translate = MatrixTranslationFromVector(worldCenter);

		// OBB ȸ������ ����ȯ�� �ؼ�. AABB �����̽��� ������ش�.
			// �ǹ̻����� Vector3 Transform�� ����.
		Vector sphereLocalCenter = worldCenter * obbRotInv;

		Vector boxLocalCenter = other->worldCenter * obbRotInv;

		// AABB�� �� �浹.
		// Box�� MinMax�� ���� �߽� ������ �Ÿ��� Ȯ���Ѵ�.
		// �ڽ��� ���Ϳ���.. ������(�� �ڽ��� �� ���� ����)�� ������ ���ų� ���ϸ� �ּ� �ִ� ���̴�.
		Vector boxMin = std::move(boxLocalCenter - other->size * 0.5f);
		Vector boxMax = std::move(boxLocalCenter + other->size * 0.5f);

		// �ش� �ִ� �ּ� ���� �̿��ؼ� x, y, z���� min������ �����ش�..
			// �ڽ��� ���� �����.. ģ��.
		float minX = std::max<float>(boxMin.x, std::min<float>(sphereLocalCenter.x, boxMax.x));
		float minY = std::max<float>(boxMin.y, std::min<float>(sphereLocalCenter.y, boxMax.y));
		float minZ = std::max<float>(boxMin.z, std::min<float>(sphereLocalCenter.z, boxMax.z));

		Vector minPos = Vector{ minX, minY, minZ };

		float minDist = Vector3Length(minPos - sphereLocalCenter);

		if (minDist <= radius)
			return true;
		else
			return false;
	}

	void SphereCollider::Release()
	{
		__super::Release();
	}

	bool SphereCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// other�� �ڽ� �����ͷ� ���� ĳ���� �ؼ�.. Ȯ���غ���..
		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		// �갡 ���� �ƴϸ�.. �� �浹 üũ�� �Ѵ�.
		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;
	}

	void SphereCollider::ColliderRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		Matrix worldPos = MatrixTranslationFromVector(worldCenter);
		Matrix worldScale = MatrixScalingFromVector({ radius, radius, radius });
		Matrix world = worldScale * worldPos;

		renderer->GraphicsDebugBeginEvent("Sphere Line");
		renderer->DrawLine(resourceManager->GetBuffer("SphereLineVB"), resourceManager->GetBuffer("SphereLineIB"), color, MatrixTranspose(world));
		renderer->GraphicsDebugEndEvent();
	}

	void SphereCollider::LateUpdate(float tick)
	{
		// ������Ʈ�� ���������� �ش� �������� �ݿ����ش�.
		worldCenter = localCenter + transform->GetWorldPosition();
	}
}
