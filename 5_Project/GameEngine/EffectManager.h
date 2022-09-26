#pragma once



namespace GameEngineSpace
{
	class EffectManager
	{
		static std::shared_ptr<EffectManager> instance;

	public:
		EffectManager();
		~EffectManager();
	};
}