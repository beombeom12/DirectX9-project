#include "stdafx.h"
#include "..\Header\ObtainDefense.h"
#include "CubePlayer.h"

CObtainDefense::CObtainDefense(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
}

CObtainDefense::~CObtainDefense()
{
}

HRESULT CObtainDefense::Ready_Object(const _vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransCom->Set_Rotation(ROT_Z, D3DXToRadian(270.f));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_pHitBoxTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pHitBoxTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pHitBoxTransCom->Static_Update();

	return S_OK;
}

_int CObtainDefense::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Effect();
		dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"))->Get_Defense();
		return -1;
	}
	CItem::Move_Item(fTimeDelta);
	CItem::Update_Object(fTimeDelta);

	return 0;
}

void CObtainDefense::LateUpdate_Object()
{
	CItem::LateUpdate_Object();
}

void CObtainDefense::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pDBufferCom->Render_Buffer();
}

HRESULT CObtainDefense::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pDBufferCom = dynamic_cast<CDynamicBuffer*>(Clone_Proto(ADDDEFENCE_ITEM_BUFFER));
	NULL_CHECK_RETURN(m_pDBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ ADDDEFENCE_ITEM_BUFFER, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(ADDDEFENCE_ITEM_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ ADDDEFENCE_ITEM_TEX, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(m_pHitBox, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pHitBoxTransCom = dynamic_cast<CTransform*>(Clone_Proto(ITEM_TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pHitBoxTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ ITEM_TRANSFORM_COMP, pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	return S_OK;
}

void CObtainDefense::Dead_Effect(void)
{
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	if (!m_pItemParicle)
		m_pItemParicle = dynamic_cast<CItemParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ItemParticle"));
	m_pItemParicle->Set_PclePos(vPos);
	for (_int i = 0; i < 150; ++i)
	{
		m_pItemParicle->addParticle();
	}
}

CObtainDefense * CObtainDefense::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos)
{
	CObtainDefense*	pInstance = new CObtainDefense(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CObtainDefense::Free()
{
	CItem::Free();
}
