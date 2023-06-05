#include "stdafx.h"
#include "TriggerFront.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "Weapon.h"

CTriggerFront::CTriggerFront(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .035f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CTriggerFront::~CTriggerFront()
{
}

HRESULT CTriggerFront::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CTriggerFront::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		/*iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta * 5.f;
		iter->vPos.y += 0.01f* (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}*/

		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta * m_fSpeed * _float(rand()%50) * 0.1f;
		iter->vPos.y += 0.01f* (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CTriggerFront::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CTriggerFront::Render_Object(void)
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

HRESULT CTriggerFront::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Red_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Red_Tex", pComponent });

	/*pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"STATICPARTICLE_TEX"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"STATICPARTICLE_TEX", pComponent });*/
	return S_OK;
}

CTriggerFront * CTriggerFront::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriggerFront *	pInstance = new CTriggerFront(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTriggerFront::Free(void)
{
	CPSystem::Free();
}

void CTriggerFront::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	_vec3 vRand = { 0.f,0.f ,0.f };
	_float fRand = (rand() % 5)*0.1f;
	_float m_fHand = _float(rand() % 2);

	GetRandomVector(&vRand, &min, &max);
	m_fGravity = 10.f;

	attribute->vVelocity = m_vDir + vRand*0.05f;
	attribute->vPos = m_vTriggerFrontPos - m_vDir + vRand*.00005f; //+ attribute->vVelocity / 5.f;
	attribute->vPos.y += vRand.y*.05f; //+ attribute->vVelocity / 5.f;

	attribute->dwColor = D3DXCOLOR(.8f- fRand, .8f- fRand, .8f- fRand, 1.f);
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
			D3DXVECTOR3 vDir;
			pTransform->Get_Info(INFO_LOOK, &m_vDir);

			m_fSpeed = 10.f;
			attribute->vPos = vPos - ((vRand)*0.05f);
			attribute->vVelocity = m_vDir* 1.f + ((vRand)*0.02f);
			attribute->fLifeTime = .25f;
		}
		if (m_fHand == 1)
		{
			pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_1", TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pTransform, );
			D3DXVECTOR3 vPos1;
			pTransform->Get_Info(INFO_POS, &vPos1);
			D3DXVECTOR3 vDir1;
			pTransform->Get_Info(INFO_LOOK, &vDir1);

			m_fSpeed = 10.f;
			attribute->vPos = vPos1 - ((vRand)*0.05f);
			attribute->vVelocity = vDir1* 1.f + ((vRand)*0.02f);
			attribute->fLifeTime = .25f;
		}
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);
		D3DXVECTOR3 vDir;
		pTransform->Get_Info(INFO_LOOK, &m_vDir);
		D3DXVec3Normalize(&vRand, &vRand);
		
		m_fSpeed = 10.f;
		attribute->vPos = vPos - ((vRand)*0.1f);
		attribute->vVelocity = m_vDir* 1.f - ((vRand)*0.05f);
		attribute->fLifeTime = .5f;
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
	{
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);
		D3DXVECTOR3 vDir;
		pTransform->Get_Info(INFO_LOOK, &m_vDir);
		D3DXVec3Normalize(&vRand, &vRand);

		m_fSpeed = 10.f;
		attribute->vPos = vPos - ((vRand)*0.1f);
		attribute->vVelocity = m_vDir* 1.f - ((vRand)*0.1f);
		attribute->fLifeTime = .5f;
	}
	
}

