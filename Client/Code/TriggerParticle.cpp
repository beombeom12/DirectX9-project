#include "stdafx.h"
#include "TriggerParticle.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CTriggerParticle::CTriggerParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 0.07f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CTriggerParticle::~CTriggerParticle()
{
}

HRESULT CTriggerParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CTriggerParticle::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta * _float(rand() % 50) * 1.f;
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

void CTriggerParticle::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CTriggerParticle::Render_Object(void)
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

HRESULT CTriggerParticle::Add_Component(void)
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

CTriggerParticle * CTriggerParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriggerParticle *	pInstance = new CTriggerParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTriggerParticle::Free(void)
{
	CPSystem::Free();
}

void CTriggerParticle::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	m_fGravity = 35.f;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	_vec3 vRand = { 0.f,0.f ,0.f };
	_float fRand = (rand() % 3) * 0.1f;

	GetRandomVector(&vRand, &min, &max);
	
	attribute->vVelocity = m_vDir + vRand * 0.05f;
	attribute->vPos = m_vTriggerParticlePos - m_vDir+ vRand*.00005f; //+ attribute->vVelocity / 5.f;
	attribute->vPos.y += vRand.y*.05f; //+ attribute->vVelocity / 5.f;

	attribute->dwColor = D3DXCOLOR( fRand, fRand, fRand, 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = 1.f;
}

