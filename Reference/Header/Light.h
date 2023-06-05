#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLight :	public CBase
{
public:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight();

public:
	HRESULT	Ready_Light(const D3DLIGHT9* tLightInfo, const _uint& iIndex);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	D3DLIGHT9			m_tLightInfo;
	_uint				m_iIndex;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* m_tLightInfo, const _uint& m_iIndex);
	virtual void Free(void);
};

END