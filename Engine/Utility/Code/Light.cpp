#include "..\..\Header\Light.h"

USING(Engine)

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * tLightInfo, const _uint & iIndex)
{
	//	D3DLIGHT9 구조체와 조명의 인덱스를 받아와서 생성하고 추가하는 부분

	memcpy(&m_tLightInfo, tLightInfo, sizeof(D3DLIGHT9));
	m_iIndex = iIndex;

	m_pGraphicDev->SetLight(m_iIndex, &m_tLightInfo);	//	구조체의 정보로 조명 세팅
	m_pGraphicDev->LightEnable(m_iIndex, TRUE);			//	조명 사용 true

	return S_OK;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * m_tLightInfo, const _uint & m_iIndex)
{
	CLight* pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(m_tLightInfo, m_iIndex)))
	{
		MSG_BOX("Light Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);		//	조명 사용 false
	Safe_Release<LPDIRECT3DDEVICE9>(m_pGraphicDev);
}
