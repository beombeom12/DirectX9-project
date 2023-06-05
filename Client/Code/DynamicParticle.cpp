#include "stdafx.h"
#include "..\Header\DynamicParticle.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "TargetCube.h"
CDynamicParticle::CDynamicParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 1.f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CDynamicParticle::~CDynamicParticle()
{
}

HRESULT CDynamicParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CDynamicParticle::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CDynamicParticle::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CDynamicParticle::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture(m_iTextureIndex);
		CPSystem::Render_Object();
	}
}

HRESULT CDynamicParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_DynamicParticle"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_DynamicParticle", pComponent });

	return S_OK;
}

CDynamicParticle * CDynamicParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicParticle *	pInstance = new CDynamicParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDynamicParticle::Free(void)
{
	CPSystem::Free();
}

void CDynamicParticle::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	attribute->vPos = m_vDynamicPatriclePos;
	attribute->dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = 1.f;
}
