#include "stdafx.h"
#include "..\Header\DeadParticle.h"


CDeadParticle::CDeadParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .05f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}


CDeadParticle::~CDeadParticle()
{
}

HRESULT CDeadParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CDeadParticle::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		
		_vec3 vtest;
		D3DXVec3Normalize(&vtest, &(iter->vVelocity));
		if (!(-0.1f < iter->vPos.y && iter->vPos.y < 0.05f))
		{
			iter->vPos += vtest * fTimeDelta  * .1f;
			if (iter->fAge > 0.30f)
				iter->vPos.y -= 3.f * fTimeDelta*(iter->fAge - 2)*(iter->fAge - 2);
		}
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CDeadParticle::LateUpdate_Object(void)
{
	CPSystem::LateUpdate_Object();
}

void CDeadParticle::Render_Object(void)
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

HRESULT CDeadParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"LightGreen_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LightGreen_Tex", pComponent });

	return S_OK;
}

CDeadParticle * CDeadParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDeadParticle *	pInstance = new CDeadParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDeadParticle::Free(void)
{
	CPSystem::Free();
}

void CDeadParticle::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CTransform* pTransform = nullptr;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);



	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	attribute->vPos = m_pDeadParticle + attribute->vVelocity/ 2.f;
	attribute->vVelocity *= -10.f;
	attribute->dwColor = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = 1.f;
}
