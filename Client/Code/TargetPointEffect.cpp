#include "stdafx.h"
#include "TargetPointEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CTargetPointEffect::CTargetPointEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .0125f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CTargetPointEffect::~CTargetPointEffect()
{
}

HRESULT CTargetPointEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CTargetPointEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		iter->fAge += fTimeDelta;
		iter->vPos += (iter->vVelocity) * fTimeDelta* (rand()%10000*0.001f)*0.25f;
		iter->vPos += m_vMoveDir*fTimeDelta*10.f;

		if (iter->fAge > iter->fLifeTime)
		{
			iter->bAlive = false;
		}
	}
	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CTargetPointEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CTargetPointEffect::Render_Object(void)
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

HRESULT CTargetPointEffect::Add_Component(void)
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

CTargetPointEffect * CTargetPointEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTargetPointEffect *	pInstance = new CTargetPointEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTargetPointEffect::Free(void)
{
	CPSystem::Free();
}

void CTargetPointEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;
	CTransform* pTransform = nullptr;

	/*_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);*/
	attribute->vPos = m_vTargetPointEffectPos*1.f;
	attribute->vVelocity = m_vDir * 10.f;
	attribute->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	attribute->fAge = 0.0f;
	attribute->fLifeTime = .25f;
}

//
//
//
////플레이어텔포
//if (!m_pTargetPointEffect)
//m_pTargetPointEffect = dynamic_cast<CTargetPointEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TargetPointEffect"));
//
//_vec3 vPos;														//대쉬이펙트하려던것
//
//_vec3 vDir;
//m_pTransform->Get_Info(INFO_POS, &vPos);
//_vec3 min = { -1.0f ,-1.0f ,-1.0f };
//_vec3 max = { 1.0f ,1.0f ,1.0f };
//
//m_pTransform->Get_Info(INFO_POS, &vPos);
///*vPos.x -= 5.f;
//vPos.y += 5.f;
//vPos.z -= 5.f;*/
//for (_int i = -5; i < 5; i++)
//{
//	for (_int j = -5; j < 5; j++)
//	{
//		for (_int k = -5; k < 5; k++)
//		{
//			//D3DXVec3Normalize(&min, &_vec3(i, j, k));						
//
//			dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.01f);
//
//			dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleDir(_vec3(i, j, k));
//			dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleMoveDir(max);
//
//			m_pTargetPointEffect->addParticle();
//
//
//		}
//	}
//}