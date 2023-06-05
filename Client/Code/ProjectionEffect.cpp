#include "stdafx.h"
#include "ProjectionEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CProjectionEffect::CProjectionEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .05f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CProjectionEffect::~CProjectionEffect()
{
}

HRESULT CProjectionEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CProjectionEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta* .375f;
		iter->vPos += m_vMoveDir*fTimeDelta*5.f;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CProjectionEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CProjectionEffect::Render_Object(void)
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

HRESULT CProjectionEffect::Add_Component(void)
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

CProjectionEffect * CProjectionEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProjectionEffect *	pInstance = new CProjectionEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CProjectionEffect::Free(void)
{
	CPSystem::Free();
}

void CProjectionEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;
	
	/*_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);*/
	attribute->vPos = m_vProjectionEffectPos*1.f;
	attribute->vVelocity = m_vDir * 10.f;
	attribute->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = 2.5f;
}
//
//CProjectionEffect* m_pProjectionEffect = nullptr;
//if (!m_pProjectionEffect)
//m_pProjectionEffect = dynamic_cast<CProjectionEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ProjectionEffect"));
//
//_vec3 vPos;														//대쉬이펙트하려던것
//
//_vec3 vDir;
//m_pTransform->Get_Info(INFO_POS, &vPos);
//_vec3 min = { -1.0f ,-1.0f ,-1.0f };
//_vec3 max = { 1.0f ,1.0f ,.0f };
//
//m_pTransform->Get_Info(INFO_POS, &vPos);
//vPos.x -= 5.f;
//vPos.y += 5.f;
//vPos.z -= 5.f;
//for (_int i = -5; i < 5; i++)
//{
//	for (_int j = -5; j < 5; j++)
//	{
//		for (_int k = -5; k < 5; k++)
//		{
//			D3DXVec3Normalize(&min, &_vec3(i, j, k));
//
//			dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PclePos(vPos + _vec3(i, j, k));
//
//			dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PcleDir(-min);
//			dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PcleMoveDir(max);
//
//			m_pProjectionEffect->addParticle();
//
//
//		}
//	}
//}


// 발산 프로젝션
//CProjectionEffect* m_pProjectionEffect = nullptr;
//if (!m_pProjectionEffect)
//m_pProjectionEffect = dynamic_cast<CProjectionEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ProjectionEffect"));
//
//_vec3 vPos;													//대쉬이펙트하려던것		
//_vec3 vRootPos;
//_vec3 vDir;
//_vec3 min = { -1.0f ,-1.0f ,-1.0f };
//_vec3 max = { 1.0f ,1.0f ,.0f };
//m_pTransCom->Get_Info(INFO_POS, &vPos);
//vPos.z += 5.f;
//vRootPos = vPos;
//vRootPos.y = 10.f;
//vRootPos.z = 55.f;
//for (_int i = -5; i < 5; i++)
//{
//	for (_int j = -5; j < 5; j++)
//	{
//		for (_int k = -5; k < 5; k++)
//		{
//			//D3DXVec3Normalize(&min, &_vec3(i, j, k));
//
//			dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PclePos(vRootPos + _vec3(i, j, k)*0.01f);
//			dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PcleDir(_vec3(i, j, k));
//			dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PcleMoveDir((vPos - vRootPos)*0.1f);
//			m_pProjectionEffect->addParticle();
//		}
//	}
//}