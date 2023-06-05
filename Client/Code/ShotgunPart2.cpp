#include "stdafx.h"
#include "..\Header\ShotgunPart2.h"
#include "Shotgun.h"

CShotgunPart2::CShotgunPart2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CShotgunPart2::~CShotgunPart2()
{
}

HRESULT CShotgunPart2::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Scale(0.02f, 0.015f, 0.0975f);

	return S_OK;
}

_int CShotgunPart2::Update_Object(const _float & fTimeDelta)
{
	if (dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Get_State())
	{
		Add_RenderGroup(RENDER_NONALPHA, this);
		CGameObject::Update_Object(fTimeDelta);
	}
	return 0;
}

void CShotgunPart2::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CShotgunPart2::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(47);
	m_pCube->Render_Buffer();
}

HRESULT CShotgunPart2::Add_Component(void)
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

CShotgunPart2 * CShotgunPart2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShotgunPart2* pInstance = new CShotgunPart2(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CShotgunPart2::Free(void)
{
	CGameObject::Free();
}


