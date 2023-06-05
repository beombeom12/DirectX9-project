#include "stdafx.h"
#include "..\Header\FlightBulletParticle.h"


CFlightBulletParticle::CFlightBulletParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 0.3f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CFlightBulletParticle::~CFlightBulletParticle()
{
}

HRESULT CFlightBulletParticle::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CFlightBulletParticle::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->vPos += iter->vVelocity * fTimeDelta;

		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CFlightBulletParticle::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CFlightBulletParticle::Render_Object(void)
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

HRESULT CFlightBulletParticle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(BULLETPARTICLE_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ BULLETPARTICLE_TEX, pComponent });

	return S_OK;
}

CFlightBulletParticle * CFlightBulletParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlightBulletParticle *	pInstance = new CFlightBulletParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFlightBulletParticle::Free(void)
{
	CPSystem::Free();
}

void CFlightBulletParticle::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CGameObject* pFlight = nullptr;
	pFlight = Engine::Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER");
	NULL_CHECK_RETURN(pFlight, );

	CTransform* pTransform = nullptr;

	pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(pTransform, );
	m_iPosSet *= -1;
	if (m_iPosSet == 1)
	{
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);
		vPos.x -= 0.5f;
		attribute->vPos = vPos;
	}
	else
	{
		D3DXVECTOR3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);
		vPos.x += 0.5f;
		attribute->vPos = vPos;
	}

	D3DXVECTOR3 vDir;
	pTransform->Get_Info(INFO_LOOK, &vDir);

	attribute->vVelocity = -vDir * 100.f;

	attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = 5.f;
}
