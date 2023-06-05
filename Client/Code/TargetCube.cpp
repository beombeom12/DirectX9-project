#include "stdafx.h"
#include "..\Header\TargetCube.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "MonsterUI.h"
#include "RainbowCloudEffect.h"

CTargetCube::CTargetCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTargetCube::~CTargetCube()
{
}

HRESULT CTargetCube::Ready_Object(const _vec3& vPos, const _vec3& vDir, const _vec3& vScale, const _int& iIndex)
{
	m_tAbility = new MONSTERABILITY;
	m_tAbility->iLevel = 0;
	m_tAbility->fMaxHp = 25.f;
	m_tAbility->fCurrentHp = m_tAbility->fMaxHp;
	m_tAbility->fDamage = 0.f;
	m_tAbility->strObjTag = L"WALL";

	m_BeforeHp = m_tAbility->fMaxHp;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = vScale;
	m_pTransCom->Set_Scale(&m_vScale);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_iTexIndex = iIndex;
	m_vDir = vDir;

	m_pTransUICom->Set_Scale(1.f, 0.1f, 0.f);

	return S_OK;
}

_int CTargetCube::Update_Object(const _float & fTimeDelta)
{
	if (m_pMonsterUI == nullptr)
		m_pMonsterUI = dynamic_cast<CMonsterUI*>(Engine::Get_GameObject(STAGE_UI, L"MonsterUI"));

	_vec3 vCenter;
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS,&vPos);
	vCenter.y = vPos.y - m_vScale.y;

	if(vCenter.y < 0.f)
		m_pTransCom->Move_Pos(&(m_vDir * fTimeDelta));

	m_fFrame += fTimeDelta;

	_vec3 vUIPos;
	m_pTransCom->Get_Info(INFO_POS, &vUIPos);
	m_pTransUICom->Set_Pos(vUIPos.x, vUIPos.y + 0.5f, vUIPos.z);
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pTransUICom->Billboard_Transform(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	Hit_Check(fTimeDelta);

	if (m_bDead)
	{
		if (!m_pRainbowCloudEffectParticle)
			m_pRainbowCloudEffectParticle = dynamic_cast<CRainbowCloudEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"RainbowCloudEffect"));
		m_pRainbowCloudEffectParticle->Set_PclePos(vPos);
		for (_int i = 0; i < 25; ++i)
		{
			m_pRainbowCloudEffectParticle->addParticle();
		}
		return -1;
	}
	return 0;
}

void CTargetCube::LateUpdate_Object(void)
{
	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_pMonsterUI->Off_Switch();
		this->Kill_Obj();
	}
}

void CTargetCube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransUICom->Get_WorldMatrixPointer());
	m_pTextureUICom->Set_Texture();
	m_pBufferUICom->Resize_Buffer(m_tAbility->fCurrentHp / m_tAbility->fMaxHp);
	m_pBufferUICom->Render_Buffer();
}

HRESULT CTargetCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Engine::Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pComponent });

	// FOR UI
	pComponent = m_pTransUICom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformUICom", pComponent });

	pComponent = m_pBufferUICom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_MONTER_HP_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_MONTER_HP_COMP, pComponent });

	pComponent = m_pTextureUICom = dynamic_cast<CTexture*>(Clone_Proto(L"Monster_General_HP"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Monster_General_HP", pComponent });

	return S_OK;
}

void CTargetCube::Hit_Check(const _float& fTimeDelta)
{
	m_pTransCom->Static_Update();

	CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
	CWeapon* pWeapon = pPlayer->Get_Weapon();
	if (pWeapon)
	{
		_vec3 vSrcPos;
		pWeapon->Get_Transform()->Get_Info(INFO_POS, &vSrcPos);

		_vec3 vDir;
		if (m_pCollision->HitScan(g_hWnd, &vSrcPos, this->m_pBufferCom, this->m_pTransCom, &vDir))
		{
			if (pWeapon->Get_Shoot() == true)
			{
				m_tAbility->fCurrentHp -= pWeapon->Get_Ability()->fBulletAttack;
				pWeapon->Set_Shoot(false);
			}
		}
		if (m_BeforeHp != m_tAbility->fCurrentHp)
		{
			m_BeforeHp = m_tAbility->fCurrentHp;

			m_pMonsterUI->Set_Name(m_tAbility->strObjTag);
			m_pMonsterUI->Set_Hp(m_tAbility->fCurrentHp);
			m_pMonsterUI->Set_MaxHp(m_tAbility->fMaxHp);
			m_pMonsterUI->On_Switch();
		}
		else
		{
			m_fUISwitchTime += fTimeDelta;
			if (m_fUISwitchTime >= 5.f)
			{
				m_pMonsterUI->Off_Switch();
				m_fUISwitchTime = 0.f;
			}
		}
	}

	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_tAbility->fCurrentHp = 0.f;
	}
}

CTargetCube * CTargetCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vDir, const _vec3& vScale, const _int& iIndex)
{
	CTargetCube *	pInstance = new CTargetCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos,vDir,vScale, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTargetCube::Free(void)
{
	Safe_Delete<MONSTERABILITY*>(m_tAbility);
	CGameObject::Free();
}
