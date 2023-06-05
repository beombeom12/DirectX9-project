#include "stdafx.h"
#include "BrownCloudEffect.h"
#include "StaticCamera.h"
#include "CubePlayer.h"
#include "ObtainBullet.h"
#include "Weapon.h"

CBrownCloudEffect::CBrownCloudEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPSystem(pGraphicDev)
{
	m_fSize = 10.f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

CBrownCloudEffect::~CBrownCloudEffect()
{
}

HRESULT CBrownCloudEffect::Ready_Object(void)
{
	CPSystem::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CBrownCloudEffect::Update_Object(const _float & fTimeDelta)
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

		iter->vPos += iter->vVelocity * fTimeDelta ;
		iter->fAge += fTimeDelta;

		if (iter->fAge > iter->fLifeTime)
			iter->bAlive = false;
	}

	removeDeadParticles();

	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CBrownCloudEffect::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CBrownCloudEffect::Render_Object(void)
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

HRESULT CBrownCloudEffect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(STATICPARTICLE_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ STATICPARTICLE_TEX, pComponent });

	return S_OK;
}

CBrownCloudEffect * CBrownCloudEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBrownCloudEffect *	pInstance = new CBrownCloudEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBrownCloudEffect::Free(void)
{
	CPSystem::Free();
}

void CBrownCloudEffect::resetParticle(ATTRIBUTE * attribute)
{
	attribute->bAlive = true;

	CTransform* pTransform = nullptr;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);

	_float fCol = (rand() % 10) / 100.f;
	_vec3 min = _vec3(-1.f, -1.0f, -1.f);
	_vec3 max = _vec3(1.f, 0.02f, 1.f);
	GetRandomVector(&attribute->vVelocity, &min, &max);
	attribute->vPos = vBrownCloudEffectPos + attribute->vVelocity * 5.f;
	attribute->vVelocity *= 0.3f;
	attribute->vPos += attribute->vVelocity;

	attribute->dwColor = D3DXCOLOR(0.6f, 0.5f, 0.3f, 1.f);
	// 5:4:2 ->갈색
	// 3:5:3 ->녹색 (독구름)
	//attribute->vPos.z = vBrownCloudEffectPos.z + 0.5f;
	//attribute->vVelocity = vDir * 5000.f;

	//attribute->dwColor = D3DXCOLOR(1.f, 1.f, 0.5f, 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = 5.f;
}

