#include "stdafx.h"
#include "ItemCubeEffect.h"
//#include "StaticCamera.h"
//#include "CubePlayer.h"

CItemCubeEffect::CItemCubeEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .005f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CItemCubeEffect::~CItemCubeEffect()
{
}

HRESULT CItemCubeEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CItemCubeEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta* .5f;		

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CItemCubeEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CItemCubeEffect::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture();
		CPSystem::Render_Object();
	}
}

HRESULT CItemCubeEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"LightGreen_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LightGreen_Tex", pComponent });

	/*pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"STATICPARTICLE_TEX"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"STATICPARTICLE_TEX", pComponent });*/
	return S_OK;
}

CItemCubeEffect * CItemCubeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItemCubeEffect *	pInstance = new CItemCubeEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItemCubeEffect::Free(void)
{
	CPSystem::Free();
}

void CItemCubeEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;

	/*_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);*/
	attribute->vPos = m_vItemCubeEffectPos*1.f;
	attribute->vVelocity = m_vDir * (_float)(-2+rand()%4);
	attribute->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//attribute->dwColor = D3DXCOLOR(GetRandomFloat(0.25f, 1.f), GetRandomFloat(0.5f, .75f), GetRandomFloat(0.25f, 1.f), 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = .5f;
}
