#include "..\..\Header\InputDev.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

CInputDev::CInputDev()
	: m_pInputSDK(nullptr), m_pKeyBoard(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}


CInputDev::~CInputDev()
{
	Free();
}

bool CInputDev::Key_Pressing(int _iKey)
{
	if (Get_DIKeyState(_iKey) & 0x80)
		return true;

	return false;
}

bool CInputDev::Key_Down(int _byKeyID)
{
	if (!m_bKeyState[_byKeyID] && Get_DIKeyState(_byKeyID) & 0x80)
	{
		m_bKeyState[_byKeyID] = true;
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (m_bKeyState[i] && !(Get_DIKeyState(i) & 0x80))
		{
			m_bKeyState[i] = false;
		}
	}
	return false;
}

bool CInputDev::Key_Up(int _iKey)
{
	if (m_bKeyState[_iKey] && !(Get_DIKeyState(_iKey) & 0x80))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (!m_bKeyState[i] && (Get_DIKeyState(i) & 0x80))
		{
			m_bKeyState[i] = true;
		}
	}

	return false;
}

bool CInputDev::Mouse_Down(MOUSEKEYSTATE _MouseButton)
{
	if (!m_bMouseState[_MouseButton] && m_MouseState.rgbButtons[_MouseButton] & 0x80)
	{
		m_bMouseState[_MouseButton] = true;
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_bMouseState[i] && !(m_MouseState.rgbButtons[i] & 0x80))
		{
			m_bMouseState[i] = false;
		}
	}
	return false;
}


HRESULT CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDev::SetUp_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
