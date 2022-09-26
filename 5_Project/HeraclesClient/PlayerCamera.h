#pragma once

namespace Heracles
{
	/// <summary>
	/// 카메라 로테이션 고정
	/// radialBlur 적용여부
	/// </summary>
	namespace Player
	{
		class PlayerCamera : public ComponentBase
		{
		private:
			std::shared_ptr<GameEngineSpace::Transform> target;
			std::shared_ptr<GameEngineSpace::Transform> transform;

			float x = -80.f;
			float y = 80.f;
			float z = -80.f;

			bool cameraLookat = true;

		public:
			PlayerCamera(std::weak_ptr<GameObject> obj);

			virtual void Start() override;
			virtual void PreUpdate(float tick) override;
			virtual void LateUpdate(float tick) override;

			void SetCameraPos(float x,float y, float z);

			inline void SetTarget(std::shared_ptr<GameEngineSpace::Transform> target) { this->target = target; }

			inline void SetActivation(bool trigger) { this->cameraLookat = trigger; }
		};
	}
}

