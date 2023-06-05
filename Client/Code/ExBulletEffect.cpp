#include "stdafx.h"
#include "ExBulletEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CExBulletEffect::CExBulletEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .025f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CExBulletEffect::~CExBulletEffect()
{
}

HRESULT CExBulletEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CExBulletEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		iter->vPos -= (iter->vVelocity) * fTimeDelta * _float(-1 + rand() % 3) * 2.f;
		//iter->vPos.y -= 0.01f* (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CExBulletEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CExBulletEffect::Render_Object(void)
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

HRESULT CExBulletEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Orange_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Orange_Tex", pComponent });

	return S_OK;
}

CExBulletEffect * CExBulletEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CExBulletEffect *   pInstance = new CExBulletEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CExBulletEffect::Free(void)
{
	CPSystem::Free();
}

void CExBulletEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	m_fGravity = 15.f;
	_vec3 min = _vec3(-1.f, -1.f, -1.f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	_float fRand = (rand() % 100)*0.001f;
	GetRandomVector(&m_vDir, &min, &max);

	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	attribute->vPos = m_vCubePatriclePos + m_vDir * 0.01f;
	attribute->vVelocity = m_vDir*GetRandomFloat(-1.f, 1.f)*2.5;
	attribute->dwColor = D3DXCOLOR(0.7f + fRand, .7f + fRand, .7f + fRand, 1.f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = .5f;
}
