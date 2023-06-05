
#include "stdafx.h"
#include "KrakenEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CKrakenEffect::CKrakenEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 0.15f;
	m_vbSize = 51200;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CKrakenEffect::~CKrakenEffect()
{
}

HRESULT CKrakenEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CKrakenEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		_vec3 vDirection = { 0.f, 0.5f, 0.f };
		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta * 1.7f;
		iter->vPos.y += vDirection.y * (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta * 4.5f;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CKrakenEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CKrakenEffect::Render_Object(void)
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

HRESULT CKrakenEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"White_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"White_Tex", pComponent });

	return S_OK;
}

CKrakenEffect * CKrakenEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKrakenEffect *	pInstance = new CKrakenEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CKrakenEffect::Free(void)
{
	CPSystem::Free();
}

void CKrakenEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	m_fGravity = 0.f;
	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.f, 1.0f);

	_vec3 vDirection = { 0.f, 1.f, 0.f };
	_float fRand = (rand() % 5)*0.1f;

	GetRandomVector(&attribute->vVelocity, &min, &max);
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	attribute->vPos = m_vCubePatriclePos + attribute->vVelocity * 3.f;
	attribute->vPos += m_vDir;
	attribute->vPos.y = attribute->vVelocity.y * 0.5f;
	attribute->dwColor = D3DXCOLOR(0.25f - fRand, 0.25f - fRand, 0.25f - fRand, 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = .5f;
}