#pragma once

#include "Engine_Include.h"
#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;
class CTimerMgr;

END

class CMainApp : public CBase
{
private:			// 积己磊, 家戈磊
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT			Ready_MainApp(void);
	Engine::_int	Update_MainApp(const Engine::_float& fTimeDelta);
	void			LateUpdate_MainApp(void);
	void			Render_MainApp(void);

private:
	HRESULT			SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement);

private:
	Engine::CGraphicDev*			m_pDeviceClass = nullptr;
	Engine::CManagement*			m_pManagementClass = nullptr;
	Engine::CTimerMgr*				m_pTimerMgr = nullptr;

	LPDIRECT3DDEVICE9				m_pGraphicDev;
	bool							m_bImgUI = false;

public:			// 按眉 积己 窃荐
	static		CMainApp*		Create(void);

private:		// 按眉 家戈 窃荐
	virtual		void			Free(void);
};

