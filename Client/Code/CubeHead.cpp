#include "stdafx.h"
#include "..\Header\CubeHead.h"
#include "Flight.h"

CCubeHead::CCubeHead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCubeHead::~CCubeHead()
{
}

HRESULT CCubeHead::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Set_Scale(0.1f, 0.1f, 0.1f);

	return S_OK;
}

_int CCubeHead::Update_Object(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);

	CGameObject* pGameObject = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));

	if (dynamic_cast<CFlight*>(pGameObject)->Get_Ending() == false)
		Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CCubeHead::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CCubeHead::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(1);
	m_pCube->Render_Buffer();
}

void CCubeHead::Key_Input(const _float & fTimeDelta)
{
	_long MoveX = Get_DIMouseMove(DIMS_X);
	_long MoveY = Get_DIMouseMove(DIMS_Y);
	_long MoveZ = Get_DIMouseMove(DIMS_Z);
	//_int _ia = 1;
	//MoveX += _ia;
	//_ia++;
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(MoveX / 10.f));
	m_pTransform->Rotation(ROT_X, D3DXToRadian(MoveY / 10.f));
}

HRESULT CCubeHead::Add_Component(void)
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

CCubeHead * CCubeHead::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeHead* pInstance = new CCubeHead(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CCubeHead::Free(void)
{
	CGameObject::Free();
}
