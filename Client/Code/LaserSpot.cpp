#include "stdafx.h"
#include "..\Header\LaserSpot.h"
#include "CubePlayer.h"

CLaserSpot::CLaserSpot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLaserSpot::~CLaserSpot()
{
}

HRESULT CLaserSpot::Ready_Object(const _vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(vPos.x, 0.1f, vPos.z);
	m_pTransCom->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pTransCom->Static_Update();

	return S_OK;
}

_int CLaserSpot::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerTrans == nullptr)
		m_pPlayerTrans = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));

	_vec3 vPlayerPos;
	m_pPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos.y = 0.1f;

	m_pTransCom->Chase_Target(&vPlayerPos, 5.f, fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CLaserSpot::LateUpdate_Object(void)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_CHARACTER);
	CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(pLayer->Get_GameObject(L"PLAYER"));

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	CHitBox* pPlayerHitbox = dynamic_cast<CHitBox*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", HITBOX_COMP, ID_STATIC));

	if (m_pCollision->Collision_Square(pPlayerTransform, pPlayerHitbox, m_pTransCom, m_pHitBox) && m_bAttack)
	{
		pPlayer->KnuckDown(10.f, 15.f);		
	}

	CGameObject::LateUpdate_Object();
}

void CLaserSpot::Render_Object(void)
{
}

HRESULT CLaserSpot::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeCol = dynamic_cast<CCubeCol*>(Clone_Proto(CUBECOL_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({CUBECOL_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	return S_OK;
}

CLaserSpot * CLaserSpot::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos)
{
	CLaserSpot* pInstance = new CLaserSpot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLaserSpot::Free(void)
{
	CGameObject::Free();
}
