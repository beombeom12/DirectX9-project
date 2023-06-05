#include "stdafx.h"
#include "BulletParticle.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "Weapon.h"

CBulletParticle::CBulletParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 0.1f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CBulletParticle::~CBulletParticle()
{
}

HRESULT CBulletParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CBulletParticle::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->vPos += iter->vVelocity * fTimeDelta;

		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CBulletParticle::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CBulletParticle::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture(0);
		CPSystem::Render_Object();
	}
}

HRESULT CBulletParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(BULLETPARTICLE_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ BULLETPARTICLE_TEX, pComponent });

	return S_OK;
}

CBulletParticle * CBulletParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBulletParticle *	pInstance = new CBulletParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBulletParticle::Free(void)
{
	CPSystem::Free();
}

void CBulletParticle::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CGameObject* pPlayer = nullptr;
	pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");
	NULL_CHECK_RETURN(pPlayer, );

	CTransform* pTransform = nullptr;

	if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);

		D3DXVECTOR3 vDir;
		pTransform->Get_Info(INFO_LOOK, &vDir);

		attribute->vPos = vPos;
		attribute->vVelocity = vDir * 5000.f;

		attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

		attribute->fAge = 0.0f;
		attribute->fLifeTime = 1.f;
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);

		D3DXVECTOR3 vDir;
		pTransform->Get_Info(INFO_LOOK, &vDir);

		attribute->vPos = vPos + vDir;
		attribute->vVelocity = vDir * 1000.f;

		attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

		attribute->fAge = 0.0f;
		attribute->fLifeTime = 1.f;
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_2", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);

		D3DXVECTOR3 vDir;
		pTransform->Get_Info(INFO_LOOK, &vDir);

		attribute->vPos = vPos + vDir;
		attribute->vVelocity = vDir * 1000.f;

		attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

		attribute->fAge = 0.0f;
		attribute->fLifeTime = 1.f;
	}
}