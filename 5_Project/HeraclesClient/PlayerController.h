#pragma once
#include <functional>


namespace Heracles
{
	namespace Player
	{
		class EventBase;

		class PlayerController : public ComponentBase
		{
		private:
			std::shared_ptr<Transform> playerTransform;
			std::shared_ptr<InputManager> input;			

			float lookVectorWeight = 1.f;
			float rotationWeightInput = 0.2f;

			Vector target;

			float moveSpeed;
			float rotateDegree;

			// 이동계산에 사용되는 벡터 상수
			const Vector dirStop = Vector::Zero;
			const Vector dirUp = Vector(-1.f, 0.f, -1.f);			
			const Vector dirLeft = Vector(1.f, 0.f, -1.f);
			const Vector dirDown = Vector(1.f, 0.f, 1.f);
			const Vector dirRight = Vector(-1.f, 0.f, 1.f);		


		public:
			PlayerController(std::weak_ptr<GameObject> obj);
			~PlayerController();

			virtual void Awake() override;
			virtual void Start() override;

			virtual void PreUpdate(float tick) override;
			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

		private:
			
			void MoveCheck(EventBase* e);

			Vector CheckInputAxis();

			Vector CalculateRotationVector(bool up, bool down, bool left, bool right);

			void MoveLookDir(float m = 1.f,Vector axis = Vector::Zero);

			void Rotate(Vector forward, Vector inputAxis);
		};
	}
}


