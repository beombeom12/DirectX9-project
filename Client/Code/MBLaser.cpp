#include "stdafx.h"
#include "MBLaser.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "TraceEffect.h"
CMBLaser::CMBLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .05f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CMBLaser::~CMBLaser()
{
}

HRESULT CMBLaser::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CMBLaser::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		_vec3 vRand;
		_vec3 min = _vec3(-1.f, -1.f, -1.f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
		GetRandomVector(&vRand, &min, &max);

		iter->fAge += GetRandomFloat(0.f, 2 * fTimeDelta)*1.5f;
		iter->vPos += (iter->vVelocity) * 1000 * fTimeDelta;
		//iter->vPos.y -= 0.01f* (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta;

		CTraceEffect* m_pTraceEffect = nullptr;
		if (!m_pTraceEffect)
			m_pTraceEffect = dynamic_cast<CTraceEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TraceEffect"));
		dynamic_cast<CTraceEffect*>(m_pTraceEffect)->Set_PclePos(iter->vPos);
		dynamic_cast<CTraceEffect*>(m_pTraceEffect)->Set_PcleDir(iter->vVelocity);
		//dynamic_cast<CTraceEffect*>(m_pTraceEffect)->Set_PcleDir(vSpotPos - vCorePos);

		for (_int i = 0; i < 25; ++i)
		{
			m_pTraceEffect->addParticle();
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

void CMBLaser::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMBLaser::Render_Object(void)
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

HRESULT CMBLaser::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Red_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Red_Tex", pComponent });

	return S_OK;
}

CMBLaser * CMBLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMBLaser *   pInstance = new CMBLaser(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMBLaser::Free(void)
{
	CPSystem::Free();
}

void CMBLaser::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	m_fGravity = 15.f;
	_vec3 min = _vec3(-1.f, -1.f, -1.f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

	D3DXVec3Normalize(&m_vDir, &m_vDir);
	attribute->vPos = m_vCubePatriclePos + m_vDir*GetRandomFloat(0.f, 10.f);
	attribute->vVelocity = m_vDir*GetRandomFloat(0.f, .35f);
	attribute->dwColor = D3DXCOLOR(GetRandomFloat(0.8f, 1.f), GetRandomFloat(0.8f, 1.f), GetRandomFloat(0.8f, 1.f), 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = 0.25f;
}