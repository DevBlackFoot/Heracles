#pragma once

#include "RendererBase.h"
#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	using GraphicsEngineSpace::TextureBase;

	class SpriteAnimator : public RendererBase
	{
		GraphicsEngineSpace::SpriteAnimation* spriteAnimation;

		float tick;

	public:
		SpriteAnimator(std::weak_ptr<GameObject> gameObj);
		virtual ~SpriteAnimator() = default;

		void Init(const std::string& name);
		void SetSpriteTexture(TextureBase* texture, float width, float height, int frameX, int frameY, float tickPerFrame);
		void SetSpriteTexture(const std::string& textureName, const std::string& path, float width, float height, int frameX, int frameY, float tickPerFrame);
		void SetLoop(bool isLoop);
		void SetPosition(const Vector& position);
		void SetWidth(float width);
		void SetHeight(float height);
		void Play();

		void PrepareRender(float tick) override;
		void Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine>graphicsEngine) override;
	};
}