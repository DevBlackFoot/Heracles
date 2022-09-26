#pragma once


namespace Heracles
{
	namespace Obstacle
	{
		enum class Direction
		{
			LEFT,
			RIGHT,
			TOP,
			DOWN
		};

		class ObstacleState : public ComponentBase
		{
		private:
			Vector centerPos;
			float width;
			float height;
		private:
			Vector leftTop;
			Vector leftDown;
			Vector rightDown;
			Vector rightTop;

		public:
			ObstacleState(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;

			void Init(Vector centorPos, float width, float height);

			Direction GetDirectionFromTarget(Vector lookVector,Vector pos);

			//z+ , x+ ->true
			Vector GetDirectionToTarget(Direction dir);

			Vector GetNextTarget(Vector lookVector);

			Vector GetNetDirection(Vector pos);

		private:
			Vector ParallelToAxis(Vector lookVector);
		};
	}
}

