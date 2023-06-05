#include "stdafx.h"
#include "..\Header\MBullet.h"
#include "PoolMgr.h"

CMBullet::CMBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CMBullet::~CMBullet()
{
}

HRESULT CMBullet::Ready_Object(const _vec3 * pPos, const _vec3 * pDir,const _float fSpeed, const _float fScale, const _float fDamage, const _float fInterval)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vInfo[INFO_POS] = *pPos;

	m_vDirection = *pDir;

	return S_OK;



}

_int CMBullet::Update_Object(const _float & fTimeDelta)
{

	_vec3 vPos;
	m_fTimeDelta += fTimeDelta;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	if (m_fTimeDelta >= 3.f)
	{
		CPoolMgr::GetInstance()->Collect_Obj(this);
		m_fTimeDelta = 0.f;

		return -1;
	}
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Move_Pos(&(m_vDirection * fTimeDelta * m_fSpeed));

	
	return 0;
	return _int();
}

void CMBullet::LateUpdate_Object(void)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	Engine::CGameObject::LateUpdate_Object();
}

void CMBullet::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pTextureCom->Set_Texture(3);

	m_pCubetexCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMBullet::Add_Component(void)
{

	CComponent* pComponent = nullptr;



	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubeMonsterBullet"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeMonsterBullet", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCubetexCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pCubetexCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	return S_OK;
}

void CMBullet::Set_Pos(const _vec3 & vPos)
{
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CMBullet::MoveToDir(const _vec3 & vDir)
{
	m_pTransCom->m_vInfo[INFO_POS] += vDir * m_fTimeDelta * m_fSpeed;
}

CMBullet * CMBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, const _vec3 * pDir, const _float fSpeed, const _float fScale, const _float fDamage, const _float fInterval)
{
	CMBullet *	pInstance = new CMBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPos, pDir, fSpeed, fScale, fDamage, fInterval)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMBullet::Free(void)
{
	CGameObject::Free();
}
