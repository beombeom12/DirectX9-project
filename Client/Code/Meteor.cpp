#include "stdafx.h"
#include "SparkEffect.h"
#include "..\Header\Meteor.h"
#include "RcEffect.h"
#include "SparkEffect.h"
#include "FlameEffect.h"
#include "StaticCamera.h"


CMeteor::CMeteor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMeteor::~CMeteor()
{
}

HRESULT CMeteor::Ready_Object(const _vec3& Position)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_float fRand = (rand()%20)*0.1f;
	m_fSphereScale = 1.f;

	m_vTarget = Position;

	m_pTransCom->Set_Pos(Position.x + 10.f + fRand, Position.y + 10.f,Position.z+ fRand);

	m_pTransCom->Set_Scale(&_vec3(m_fSphereScale, m_fSphereScale, m_fSphereScale));

	m_fAttack = 10.f;
	m_bSphereSkill = true;

	
	return S_OK;
}

_int CMeteor::Update_Object(const _float & fTimeDelta)
{
	m_fParticleTimer += fTimeDelta;
	m_fTimer += fTimeDelta;
	_uint iResult = CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Chase_Target(&m_vTarget, 3.f, fTimeDelta);

	m_pTransCom->Get_Info(INFO_POS, &m_vPos);
	// 범위안에 여러개
	Add_RenderGroup(RENDER_ALPHA,this);

	if (m_vPos.y <= 1.f)
	{
		//사용법
		m_pEffect = CRcEffect::Create(m_pGraphicDev, EXPLOSION_EFT); //enum EFFECTID
		dynamic_cast<CRcEffect*>(m_pEffect)->Set_EffectPos(m_vPos.x, m_vPos.y, m_vPos.z);//EFFECT POS
		dynamic_cast<CRcEffect*>(m_pEffect)->Set_Scale(5.f);
		dynamic_cast<CRcEffect*>(m_pEffect)->Set_SingleUse();
		if (!m_pFlameEffectParticle)
			m_pFlameEffectParticle = dynamic_cast<CFlameEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlameEffect"));
		m_pFlameEffectParticle->Set_PclePos(m_vPos);
		for (_int i = 0; i < 25; ++i)
		{
			m_pFlameEffectParticle->addParticle();
		}

		_float fGunSound = .3f;
		Engine::PlaySoundGun(L"MeteorBomb.wav", SOUND_EFFECT, fGunSound);
		dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"))->CameraShaking();
		return -1;
	}
	Meteor_Effect();
	return iResult;
}

void CMeteor::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMeteor::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture();
	m_pSphereTex->Render_Buffer();
}

HRESULT CMeteor::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pSphereTex = dynamic_cast<CSphereTex*>(Engine::Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pComponent });

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Lava_Tex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Lava_Tex", pComponent });

	return S_OK;
}

void CMeteor::Meteor_Effect(void)
{
	_float fRand = 0.f;
	fRand = (rand() % 50)*0.05f;
	if (m_fParticleTimer > 0.75f)
	{
		if (!m_SparkEffectParticle)
			m_SparkEffectParticle = dynamic_cast<CSparkEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"SparkEffect"));
		m_SparkEffectParticle->Set_PclePos(m_vPos);
		for (_int i = 0; i < 70; ++i)
		{
			m_SparkEffectParticle->addParticle();
		}
		m_fParticleTimer = 0.f;
	}

	if (m_fTimer > 1.f)
	{
		m_pEffect = CRcEffect::Create(m_pGraphicDev, REDEFFECT_EFT); //enum EFFECTID
		dynamic_cast<CRcEffect*>(m_pEffect)->Set_Scale(5.f);
		dynamic_cast<CRcEffect*>(m_pEffect)->Set_EffectPos(m_vPos.x+ fRand, m_vPos.y, m_vPos.z+ fRand);//EFFECT POS
		dynamic_cast<CRcEffect*>(m_pEffect)->Set_SingleUse();

		
		m_fTimer = 0.f;
	}
	//dynamic_cast<CRcEffect*>(m_pEffect)->Dead_Timer(10.f);
}

CMeteor * CMeteor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& Position)
{
	CMeteor* pInstance = new CMeteor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Position)))
	{
		MSG_BOX("CMeteor Object Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMeteor::Free(void)
{
	CGameObject::Free();
}
