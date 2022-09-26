#pragma once

namespace Heracles
{
	namespace Trap
	{
		class TrapState : public ComponentBase
		{
		private:
			
			bool canBeTriggerd = true; // 몬스터를 모두 잡으면 false로 바뀜

			float waitTime = 2.f;
			float onTime;
			float delayTime;

			float waitTimer;
			float onTimer;
			float delayTimer;

		public:
			TrapState(std::weak_ptr<GameObject> obj);

			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

		private:

			void UpdateTimer(float tick);


		};
	}
}

