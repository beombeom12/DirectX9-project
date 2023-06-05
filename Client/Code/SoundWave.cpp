#include "stdafx.h"
#include "SoundWave.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "TargetCube.h"
CSoundWave::CSoundWave(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 1.f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CSoundWave::~CSoundWave()
{
}

HRESULT CSoundWave::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CSoundWave::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->vPos += (iter->vVelocity) * fTimeDelta;
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CSoundWave::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CSoundWave::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture(2);
		CPSystem::Render_Object();
	}
}

HRESULT CSoundWave::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(STATICPARTICLE_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ STATICPARTICLE_TEX, pComponent });

	return S_OK;
}

CSoundWave * CSoundWave::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSoundWave *	pInstance = new CSoundWave(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSoundWave::Free(void)
{
	CPSystem::Free();
}

void CSoundWave::resetParticle(ATTRIBUTE * attribute)
{
	_vec3 vPos;
	_vec3 vDir;
	//pPlayerTransformCom->Get_Info(INFO_POS, &vPos);
	//pPlayerTransformCom->Get_Info(INFO_LOOK, &vDir);

	attribute->bAlive = true;

	attribute->vPos = m_vSoundWavePatriclePos;

	attribute->vVelocity = (vDir * 200.f);
	attribute->dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = 0.5f;
}
