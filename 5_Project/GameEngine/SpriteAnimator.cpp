#include "pch.h"
#include "SpriteAnimator.h"

#include "GraphicsManager.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GameObject.h"

namespace GameEngineSpace
{
	SpriteAnimator::SpriteAnimator(std::weak_ptr<GameObject> gameObj)
		: RendererBase(gameObj)
		, spriteAnimation(nullptr)
		, tick(0.0f)
	{

	}

	void SpriteAnimator::Init(const std::string& name)
	{
		GraphicsEngineSpace::Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		spriteAnimation = factory->CreateSpriteAnimation(name);
	}

	void SpriteAnimator::SetSpriteTexture(TextureBase* texture, float width, float height, int frameX, int frameY, float tickPerFrame)
	{
		if (spriteAnimation == nullptr)
			return;

		spriteAnimation->SetTexture(texture, width, height, frameX, frameY, tickPerFrame);
	}

	void SpriteAnimator::SetSpriteTexture(const std::string& textureName, const std::string& path, float width, float height, int frameX, int frameY, float tickPerFrame)
	{
		if (spriteAnimation == nullptr)
			return;

		auto* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		GraphicsEngineSpace::TextureBase* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
		{
			auto* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

			texture = factory->CreateTexture(textureName, path);
		}

		spriteAnimation->SetTexture(texture, width, height, frameX, frameY, tickPerFrame);
	}

	void SpriteAnimator::SetLoop(bool isLoop)
	{
		if (spriteAnimation == nullptr)
			return;

		spriteAnimation->SetLoop(isLoop);
	}

	void SpriteAnimator::SetPosition(const Vector& position)
	{
		if (spriteAnimation == nullptr)
			return;

		spriteAnimation->SetPosition(position);
	}

	void SpriteAnimator::SetWidth(float width)
	{
		if (spriteAnimation == nullptr)
			return;

		spriteAnimation->SetWidth(width);
	}

	void SpriteAnimator::SetHeight(float height)
	{
		if (spriteAnimation == nullptr)
			return;

		spriteAnimation->SetHeight(height);
	}

	void SpriteAnimator::Play()
	{
		if (spriteAnimation == nullptr)
			return;

		spriteAnimation->Play();
	}

	void SpriteAnimator::PrepareRender(float tick)
	{
		this->tick = tick;
	}

	void SpriteAnimator::Render(std::shared_ptr<GraphicsEngineSpace::GraphicsEngine> graphicsEngine)
	{
		if (spriteAnimation == nullptr)
			return;

		const auto& camera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
		const Vector& cameraWorldPos = camera->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

		const auto& parent = GetGameObject()->GetParent();
		
		Matrix worldViewProj;

		if (parent != nullptr)
		{
			const Matrix& parentScaleMatrix = MatrixScalingFromVector(parent->GetComponent<Transform>()->GetWorldScale());
			const Matrix& parentTransMatrix = MatrixTranslationFromVector(parent->GetComponent<Transform>()->GetWorldPosition());

			worldViewProj = /*parentScaleMatrix * */parentTransMatrix * camera->GetViewMatrix() * camera->GetProjMatrix();
		}
		else
		{
			worldViewProj = camera->GetViewMatrix() * camera->GetProjMatrix();
		}

		spriteAnimation->Render(graphicsEngine.get(), worldViewProj, cameraWorldPos, tick);
	}
}