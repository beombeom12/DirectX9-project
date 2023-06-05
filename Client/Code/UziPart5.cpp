#include "stdafx.h"
#include "..\Header\UziPart5.h"
#include "Uzi.h"

CUziPart5::CUziPart5(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CUziPart5::~CUziPart5()
{
}

HRESULT CUziPart5::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Scale(.018f, .07f, .02f);


	return S_OK;
}

_int CUziPart5::Update_Object(const _float & fTimeDelta)
{
	if (dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Get_State() ||
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Get_State())
	{
		Add_RenderGroup(RENDER_NONALPHA, this);
		CGameObject::Update_Object(fTimeDelta);
	}
	return 0;
}

void CUziPart5::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUziPart5::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(71);
	m_pCube->Render_Buffer();
}

HRESULT CUziPart5::Add_Component(void)
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
	//m_mapComponent[ID_STATIC].insert({ TRANSFORM_COMP, pInstance });


	return S_OK;
}

CUziPart5 * CUziPart5::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUziPart5* pInstance = new CUziPart5(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CUziPart5::Free(void)
{
	CGameObject::Free();
}
