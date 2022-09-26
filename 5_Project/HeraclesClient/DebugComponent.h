#pragma once
#include "UIManager.h"

namespace Heracles
{
	namespace Debug
	{
		class DebugComponent : public ComponentBase
		{
		private:
			bool activated = true;

			// ������� canvas����
			Canvas* debugCanvas;

		public:
			DebugComponent(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;

			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

		private:
			void PrintMousePos();
			void PrintInventoryState();
			void PrintPlayerInfo();
			void DeActivateAll();
		};
	}
}

