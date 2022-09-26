#pragma once

#include "BoxCollider.h"

namespace Heracles
{
	namespace Player
	{
		class PlayerCollider : public BoxCollider
		{
		private:
			
		public:
			PlayerCollider(std::weak_ptr<GameObject> obj);

		};


	}
}

