#include "stdafx.h"
#include "..\Header\SpBullet.h"
#include "PoolMgr.h"
#include "Monster.h"

CSpBullet::CSpBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSpBullet::~CSpBullet()
{
}

HRESULT CSpBullet::Ready_Object(const _vec3 * pPos, const _vec3 * pDir, _float _fDamage, _float _fSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Scale(0.05f, 0.05f, 0.05f);
	m_pTransCom->m_vInfo[INFO_POS] = *pPos;
	m_vDirection = *pDir;
	m_fDamage = _fDamage;
	m_fSpeed = _fSpeed;
	m_bDamage = true;
	m_pTransCom->Static_Update();

	m_vDirection = *pDir;

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pShowTransCom->Set_Scale(0.05f, 0.01f, 0.05f);
	m_pShowTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pShowTransCom->Static_Update();

	return S_OK;
}

_int CSpBullet::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;
	Before_Update();

	if (m_fTimeDelta >= 5.f || m_bDamage == false)
	{
		m_bDamage = false;
		CPoolMgr::GetInstance()->Collect_PlayerBullet(this);
		m_fTimeDelta = 0.f;
		return -1;
	}

	CLayer*	pMonsterLayer = Engine::Get_Layer(STAGE_MONSTER);

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	_vec3 vScale;
	m_pTransCom->Get_Scale(&vScale);

	for (auto& iter : pMonsterLayer->Get_GameList())
	{
		//	구충돌 좀 봐야할듯
		CTransform* pTransform = dynamic_cast<CTransform*>(iter->Get_Component(L"HitBox_Transform", ID_DYNAMIC));
		CHitBox* pHitbox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));

		if (m_pCollision->Collision_Square(this->m_pTransCom, this->m_pHitbox, pTransform, pHitbox))
		{
			if(m_bDamage)
				dynamic_cast<CMonster*>(iter)->Set_Damaged(m_fDamage);
			m_bDamage = false;
			break;
		}
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Move_Pos(&(m_vDirection * fTimeDelta * m_fSpeed));
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pShowTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pShowTransCom->Chase_Target_By_Direction(&m_vDirection, 0.f, fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CSpBullet::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CSpBullet::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pShowTransCom->Get_WorldMatrixPointer());

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CSpBullet::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pShowTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"ShowTransCom", pComponent });

	/*pComponent = m_pBufferCom = dynamic_cast<CSphereTex*>(Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pComponent });*/

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(CUBETEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBETEX_COMP, pComponent });

	pComponent = m_pHitbox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"BULLET_TEX"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"BULLET_TEX", pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ COLLISION_COMP, pComponent });

	return S_OK;
}

void CSpBullet::Set_Pos(const _vec3 & vPos)
{
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_pShowTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pShowTransCom->Static_Update();

}

void CSpBullet::MoveToDir(const _vec3 & vDir)
{
	m_pTransCom->m_vInfo[INFO_POS] += vDir * m_fTimeDelta * m_fSpeed;
}

void CSpBullet::Before_Update()
{
}

CSpBullet * CSpBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, const _vec3 * pDir, _float _fDamage, _float _fSpeed)
{
	CSpBullet *   pInstance = new CSpBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPos, pDir, _fDamage, _fSpeed)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpBullet::Free(void)
{
	CGameObject::Free();
}
