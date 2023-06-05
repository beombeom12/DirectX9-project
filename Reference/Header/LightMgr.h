#pragma once
#include "Base.h"
#include "Light.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{

	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr();
	virtual ~CLightMgr();

public:
	HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* tLightInfo, const _uint& iIndex);
	
private:
	list<CLight*>	m_Lightlist;
	virtual void Free(void);
};

END