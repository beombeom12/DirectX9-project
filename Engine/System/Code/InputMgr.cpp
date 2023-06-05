#include "InputMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputMgr)

CInputMgr::CInputMgr()
{
}


CInputMgr::~CInputMgr()
{
	Free();
}

void CInputMgr::Ready_Input(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
	_bAxisMode = false;

	GetClientRect(_hwnd, &_rtClient);
	_fVertical = 0.f;
	_fHorizontal = 0.f;

	_ptCenter = { (_rtClient.right - _rtClient.left) / 2, (_rtClient.bottom - _rtClient.top) / 2 };
}

void CInputMgr::Update_Input()
{
	HWND hwnd = ::GetActiveWindow();
	if (_hwnd != hwnd)
	{
		for (_uint key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (_uint key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}
	_bAxisMode = false;
	if (_bAxisMode)
	{
		POINT ptCur;
		POINT ptCenter = _ptCenter;
		ClientToScreen(_hwnd, &ptCenter);
		GetCursorPos(&ptCur);

		_fHorizontal = static_cast<_float>(ptCur.x - ptCenter.x) * 0.001f;
		_fVertical = static_cast<_float>(ptCur.y - ptCenter.y) * 0.001f;
		SetCursorPos(ptCenter.x, ptCenter.y);
	}
}

void CInputMgr::SetAxisMode(bool bAxisMode)
{
	_bAxisMode = bAxisMode;
	if (_bAxisMode)
	{
		ShowCursor(FALSE);
		POINT ptCenter = _ptCenter;
		ClientToScreen(_hwnd, &ptCenter);
		SetCursorPos(ptCenter.x, ptCenter.y);
	}
	else
	{
		ShowCursor(TRUE);
		_fVertical = 0.f;
		_fHorizontal = 0.f;
	}
}

void CInputMgr::Free(void)
{
	_states.clear();
	_states.shrink_to_fit();
}

