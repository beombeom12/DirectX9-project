#include "stdafx.h"
#include "CloudEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "ObtainBullet.h"
#include "Weapon.h"

CCloudEffect::CCloudEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 1.f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CCloudEffect::~CCloudEffect()
{
}

HRESULT CCloudEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CCloudEffect::Update_Object(const _float & fTimeDelta)
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

		iter->vPos += iter->vVelocity * fTimeDelta;
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CCloudEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CCloudEffect::Render_Object(void)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	if (!m_particles.empty())
	{
		m_pTextureCom->Set_Texture(4);
		CPSystem::Render_Object();
	}
}

HRESULT CCloudEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(STATICPARTICLE_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ STATICPARTICLE_TEX, pComponent });

	return S_OK;
}

CCloudEffect * CCloudEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCloudEffect *	pInstance = new CCloudEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCloudEffect::Free(void)
{
	CPSystem::Free();
}

void CCloudEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CTransform* pTransform = nullptr;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);

	_float fCol = (rand() % 10) / 100.f;
	_vec3 min = _vec3(-0.6f, 0.01f, -0.6f);
	_vec3 max = _vec3(0.4f, 0.2f, 0.4f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	attribute->vPos = vCloudEffectPos + attribute->vVelocity / 1.f;
	attribute->vVelocity *= 0.3f;
	attribute->vPos += attribute->vVelocity;

	attribute->dwColor = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	// 5:4:2 ->갈색
	// 3:5:3 ->녹색 (독구름)
	//attribute->vPos.z = vCloudEffectPos.z + 0.5f;
	//attribute->vVelocity = vDir * 5000.f;

	//attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = 5.f;
}

