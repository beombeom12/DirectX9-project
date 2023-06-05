#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

enum class KEY_TYPE
{
	UP = DIK_UP,
	DOWN = DIK_DOWN,
	LEFT = DIK_LEFT,
	RIGHT = DIK_RIGHT,

	W = DIK_W,
	A = DIK_A,
	S = DIK_S,
	D = DIK_D,
	Q = DIK_Q,
	E = DIK_E,
	Z = DIK_Z,
	C = DIK_C,

	RETURN = DIK_RETURN,
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	E_END
};

enum
{
	KEY_TYPE_COUNT = static_cast<_int>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<_int>(KEY_STATE::E_END),
};

class ENGINE_DLL CInputMgr : public CBase
{
	DECLARE_SINGLETON(CInputMgr)

private:
	explicit CInputMgr();
	virtual ~CInputMgr();

public:
	void Ready_Input(HWND hwnd);
	void Update_Input();

	// 누르고 있을 때
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// 맨 처음 눌렀을 때
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// 맨 처음 눌렀다 뗐을 때
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

	void SetAxisMode(bool bAxisMode);
	_float GetVerticalAxis() const { return _fVertical; }
	_float GetHorizontalAxis() const { return _fHorizontal; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<_byte>(key)]; }

private:
	HWND _hwnd;
	vector<KEY_STATE> _states;

	bool _bAxisMode;
	RECT _rtClient;
	POINT _ptCenter;
	_float _fVertical;
	_float _fHorizontal;

public:
	virtual void Free(void) override;
};
END
