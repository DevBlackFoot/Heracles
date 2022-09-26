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
		// 로컬 센터에서 부터 Dir방향으로 떨어진..
		Vector vertices[2] =
		{
			{0.f, 0.f, 0.f, 1.0f},
			{(direction.x * 10.f), (direction.y * 10.f), (direction.z * 10.f), 1.0f}
		};

		WORD indices[2]
		{
			0, 1
		};

		// 그래픽스 엔진 가져와야함.
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
		// this가 Ray고 other이 Ray가 아닐 때.
		Matrix obbRotInv = MatrixInverse(other->objWorldRotTM);

		// 포지션을 other의 스페이스로 변환
		Vector rayLocalCenter = worldCenter * obbRotInv;
		// direction도 변환해주기
		Vector rayLocalDir = direction * obbRotInv;
		rayLocalDir = Vector3Normalize(rayLocalDir);
		rayLocalDir = VectorReciprocal(rayLocalDir);

		Vector boxLocalCenter = other->worldCenter * obbRotInv;

		// Box의 버텍스 중 x, y, z의 최소, 최대 값.
		Vector boxMin = { FLT_MAX, FLT_MAX, FLT_MAX };
		Vector boxMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		// 상자의 모든 버텍스 좌표를 돌면서 MinMax를 뽑아낸다..
			// extents를 활용해서 구해주자.
		std::vector<Vector> vertices;
		// 중심에서 각 extents 별로 더해주고 빼주면 vertices 정보가 나온다.
			// extents == 각 축의 half
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
				// 버텍스의 각 축 좌표
				float vertexCoord = vertices[i].v[j];

				if (boxMin.v[j] > vertexCoord)
					boxMin.v[j] = vertexCoord;

				if (boxMax.v[j] < vertexCoord)
					boxMax.v[j] = vertexCoord;
			}
		}

		// 레이의 위치로 뽑아낸 벡터
		boxMin = boxMin - rayLocalCenter;
		boxMax = boxMax - rayLocalCenter;

		// 레이에 투영
		Vector t1 = boxMin * rayLocalDir;
		Vector t2 = boxMax * rayLocalDir;

		// 각 좌표축의 최소 최대값.
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

		// Min에서 가장 큰 값과 Max에서 가장 작은 값을 찾는다.
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
		// 만날 수 있는 점의 위치
		float t0, t1;

		// 원의 중심으로 향하는 벡터
		Vector rayToSphere = other->GetWorldCenter() - worldCenter;

		// 해당 벡터를 dir에 사영.
		float projRayToSphere = Vector3Dot(rayToSphere, direction);

		// 원의 중심에서 레이 까지 수직인 선분의 길이
		float d = Vector3Dot(rayToSphere, rayToSphere) - projRayToSphere * projRayToSphere;

		// 이 길이가 반지름보다 크면. 만나지 않는다.
		if (d > other->radius)
			return false;
		
		// 중심에서 벡터로 내린 수선에서 원과 부딪히는 부분까지의 거리
		float solDist = sqrtf(other->radius - d);
		t0 = projRayToSphere - solDist;
		t1 = projRayToSphere + solDist;

		// t0이 t1보다 크면 -> 즉 뒤에 있으면.. 스왑한다.
		if (t0 > t1)
			std::swap(t0, t1);

		// 대신 0보다 작으면 t1을 대신 쓴다.
		if (t0 < 0)
		{
			t0 = t1;
			// 그럼에도 0보다 작으면. false를 리턴한다.
			if (t0 < 0)
				return false;
		}

		// 그 거리를 넣어준다.
		//SetHit(other->GetGameObject(), t0);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), t0));

		return true;
	}

	void RayCollider::SetHit()
	{
		// 기존에 없다면..
		// 새로 hit 한 녀석의 거리가 더 가까우면 => 제일 앞에 있다.
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
		// Hit 초기화.
			// 길이 벡터 초기화
		currHitObjVec.clear();
	}

	bool RayCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// 원과.. 박스만 해준다.
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
		// 업데이트 처리
		// 월드 센터 업데이트
		worldCenter = localCenter + transform->GetWorldPosition();

		// direction 업뎃
		//direction = transform->GetLook();
	}
}
