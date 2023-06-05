HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End(HWND hWnd)
{
	CGraphicDev::GetInstance()->Render_End(hWnd);
}

_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}
void			Update_Timer(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Update_Timer(pTimerTag);
}

_bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

void Ready_Input(HWND hwnd)
{
	CInputMgr::GetInstance()->Ready_Input(hwnd);
}
void Update_Input()
{
	CInputMgr::GetInstance()->Update_Input();
}
bool GetButton(KEY_TYPE key)
{
	return CInputMgr::GetInstance()->GetButton(key);
}
bool GetButtonDown(KEY_TYPE key)
{
	return CInputMgr::GetInstance()->GetButtonDown(key);
}
bool GetButtonUp(KEY_TYPE key)
{
	return CInputMgr::GetInstance()->GetButtonUp(key);
}
void SetAxisMode(bool bAxisMode)
{
	CInputMgr::GetInstance()->SetAxisMode(bAxisMode);
}

_float GetVerticalAxis()
{
	return CInputMgr::GetInstance()->GetVerticalAxis();
}

_float GetHorizontalAxis()
{
	return CInputMgr::GetInstance()->GetHorizontalAxis();
}

_byte		Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
{
	return CInputDev::GetInstance()->Get_DIMouseState(byMouseID);
}

_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMoveState);
}
HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void			SetUp_InputDev(void)
{
	CInputDev::GetInstance()->SetUp_InputDev();
}

_bool		Key_Pressing(_int _iKey)
{
	return CInputDev::GetInstance()->Key_Pressing(_iKey);
}
_bool		Key_Down(_int _iKey)
{
	return CInputDev::GetInstance()->Key_Down(_iKey);
}
_bool		Key_Up(_int _iKey)
{
	return CInputDev::GetInstance()->Key_Up(_iKey);
}
_bool		Mouse_Down(MOUSEKEYSTATE _MouseButton)
{
	return CInputDev::GetInstance()->Mouse_Down(_MouseButton);
}


HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,	 // 키값
	const _tchar* pFontType, // 서체
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}


inline void			Release_System(void)
{
	CInputMgr::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CInputDev::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}
