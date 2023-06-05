#include "stdafx.h"
#include "..\Header\SniperPart3.h"
#include "Sniper.h"

CSniperPart3::CSniperPart3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSniperPart3::~CSniperPart3()
{
}

HRESULT CSniperPart3::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransform->Set_Scale(2.1f, 0.45f, 0.21f);
	m_pTransform->Set_Scale(0.021f, 0.045f, 0.21f);

	return S_OK;
}

_int CSniperPart3::Update_Object(const _float & fTimeDelta)
{
	if (dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Get_State())
	{
		Add_RenderGroup(RENDER_NONALPHA, this);
		CGameObject::Update_Object(fTimeDelta);
	}
	return 0;
}

void CSniperPart3::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CSniperPart3::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(28);
	m_pCube->Render_Buffer();
}

HRESULT CSniperPart3::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pCube = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pInstance });

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pInstance });

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });


	return S_OK;
}

CSniperPart3 * CSniperPart3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSniperPart3* pInstance = new CSniperPart3(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CSniperPart3::Free(void)
{
	CGameObject::Free();
}


