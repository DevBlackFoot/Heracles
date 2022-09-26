#pragma once
#include <vector>
#include "FootEngineTypedef.h"

class IRenderer;
class Camera;
class Timer;
class IDXObject;

/**
 * \brief 실제로 게임의 루프에 들어갈 Engine입니다.
 *
 * 씬매니저가 만들어지면 씬의 내용을 업데이트하고 렌더하는 역할을 담당합니다
 *
 * 2022.07.19(화) Dev.BlackFoot
 */
namespace FootEngine
{
	class GameEngine
	{
	public:
		GameEngine();
		~GameEngine();

		bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight);
		void Finalize();

		void SceneUpdate(float _dt);

		void SceneRender();

		// 마우스 입력은 받아야할 듯
		void OnMouseDown(int x, int y);
		void OnMouseUp(int x, int y);
		void MouseMove(int btnState, int x, int y);

		void OnResize();

		// 렌더러 가져오기
		bool IsVaildDevice();
		void SetClientSize(int _width, int _height);
		void SetWinMinMax(bool _isMinimized, bool _isMaximized);

		// 내부적으로 렌더러를 가지고 있고..
	private:
		// 포인터를 스마트 포인터 위주로 사용해보자..
		//IRenderer* m_Renderer;
		std::unique_ptr<IRenderer> m_Renderer;
		HWND m_hWnd;

		POINT m_LastMousePos;

		// 실제로 그림을 그려줄 각종 오브젝트들을 가지고 있을 뭔가가 필요하다..
		// 컴포넌트 패턴으로 가지 않을 것이면. ObjBase의 포인터를 가지고 있는 벡터 or list가 필요할 듯 하다.
			// Scene으로 대체할 수 있지 않을까 이것을..
		//Camera* m_Camera;
		std::unique_ptr<Camera> m_Camera;
		float m_DeltaTime;
		// 여기에 박스를 한번 만들어보자..
		IDXObject* m_Box;

		// 오브젝트 리스트를 만들자.
		std::vector<IDXObject*> m_ObjVector;
	};

}