#include "stdafx.h"
#include "..\Header\Trap.h"


CTrap::CTrap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTrap::~CTrap()
{
}

HRESULT CTrap::Ready_Object(const _vec3 & vPos, _tchar* Name)
{
	return S_OK;
}

_int CTrap::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CTrap::LateUpdate_Object(void)
{
}

void CTrap::Render_Object(void)
{
}

void CTrap::Free(void)
{
	CGameObject::Free();
}
