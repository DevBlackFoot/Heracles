#include "pch.h"
#include "RayCollider.h"

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include <utility>
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	RayCollider::RayCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, localCenter(Vector::Zero)
		, worldCenter(transform->GetWorldPosition())
		, direction(transform->GetLook())
		, hit({})
	{
		// ���� ���Ϳ��� ���� Dir�������� ������..
		Vector vertices[2] =
		{
			{0.f, 0.f, 0.f, 1.0f},
			{(direction.x * 10.f), (direction.y * 10.f), (direction.z * 10.f), 1.0f}
		};

		WORD indices[2]
		{
			0, 1
		};

		// �׷��Ƚ� ���� �����;���.
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("RayLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 2);
		factory->CreateIndexBuffer("RayLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 2);
	}

	void RayCollider::Release()
	{
		__super::Release();
	}

	bool RayCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		// this�� Ray�� other�� Ray�� �ƴ� ��.
		Matrix obbRotInv = MatrixInverse(other->objWorldRotTM);

		// �������� other�� �����̽��� ��ȯ
		Vector rayLocalCenter = worldCenter * obbRotInv;
		// direction�� ��ȯ���ֱ�
		Vector rayLocalDir = direction * obbRotInv;
		rayLocalDir = Vector3Normalize(rayLocalDir);
		rayLocalDir = VectorReciprocal(rayLocalDir);

		Vector boxLocalCenter = other->worldCenter * obbRotInv;

		// Box�� ���ؽ� �� x, y, z�� �ּ�, �ִ� ��.
		Vector boxMin = { FLT_MAX, FLT_MAX, FLT_MAX };
		Vector boxMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		// ������ ��� ���ؽ� ��ǥ�� ���鼭 MinMax�� �̾Ƴ���..
			// extents�� Ȱ���ؼ� ��������.
		std::vector<Vector> vertices;
		// �߽ɿ��� �� extents ���� �����ְ� ���ָ� vertices ������ ���´�.
			// extents == �� ���� half
		Vector tempVertex = { boxLocalCenter.x + other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x + other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x + other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x - other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x + other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x - other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x - other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = { boxLocalCenter.x - other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);

		for (int i = 0; i < vertices.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				// ���ؽ��� �� �� ��ǥ
				float vertexCoord = vertices[i].v[j];

				if (boxMin.v[j] > vertexCoord)
					boxMin.v[j] = vertexCoord;

				if (boxMax.v[j] < vertexCoord)
					boxMax.v[j] = vertexCoord;
			}
		}

		// ������ ��ġ�� �̾Ƴ� ����
		boxMin = boxMin - rayLocalCenter;
		boxMax = boxMax - rayLocalCenter;

		// ���̿� ����
		Vector t1 = boxMin * rayLocalDir;
		Vector t2 = boxMax * rayLocalDir;

		// �� ��ǥ���� �ּ� �ִ밪.
		Vector tMin;
		Vector tMax;

		for (int i = 0; i < 3; i++)
		{
			if (t1.v[i] > t2.v[i])
			{
				tMin.v[i] = t2.v[i];
				tMax.v[i] = t1.v[i];
			}
			else
			{
				tMin.v[i] = t1.v[i];
				tMax.v[i] = t2.v[i];
			}
		}

		float maxOfMin = -FLT_MAX;
		float minOfMax = FLT_MAX;

		// Min���� ���� ū ���� Max���� ���� ���� ���� ã�´�.
		for (int i = 0; i < 3; i++)
		{
			if (maxOfMin < tMin.v[i])
				maxOfMin = tMin.v[i];

			if (minOfMax > tMax.v[i])
				minOfMax = tMax.v[i];
		}

		if (maxOfMin > minOfMax)
			return false;
		

		if (minOfMax < 0.0f)
			return false;
		

		//SetHit(other->GetGameObject(), maxOfMin);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), maxOfMin));

		return true;
	}

	bool RayCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// ���� �� �ִ� ���� ��ġ
		float t0, t1;

		// ���� �߽����� ���ϴ� ����
		Vector rayToSphere = other->GetWorldCenter() - worldCenter;

		// �ش� ���͸� dir�� �翵.
		float projRayToSphere = Vector3Dot(rayToSphere, direction);

		// ���� �߽ɿ��� ���� ���� ������ ������ ����
		float d = Vector3Dot(rayToSphere, rayToSphere) - projRayToSphere * projRayToSphere;

		// �� ���̰� ���������� ũ��. ������ �ʴ´�.
		if (d > other->radius)
			return false;
		
		// �߽ɿ��� ���ͷ� ���� �������� ���� �ε����� �κб����� �Ÿ�
		float solDist = sqrtf(other->radius - d);
		t0 = projRayToSphere - solDist;
		t1 = projRayToSphere + solDist;

		// t0�� t1���� ũ�� -> �� �ڿ� ������.. �����Ѵ�.
		if (t0 > t1)
			std::swap(t0, t1);

		// ��� 0���� ������ t1�� ��� ����.
		if (t0 < 0)
		{
			t0 = t1;
			// �׷����� 0���� ������. false�� �����Ѵ�.
			if (t0 < 0)
				return false;
		}

		// �� �Ÿ��� �־��ش�.
		//SetHit(other->GetGameObject(), t0);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), t0));

		return true;
	}

	void RayCollider::SetHit()
	{
		// ������ ���ٸ�..
		// ���� hit �� �༮�� �Ÿ��� �� ������ => ���� �տ� �ִ�.
		/*if ((hit.hitName != TEXT("HitNone") && hit.hitDist < dist) || dist < 0)
			return;
	
		hit.hitGameObject = gameObj;
		hit.hitTransform = gameObj->GetComponent<Transform>();
		hit.hitDist = dist;
		hit.hitName = gameObj->GetName();*/

		if (currHitObjVec.empty() == true)
			return;

		int idx = 0;
		float min = FLT_MAX;

		for(int i = 0; i < currHitObjVec.size(); i++)
		{
			if(min > currHitObjVec[i].second && currHitObjVec[i].second > 0)
			{
				min = currHitObjVec[i].second;
				idx = i;
			}
		}

		hit.hitGameObject = currHitObjVec[idx].first;
		hit.hitTransform = currHitObjVec[idx].first->GetComponent<Transform>();
		hit.hitDist = currHitObjVec[idx].second;
		hit.hitName = currHitObjVec[idx].first->GetName();
	}

	void RayCollider::ClearHit()
	{
		// Hit �ʱ�ȭ.
			// ���� ���� �ʱ�ȭ
		currHitObjVec.clear();
	}

	bool RayCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// ����.. �ڽ��� ���ش�.
		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;

	}

	void RayCollider::ColliderRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		Matrix lookAt = MatrixLookAt(worldCenter, direction, Vector::UnitY);

		Matrix world = MatrixInverse(lookAt);

		renderer->GraphicsDebugBeginEvent("Ray Line");
		renderer->DrawLine(resourceManager->GetBuffer("RayLineVB"), resourceManager->GetBuffer("RayLineIB"), color, MatrixTranspose(world));
		renderer->GraphicsDebugEndEvent();

	}

	void RayCollider::LateUpdate(float tick)
	{
		// ������Ʈ ó��
		// ���� ���� ������Ʈ
		worldCenter = localCenter + transform->GetWorldPosition();

		// direction ����
		//direction = transform->GetLook();
	}
}
