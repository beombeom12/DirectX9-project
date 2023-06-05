#include "stdafx.h"
#include "..\Header\CubeFoot.h"
#include "Flight.h"

CCubeFoot::CCubeFoot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCubeFoot::~CCubeFoot()
{
}

HRESULT CCubeFoot::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Set_Scale(0.05f, 0.1f, 0.05f);

	return S_OK;
}

_int CCubeFoot::Update_Object(const _float & fTimeDelta)
{
	m_pMyLayer = Engine::Get_Layer(STAGE_CHARACTER);

	CGameObject::Update_Object(fTimeDelta);

	CGameObject* pGameObject = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));

	if (dynamic_cast<CFlight*>(pGameObject)->Get_Ending() == false)
		Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CCubeFoot::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CCubeFoot::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_pMyLayer->Get_GameObject(L"L_FOOT") == this)
		m_pTexture->Set_Texture(7);
	else if (m_pMyLayer->Get_GameObject(L"R_FOOT") == this)
		m_pTexture->Set_Texture(9);

	m_pCube->Render_Buffer();
}

HRESULT CCubeFoot::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pCube = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pInstance });

	/*pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pInstance });*/

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"DOOMGUY"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"DOOMGUY", pInstance });

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });

	return S_OK;
}

CCubeFoot * CCubeFoot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeFoot* pInstance = new CCubeFoot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CCubeFoot::Free(void)
{
	CGameObject::Free();
}
