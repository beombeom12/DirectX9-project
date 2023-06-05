#include "stdafx.h"
#include "..\Header\ObtainBullet.h"
#include "Weapon.h"
#include "CubePlayer.h"
#include "Uzi.h"
#include "Sniper.h"
#include "Shotgun.h"

CObtainBullet::CObtainBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
}

CObtainBullet::~CObtainBullet()
{

}

HRESULT CObtainBullet::Ready_Object(const _vec3 & vPos)
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

_int CObtainBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Effect();

		if (dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"))->Get_Weapon())
		{
			if(dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"))->Get_Weapon() == 
				dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1")))
			{
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Get_Bullet();
			}
			else if (dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"))->Get_Weapon() ==
				dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
			{
				dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Get_Bullet();
			}
			else if(dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"))->Get_Weapon() ==
				dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
			{
				dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Get_Bullet();
			}
		}
		return -1;
	}

	CItem::Move_Item(fTimeDelta);
	CItem::Update_Object(fTimeDelta);
	return 0;
}

void CObtainBullet::LateUpdate_Object()
{
	CItem::LateUpdate_Object();
}

void CObtainBullet::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pDBufferCom->Render_Buffer();
}

HRESULT CObtainBullet::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pDBufferCom = dynamic_cast<CDynamicBuffer*>(Clone_Proto(BULLET_ITEM_BUFFER));
	NULL_CHECK_RETURN(m_pDBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ BULLET_ITEM_BUFFER, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(BULLET_ITEM_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ BULLET_ITEM_TEX, pComponent });

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

void CObtainBullet::Dead_Effect(void)
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

CObtainBullet * CObtainBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos)
{
	CObtainBullet* pInstance = new CObtainBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CObtainBullet::Free()
{
	CItem::Free();
}
