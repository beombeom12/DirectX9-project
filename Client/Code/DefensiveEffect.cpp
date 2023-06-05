#include "stdafx.h"
#include "DefensiveEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CDefensiveEffect::CDefensiveEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .025f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CDefensiveEffect::~CDefensiveEffect()
{
}

HRESULT CDefensiveEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CDefensiveEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;

		//if (iter->fAge <= 1.f)
		//	iter->vPos -= (iter->vVelocity) * iter->fAge*iter->fAge*fTimeDelta * 1.f;
		//if (iter->fAge > 1.f)
		{
			if (iter->fAge < .1f)
			{
				iter->vVelocity = (iter->vPos - m_vCenterPos)*0.1f;
				//D3DXVec3Normalize(&iter->vVelocity, &iter->vVelocity);
				iter->vPos -= (iter->vVelocity) *iter->fAge*iter->fAge* fTimeDelta * 500.f;
			}
			if (iter->fAge>.1f)
				iter->vPos -= (iter->vVelocity) *iter->fAge*iter->fAge* fTimeDelta * 1000.f;
		}

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CDefensiveEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CDefensiveEffect::Render_Object(void)
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

HRESULT CDefensiveEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"White_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"White_Tex", pComponent });

	/*pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"STATICPARTICLE_TEX"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"STATICPARTICLE_TEX", pComponent });*/
	return S_OK;
}

CDefensiveEffect * CDefensiveEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefensiveEffect *	pInstance = new CDefensiveEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDefensiveEffect::Free(void)
{
	CPSystem::Free();
}

void CDefensiveEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;


	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);


	GetRandomVector(&attribute->vVelocity, &min, &max);
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);
	attribute->vPos = m_vDefensiveEffectPos;
	attribute->vVelocity *= 2.f;
	attribute->dwColor = D3DXCOLOR(GetRandomFloat(0.8f, 1.0f), GetRandomFloat(0.f, 0.2f), GetRandomFloat(0.f, .2f), 1.0f);
	attribute->dwColor = D3DXCOLOR(.7f, .6f, 1.f, 1.f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = 5.f;
}

