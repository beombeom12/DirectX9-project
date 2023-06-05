#include "..\..\Header\Interface.h"
#include "Export_Function.h"

CInterface::CInterface(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CInterface::~CInterface()
{
}

void Engine::CInterface::Free(void)
{
	CGameObject::Free();
}

void Engine::CInterface::LateUpdate_Object(void)
{

}

Engine::_int Engine::CInterface::Update_Object(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

HRESULT Engine::CInterface::Ready_Object(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixOrthoLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	return S_OK;
}
