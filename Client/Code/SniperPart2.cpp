#include "stdafx.h"
#include "..\Header\SniperPart2.h"
#include "Sniper.h"

CSniperPart2::CSniperPart2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSniperPart2::~CSniperPart2()
{
}

HRESULT CSniperPart2::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransform->Set_Scale(4.5f, 0.15f, 0.2f);
	m_pTransform->Set_Scale(0.02f, 0.015f, 0.45f);

	return S_OK;
}

_int CSniperPart2::Update_Object(const _float & fTimeDelta)
{
	if (dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Get_State())
	{
		Add_RenderGroup(RENDER_NONALPHA, this);
		CGameObject::Update_Object(fTimeDelta);
	}
	return 0;
}

void CSniperPart2::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CSniperPart2::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(28);
	m_pCube->Render_Buffer();
}

HRESULT CSniperPart2::Add_Component(void)
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

CSniperPart2 * CSniperPart2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSniperPart2* pInstance = new CSniperPart2(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CSniperPart2::Free(void)
{
	CGameObject::Free();
}


