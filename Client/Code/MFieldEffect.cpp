#include "stdafx.h"
#include "MFieldEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CMFieldEffect::CMFieldEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .25f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CMFieldEffect::~CMFieldEffect()
{
}

HRESULT CMFieldEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CMFieldEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		if (iter->vPos.y > 0.25)
		{
			iter->vPos += (iter->vVelocity) * fTimeDelta*25.f;
			iter->vPos += m_vMoveDir*fTimeDelta * 150;
			iter->vPos.y -= 5 * fTimeDelta*(iter->fAge)*(iter->fAge);
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

void CMFieldEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMFieldEffect::Render_Object(void)
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

HRESULT CMFieldEffect::Add_Component(void)
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

CMFieldEffect * CMFieldEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMFieldEffect *	pInstance = new CMFieldEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMFieldEffect::Free(void)
{
	CPSystem::Free();
}

void CMFieldEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;

	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	_vec3 vRand = _vec3(1.0f, 1.0f, 1.0f);
	_float fRand = (rand() % 3) * 0.3f;
	GetRandomVector(&vRand, &min, &max);
	
	attribute->vPos = m_vMFieldEffectPos + (vRand*2.f);
	attribute->vVelocity = (m_vDir-vRand) * 1.f;
	//attribute->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	attribute->dwColor = D3DXCOLOR(fRand, fRand, fRand, 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = 10.f;
}

