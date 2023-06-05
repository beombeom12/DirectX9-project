#pragma once
#include "Engine_Include.h"
#include "Base.h" 
class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	CRITICAL_SECTION		Get_Crt()		{ return m_Crt;	}
	_bool					Get_Finish()	{ return m_bFinish; }
	LOADINGID				Get_LoadingID() { return m_eID; }


	_int					Get_CurPercentage() { return m_iCur; }
	_int					Get_MaxPercentage() { return m_iMax; }

public:
	HRESULT		Ready_Loading(LOADINGID eID);

	_uint		Loading_ForStage(void);
	_uint		Loading_ForBoss(void);
private:

public:
	static unsigned int	CALLBACK Thread_Main(void* pArg);

private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_Crt;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LOADINGID				m_eID;
	_bool					m_bFinish;


	_int					m_iCur = 0;
	_int					m_iMax = 0;

	_bool					m_Power = false;

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
	virtual void			Free(void);

};

