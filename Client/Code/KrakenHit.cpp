#include "stdafx.h"
#include "..\Header\KrakenHit.h"


CKrakenHit::CKrakenHit(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .3f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}


CKrakenHit::~CKrakenHit()
{
}

HRESULT CKrakenHit::Ready_Object(void)
{

	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;

}

_int CKrakenHit::Update_Object(const _float & fTimeDelta)
{


	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta * 4.f;			
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

void CKrakenHit::LateUpdate_Object(void)
{
	CPSystem::LateUpdate_Object();
}

void CKrakenHit::Render_Object(void)
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

HRESULT CKrakenHit::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"LightGreen_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LightGreen_Tex", pComponent });

	return S_OK;
}

CKrakenHit * CKrakenHit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKrakenHit* pInstance = new CKrakenHit(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CKrakenHit::Free(void)
{
	CPSystem::Free();
}

void CKrakenHit::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	m_fGravity = 35.f;
	_vec3 min = _vec3(-1.0f, -2.f, -1.0f);
	_vec3 max = _vec3(2.0f, 2.0f, 2.0f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	attribute->vPos = m_vCubePatriclePos + attribute->vVelocity / 500.f;
	attribute->vPos.y -= attribute->vVelocity.y / 500.f;
	//attribute->vPos.x -= attribute->vVelocity.x / 500.f;
	attribute->dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = .5f;
}
