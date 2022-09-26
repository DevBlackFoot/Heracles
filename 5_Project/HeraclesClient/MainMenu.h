#pragma once

#include <thread>

namespace Heracles
{
	class Canvas;
	using GraphicsEngineSpace::TextureBase;
	using GraphicsEngineSpace::Factory;
	using GraphicsEngineSpace::ResourceManager;

	class MainMenu : public ComponentBase
	{
		struct FileInfo
		{
			std::string name;
			std::string path;
		};

		struct ModelInfo
		{
			std::string name;
			std::string path;
			std::string metallic;
			std::string roughness;
			std::string ambientocclusion;
		};

		struct AnimationFileInfo
		{
			FileInfo file;
			std::string animationName;
			float animationSpeed;
			bool isLoop;
		};

	private:
		static GraphicsEngineSpace::Canvas* canvas;
		GraphicsEngineSpace::ButtonUI* startButton;
		GraphicsEngineSpace::ButtonUI* settingButton;
		GraphicsEngineSpace::ButtonUI* controlsButton;
		GraphicsEngineSpace::ButtonUI* creditButton;
		GraphicsEngineSpace::ButtonUI* exitButton;

		std::thread* fileLoader;

		std::vector<ModelInfo> models;
		std::vector<AnimationFileInfo> animations;
		std::vector<FileInfo> textures;

	public:
		MainMenu(std::weak_ptr<GameObject> obj);
		~MainMenu();

		virtual void Awake() override;
		virtual void Start() override;

		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

	private:
		void CreateBackGround(TextureBase* backGround, TextureBase* title);
		void CreateMenuButton(TextureBase* hoverTexture);

		void LoadFiles(Factory* factory, ResourceManager* resourceManager);
		void SetCharacterList();
		void SetObjectList();
		void SetMapList();
		void SetTextureList();
		void SetWeaponList();
	};
}

