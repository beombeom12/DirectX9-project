#pragma once

#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }

	_byte	Get_DIMouseState(MOUSEKEYSTATE byMouseID)
	{
		return m_MouseState.rgbButtons[byMouseID];
	}
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
	{
		return *(((long *)(&m_MouseState) + eMoveState));
	}

public:
	HRESULT			Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void			SetUp_InputDev();

	_bool			Key_Pressing(_int _iKey);
	_bool			Key_Down(_int _iKey);
	_bool			Key_Up(_int _iKey);
	_bool			Mouse_Down(MOUSEKEYSTATE _MouseButton);

private:
	DIMOUSESTATE	m_MouseState;
	_byte			m_byKeyState[256];
	_bool			m_bKeyState[256];
	_bool			m_bMouseState[4];

	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	LPDIRECTINPUT8			m_pInputSDK;

public:
	void Free(void) override;
};

END