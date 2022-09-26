#pragma once

namespace Heracles
{
	namespace Boss
	{
		class BossMapCameraController : public ComponentBase
		{
		private:
			float timer = 0.f;
			std::shared_ptr<GameEngineSpace::Transform> target;
			std::shared_ptr<GameEngineSpace::Transform> transform;

			std::shared_ptr<GameObject> boss;
			std::shared_ptr<GameObject> player;

			Vector initialPos = Vector(-75.f,10.f,0.f);

			float x = -80.f;
			float y = 80.f;
			float z = -80.f;

			bool cameraLookat = true;

		public:
			BossMapCameraController(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;
			virtual void Start() override;
			
			virtual void PreUpdate(float tick) override;
			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

			inline void SetTarget(std::shared_ptr<GameEngineSpace::Transform> target) { this->target = target; }

			inline void SetActivation(bool trigger) { this->cameraLookat = trigger; }
		};
	}
}

