#include "stdafx.h"
#include "CartridgeParticle.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "Weapon.h"

CCartridgeParticle::CCartridgeParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .05f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

}

CCartridgeParticle::~CCartridgeParticle()
{
}

HRESULT CCartridgeParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CCartridgeParticle::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
				
		iter->vPos -= (iter->vVelocity) * fTimeDelta* 7.5f;
		iter->vPos.y -= 0.01f* (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta;
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CCartridgeParticle::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CCartridgeParticle::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture(8);
		CPSystem::Render_Object();
	}
}

HRESULT CCartridgeParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"STATICPARTICLE_TEX"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"STATICPARTICLE_TEX", pComponent });

	return S_OK;
}

CCartridgeParticle * CCartridgeParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartridgeParticle *	pInstance = new CCartridgeParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCartridgeParticle::Free(void)
{
	CPSystem::Free();
}

void CCartridgeParticle::resetParticle(ATTRIBUTE * attribute)
{
	_float m_fHand = _float(rand() % 2);

	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	m_fGravity = 85.f;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	_vec3 vRand = { 0.f,0.f ,0.f };
	GetRandomVector(&vRand, &min, &max);
	D3DXVec3Normalize(&vRand, &vRand);

	//attribute->vPos = m_vCartridgeParticlePos +vRand;
	attribute->vVelocity =  m_vDir* 1.f;
	attribute->dwColor = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = .5f;




	CGameObject* pPlayer = nullptr;
	pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");
	NULL_CHECK_RETURN(pPlayer, );

	if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1")))
	{		
		if (m_fHand == 0)
		{
			pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_1", TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pTransform, );
			D3DXVECTOR3 vPos;
			pTransform->Get_Info(INFO_POS, &vPos);

			attribute->vPos = vPos - ((vRand)*0.05f);
			attribute->vVelocity = m_vDir* 1.f + ((vRand)*0.05f);
		}
		if (m_fHand == 1)
		{
			pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_1", TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pTransform, );
			D3DXVECTOR3 vPos1;
			pTransform->Get_Info(INFO_POS, &vPos1);

			attribute->vPos = vPos1 - ((vRand)*0.05f);
			attribute->vVelocity = m_vDir* 1.f + ((vRand)*0.05f);
		}
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);

		attribute->vPos = vPos - ((vRand)*0.05f);
		attribute->vVelocity = m_vDir* 1.f+((vRand)*0.2f);

	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_2", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);

		attribute->vPos = vPos - ((vRand)*0.05f);
	}
}

