#include "stdafx.h"
#include "IceEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "ObtainBullet.h"
#include "Weapon.h"

CIceEffect::CIceEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = .05f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CIceEffect::~CIceEffect()
{
}

HRESULT CIceEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CIceEffect::Update_Object(const _float & fTimeDelta)
{
	for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		//_float fX;	//아이템 x좌표
		//_float fY;	//아이템 y좌표
		//_float fZ;	//아이템 z좌표
		//_float fR = 4;	 //반지름
		//
		//원의방정식 pow((x-a),2) + pow((y-b),2) = pow(r,2)
		// sqrtf((x-a) + (y-b)) = sqrtf(r)
		//
		//
		
		//iter->vPos 
		_vec3 vtest;
		D3DXVec3Normalize(&vtest, &(iter->vVelocity));
		//if(D3DXVec3Length(&(iter->vVelocity)) <1)
			//iter->vPos += iter->vVelocity * fTimeDelta;
		if (!(-0.1f < iter->vPos.y && iter->vPos.y < 0.05f ))
		{
			iter->vPos += vtest * fTimeDelta  * .1f;
			if(iter->fAge > 3.0f)
			iter->vPos.y -= 5 * fTimeDelta*(iter->fAge-2)*(iter->fAge-2);
		}
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CIceEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CIceEffect::Render_Object(void)
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

HRESULT CIceEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"White_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"White_Tex", pComponent });

	return S_OK;
}

CIceEffect * CIceEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CIceEffect *	pInstance = new CIceEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CIceEffect::Free(void)
{
	CPSystem::Free();
}

void CIceEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CTransform* pTransform = nullptr;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);



	_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	attribute->vPos = vIceEffectPos + attribute->vVelocity / .325f;
	attribute->vVelocity *= -10.f;
	attribute->dwColor = D3DXCOLOR(GetRandomFloat(0.8f, 0.9f), GetRandomFloat(0.8f, 1.0f), GetRandomFloat(0.8f, 1.f), 1.0f);

	//attribute->vPos.z = vIceEffectPos.z + 0.5f;
	//attribute->vVelocity = vDir * 5000.f;

	//attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = 5.f;
}