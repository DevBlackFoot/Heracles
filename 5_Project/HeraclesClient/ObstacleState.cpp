#include "pch.h"
#include "ObstacleState.h"
#include "GameState.h"

namespace Heracles
{
	namespace Obstacle
	{
		static const float epsilon = 0.0001f;


		ObstacleState::ObstacleState(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj, ComponentType::MONOBEHAVIOR)
		{
		}
		void ObstacleState::Awake()
		{
		}
		void ObstacleState::Init(Vector centerPos, float width, float height)
		{
			this->centerPos = centerPos;
			this->width = width;
			this->height = height;

			this->leftTop = { centerPos.x - width / 2.f,centerPos.y,centerPos.z + height / 2.f };
			this->leftDown = { centerPos.x - width / 2.f,centerPos.y,centerPos.z - height / 2.f };
			this->rightDown = { centerPos.x + width / 2.f,centerPos.y,centerPos.z - height / 2.f };
			this->rightTop = { centerPos.x + width / 2.f,centerPos.y,centerPos.z + height / 2.f };
		}
		Direction ObstacleState::GetDirectionFromTarget(Vector lookVector, Vector pos)
		{
			if (abs(lookVector.x) > abs(lookVector.z))
			{
				if (this->centerPos.x > pos.x)
				{
					return Direction::LEFT;
				}
				else
				{
					return Direction::RIGHT;
				}
			}
			else
			{
				if (this->centerPos.z > pos.z)
				{
					return Direction::DOWN;
				}
				else
				{
					return Direction::TOP;
				}
			}
		}
		Vector ObstacleState::GetDirectionToTarget(Direction dir)
		{
			Vector target = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			switch (dir)
			{
			case Direction::LEFT:
			{
				float rtDistance = Vector3Length(target - this->rightTop);
				float rdDistance = Vector3Length(target - this->rightDown);
				if (rtDistance < rdDistance)
				{
					return Vector::Forward;
				}
				else
				{
					return Vector::Backward;
				}
			}
			case Direction::RIGHT:
			{
				float ltDistance = Vector3Length(target - leftTop);
				float rdDistance = Vector3Length(target - leftDown);

				if (ltDistance < rdDistance)
				{
					return Vector::Forward;
				}
				else
				{
					return Vector::Backward;
				}
			}
			case Direction::TOP:
			{
				float ldDistance = Vector3Length(target - leftDown);
				float rdDistance = Vector3Length(target - rightDown);
				if (ldDistance > rdDistance)
				{
					return Vector::Right;
				}
				else
				{
					return Vector::Left;
				}
			}
			case Direction::DOWN:
			{
				float ltDistance = Vector3Length(target - leftTop);
				float rtDistance = Vector3Length(target - rightTop);
				if (ltDistance > rtDistance)
				{
					return Vector::Right;
				}
				else
				{
					return Vector::Left;
				}
			}
			default:
				return Vector::Zero;
			}
		}
		Vector ObstacleState::GetNextTarget(Vector lookVector)
		{
			Vector playerPos = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			
			Vector temp = this->ParallelToAxis(lookVector);
			if (temp == Vector::Zero)
			{
				// 조건 통과
			}
			else
			{
				return temp;
			}

			if (lookVector.x < 0.f  && lookVector.z < 0.f)
			{
				if (Vector3Length(playerPos - leftTop) < Vector3Length(playerPos - rightDown))
				{
					return leftTop;
				}
				else
				{
					return rightDown;
				}
			}
			else if (lookVector.x < 0.f && lookVector.z>0.f)
			{
				if (Vector3Length(playerPos - rightTop) < Vector3Length(playerPos - leftDown))
				{
					return rightTop;
				}
				else
				{
					return leftDown;
				}
			}
			else if (lookVector.x > 0.f && lookVector.z > 0.f)
			{
				if (Vector3Length(playerPos - leftTop) < Vector3Length(playerPos - rightDown))
				{
					return leftTop;
				}
				else
				{
					return rightDown;
				}
			}
			else
			{
				if (Vector3Length(playerPos - rightTop) < Vector3Length(playerPos - leftDown))
				{
					return rightTop;
				}
				else
				{
					return leftDown;
				}
			}

			return Vector::Zero;
		}
		Vector ObstacleState::GetNetDirection(Vector pos)
		{





			return Vector();
		}
		Vector ObstacleState::ParallelToAxis(Vector lookVector)
		{
			Vector playerPos = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();

			if (abs(lookVector.x * lookVector.y) < 0.f + epsilon)
			{
				if (abs(lookVector.x) < 0.f + epsilon)
				{
					if (lookVector.z > 0.f)
					{
						if (Vector3Length(playerPos - leftTop) < Vector3Length(playerPos - rightTop))
						{
							return leftDown;
						}
						else
						{
							return rightDown;
						}
					}
					else
					{
						if (Vector3Length(playerPos - leftDown) < Vector3Length(playerPos - rightDown))
						{
							return leftTop;
						}
						else
						{
							return rightTop;
						}
					}
				}
				else
				{
					if (lookVector.x > 0.f)
					{
						if (Vector3Length(playerPos - rightTop) < Vector3Length(playerPos - rightDown))
						{
							return leftTop;
						}
						else
						{
							return leftDown;
						}
					}
					else
					{
						if (Vector3Length(playerPos - leftTop) < Vector3Length(playerPos - leftDown))
						{
							return rightTop;
						}
						else
						{
							return rightDown;
						}
					}
				}
			}
			return Vector::Zero;
		}
	}
}