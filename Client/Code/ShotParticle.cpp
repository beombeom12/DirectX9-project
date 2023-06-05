#include "stdafx.h"
#include "ShotParticle.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "Weapon.h"

CShotParticle::CShotParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 0.4f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CShotParticle::~CShotParticle()
{
}

HRESULT CShotParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CShotParticle::Update_Object(const _float & fTimeDelta)
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

void CShotParticle::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CShotParticle::Render_Object(void)
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

HRESULT CShotParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(SHOTPARTICLE_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SHOTPARTICLE_TEX, pComponent });

	return S_OK;
}

CShotParticle * CShotParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShotParticle *	pInstance = new CShotParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShotParticle::Free(void)
{
	CPSystem::Free();
}

void CShotParticle::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CGameObject* pPlayer = nullptr;
	pPlayer = Engine::Get_GameObject(STAGE_CHARACTER,L"PLAYER");
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

		attribute->dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

		attribute->dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

		attribute->dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		attribute->fAge = 0.0f;
		attribute->fLifeTime = 1.f; 
	}
}