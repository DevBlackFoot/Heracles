#pragma once

namespace Heracles
{
	namespace Boss
	{
		class TestBossController : public ComponentBase
		{
		private:
			std::shared_ptr<InputManager> input;
		public:
			TestBossController(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;
			virtual void Start() override;
			virtual void Update(float tick) override;
		};
	}
}

