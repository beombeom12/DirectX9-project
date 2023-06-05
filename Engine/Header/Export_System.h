#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Include.h"
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"
#include "InputMgr.h"


BEGIN(Engine)

// GraphicDev
inline HRESULT			 Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);
inline void				 Render_Begin(D3DXCOLOR Color);
inline void				 Render_End(HWND hWnd = nullptr);
inline LPDIRECT3D9		 Get_D3D() { return CGraphicDev::GetInstance()->Get_D3D9(); }
inline LPDIRECT3DDEVICE9 Get_GraphicDev() { return CGraphicDev::GetInstance()->Get_GraphicDev(); }


// TimerMgr
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
inline HRESULT		Ready_Timer(const _tchar* pTimerTag);
inline void			Update_Timer(const _tchar* pTimerTag);

// FrameMgr
inline _bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

// InputMgr
inline void Ready_Input(HWND hwnd);
inline void Update_Input();

inline bool GetButton(KEY_TYPE key);
inline bool GetButtonDown(KEY_TYPE key);
inline bool GetButtonUp(KEY_TYPE key);

inline void SetAxisMode(bool bAxisMode);
inline _float GetVerticalAxis();
inline _float GetHorizontalAxis();

inline _bool		Key_Pressing(_int _iKey);
inline _bool		Key_Down(_int _iKey);
inline _bool		Key_Up(_int _iKey);
inline _bool		Mouse_Down(MOUSEKEYSTATE _MouseButton);


// InputDev

inline _byte		Get_DIKeyState(_ubyte byKeyID);
inline _byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID);
inline _long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState);
inline	HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void			SetUp_InputDev(void);

// FontMgr

inline HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,	 // 키값
	const _tchar* pFontType, // 서체
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color);

inline void			Release_System(void);


#include "Export_System.inl"

END

#endif // Export_System_h__
