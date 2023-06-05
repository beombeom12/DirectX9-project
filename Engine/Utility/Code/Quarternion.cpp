#include "..\Header\Quarternion.h"
#include "Export_Function.h"

USING(Engine)

CQuarternion::CQuarternion(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CQuarternion::CQuarternion(const CQuarternion & rhs)
	: CComponent(rhs)
{
	m_vecWorld = rhs.m_vecWorld;
}

CQuarternion::~CQuarternion()
{
}

HRESULT CQuarternion::Ready_Quarternion(void)
{
	return S_OK;
}

HRESULT CQuarternion::Add_World(D3DXMATRIX * _World)
{
	m_vecWorld.push_back(*_World);

	return S_OK;
}

CQuarternion * CQuarternion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuarternion* pInstance = new CQuarternion(pGraphicDev);

	if (FAILED(pInstance->Ready_Quarternion()))
	{
		MSG_BOX("QUATERNION Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CQuarternion::Clone(void)
{
	return new CQuarternion(*this);
}

void CQuarternion::Free(void)
{
	CComponent::Free();
}
