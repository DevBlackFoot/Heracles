#include "pch.h"
#include "GameEngine.h"
#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "Factory.h"
#include "BuilderManger.h"
#include "Basic32Obj.h"
#include "PosColorObj.h"

FootEngine::GameEngine::GameEngine() :
	m_Renderer(nullptr)
	, m_DeltaTime(0.0f)
	, m_Box(nullptr)
	, m_LastMousePos()
	, m_hWnd(nullptr)
{
	m_Camera = make_unique<::Camera>();
	m_Camera->LookAt(Vector3(0.0f, 15.0f, -25.0f), Vector3(0.0f, 7.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

}

FootEngine::GameEngine::~GameEngine()
{
}

bool FootEngine::GameEngine::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
{
	m_hWnd = _hWnd;
	// 렌더러 생성 및 초기화.
		// 렌더러가 생성될 때, 내부에서 그래픽스 코어, 렌더 타겟, 각종 Effect및 Factory, BuildManger가 생성된다.
		// 원시 포인터를 받아 유니크 ptr로 만들어준다.
	m_Renderer.reset(CreateRenderer());

	//팩토리 초기화.
	if (m_Renderer->Initialize(_hWnd, _clientWidth, _clientHeight) != true)
	{
		return false;
	}

#pragma region Object Create

	// 박스 만들기.
	m_Box = Factory::GetInstance()->CreateDXObject<Basic32Obj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), "Box");

	m_Renderer->AddRenderObj(m_Box);

	IDXObject* _tmpObj = Factory::GetInstance()->CreateDXObject<PosColorObj>(
		BuilderManger::GetInstance()->GetBuilder("PosColorBuilder"), "Grid"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

	_tmpObj = Factory::GetInstance()->CreateDXObject<PosColorObj>(
		BuilderManger::GetInstance()->GetBuilder("PosColorBuilder"), "Axis"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

	// texture 파일 없는 버전
	_tmpObj = Factory::GetInstance()->CreateDXObject<Basic32Obj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), "../ASE/box.ASE"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

	_tmpObj = Factory::GetInstance()->CreateDXObject<Basic32Obj>(
		BuilderManger::GetInstance()->GetBuilder("Basic32Builder"), "../ASE/teapot.ASE",
		L"../Textures/000000002476.dds"
		);
	m_ObjVector.push_back(_tmpObj);
	m_Renderer->AddRenderObj(_tmpObj);

#pragma endregion

	// 위에서 생성한 오브젝트를 초기화 시켜줍니다.
	m_Renderer->InitObject();

	return true;
}

void FootEngine::GameEngine::Finalize()
{
	m_Renderer->Finalize();
}

void FootEngine::GameEngine::SceneUpdate(float _dt)
{
	// 업데이트 할 씬..
		// 카메라를 넣어주자.
	m_DeltaTime = _dt;

	// 여기서 키 업데이트를 하면 아마 될건데..
		// dll에서 common library의 싱글턴 객체를 가져왔을 때와
		// exe에서 common library의 객체를 가져왔을 때의 메모리 주소가 다르게 나온다.
		// 그래서 생기는 문제..
	Input::GetInstance()->Update();

	// 카메라 이동. (교수님 예제)
	if (Input::GetInstance()->GetInputState('W', KeyState::PRESS))
		m_Camera->Walk(10.0f * _dt);

	if (Input::GetInstance()->GetInputState('S', KeyState::PRESS)) // s
		m_Camera->Walk(-10.0f * _dt);

	if (Input::GetInstance()->GetInputState('A', KeyState::PRESS)) // a
		m_Camera->Strafe(-10.0f * _dt);

	if (Input::GetInstance()->GetInputState('D', KeyState::PRESS)) // d
		m_Camera->Strafe(10.0f * _dt);

	if (Input::GetInstance()->GetInputState('Q', KeyState::PRESS)) // q
		m_Camera->WorldUpDown(-10.0f * _dt);

	if (Input::GetInstance()->GetInputState('E', KeyState::PRESS)) // e
		m_Camera->WorldUpDown(10.0f * _dt);

	// ViewTM 만들기
	m_Camera->UpdateViewMatrix();
	// 회전을 위한 설정
	static XMVECTOR rotate = {};
	rotate.m128_f32[1] += 0.005f;

	XMMATRIX rotateMx = XMMatrixRotationRollPitchYawFromVector(rotate);

	// 트랜지션
	XMVECTOR translate = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMMATRIX _translationMx = XMMatrixTranslationFromVector(translate);

	XMMATRIX world = rotateMx * _translationMx;

	// 그리드 Axis 업데이트
		// 둘 다 고정된 녀석이므로 이렇게 해도 된다..
	for (auto obj : m_ObjVector)
	{
		obj->Update(XMMatrixIdentity(), m_Camera->View(), m_Camera->Proj());
		// 각각의 오브젝트를 찾아서 변경해준다..
		if (obj->GetObjName() == "../ASE/box.ASE")
		{
			obj->Update(world, m_Camera->View(), m_Camera->Proj());
		}

		if (obj->GetObjName() == "../ASE/teapot.ASE")
		{
			translate.m128_f32[0] = -5.0f;
			_translationMx = XMMatrixTranslationFromVector(translate);

			world = rotateMx * _translationMx;
			obj->Update(world, m_Camera->View(), m_Camera->Proj());
		}
	}

	translate.m128_f32[0] = 5.0f;
	_translationMx = XMMatrixTranslationFromVector(translate);

	world = rotateMx * _translationMx;

	// box 띄워보기..
	m_Box->Update(world, m_Camera->View(), m_Camera->Proj());

	DirectionalLight _dirLit;
	_dirLit.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	_dirLit.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_dirLit.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_dirLit.Direction = XMFLOAT3(0.0f, -0.5f, 0.57735f);

	m_Box->LightUpdate(_dirLit, {}, {}, m_Camera->GetPosition());

	// 나머지도 빛 업데이트 해줘야합니다..
	for (auto obj : m_ObjVector)
	{
		obj->LightUpdate(_dirLit, {}, {}, m_Camera->GetPosition());
	}

	Input::GetInstance()->LateUpdate();
}

void FootEngine::GameEngine::SceneRender()
{
	m_Renderer->RenderAll();
}

void FootEngine::GameEngine::OnMouseDown(int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture(m_hWnd);
}

void FootEngine::GameEngine::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

void FootEngine::GameEngine::MouseMove(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePos.y));

		m_Camera->Pitch(dy);
		m_Camera->RotateY(dx);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}

void FootEngine::GameEngine::OnResize()
{
	m_Renderer->OnResize();

	// 카메라를 새로 세팅해준다.
	m_Camera->SetLens(0.25f * 3.1415926535f, m_Renderer->GetAspectRatio()
		, 1.0f, 1000.0f);
}

// 기본적인건 렌더러 안에 구현 되어있다..
bool FootEngine::GameEngine::IsVaildDevice()
{
	return m_Renderer->IsVaildDevice();
}

void FootEngine::GameEngine::SetClientSize(int _width, int _height)
{
	m_Renderer->SetClientSize(_width, _height);
}

void FootEngine::GameEngine::SetWinMinMax(bool _isMinimized, bool _isMaximized)
{
	m_Renderer->SetWinMinMax(_isMinimized, _isMaximized);
}
