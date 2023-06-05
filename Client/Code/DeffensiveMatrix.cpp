#include "stdafx.h"
#include "..\Header\DeffensiveMatrix.h"
#include "DefensiveEffect.h"
#include "RoundEffect.h"
CDeffensiveMatrix::CDeffensiveMatrix(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CDeffensiveMatrix::~CDeffensiveMatrix()
{
}

HRESULT CDeffensiveMatrix::Ready_Object(const _vec3 & Position)
{
	m_iSphereSkillTag = SKILL_SHIELD;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_float fGunSound = 1.f;
	Engine::PlaySoundGun(L"DeffensiveMatrix.wav", SOUND_EFFECT, fGunSound);
	m_vPos = Position;	
	m_vScale = { .5f,.5f,.5f };
	m_fSpeed = 5.f;
	m_fDuration = 3.f;
	m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	return S_OK;
}

_int CDeffensiveMatrix::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	m_fTimer += fTimeDelta;

	Scaling();
	Dead();
	
	if (m_bDead)
	{		
		DeadParticle();
		return-1;
	}
	return 0;
}

void CDeffensiveMatrix::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CDeffensiveMatrix::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
	
	/*m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);*/

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pTexture->Set_Texture(0);
	m_pCube->Render_Buffer();
	//m_pHitBox->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
	m_pCube->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT CDeffensiveMatrix::Add_Component(void)
{
	CComponent* pInstance = nullptr;	

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Shield_Tex"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Shield_Tex", pInstance });

	pInstance = m_pCube = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(CUBETEX_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBETEX_COMP, pInstance });

	pInstance = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });

	pInstance = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pInstance });

	pInstance = m_pCollision = dynamic_cast<CCollision*>(Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pInstance });


	pInstance = m_pHitBoxTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"HitBox_Transform22", pInstance });



	return S_OK;
}

void CDeffensiveMatrix::Scaling()
{
	if (m_fTimer < .5f)
	{
		m_pTransCom->Set_Angle(&m_vDir);

		m_pTransCom->Set_Scale(m_vScale.x + m_fSpeed*m_fTimer, m_vScale.y + m_fSpeed*m_fTimer, m_vScale.z + m_fSpeed*m_fTimer);
	}
}

void CDeffensiveMatrix::Dead()
{
	if (m_fTimer > m_fDuration)
		m_bDead = true;
}

void CDeffensiveMatrix::DeadParticle()
{
	_vec3 vPos;														//대쉬이펙트하려던것
	_vec3 vDir;
	_vec3 vScale;
	_vec3 min = { -1.0f ,-1.0f ,-1.0f };	
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Get_Scale(&vScale);

	CDefensiveEffect* pDefensiveEffect = nullptr;
	pDefensiveEffect = dynamic_cast<CDefensiveEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"DefensiveEffect"));
	for (_float i = -5.f; i < 5.f; i++)
	{
		for (_float j = -5.f; j < 5.f; j++)
		{
			for (_float k = -5.f; k < 5.f; k++)
			{
				D3DXVec3Normalize(&min, &_vec3(i, j, k));						

				dynamic_cast<CDefensiveEffect*>(pDefensiveEffect)->Set_PclePos(vPos + _vec3(i, j, k) * vScale.x / 4.f);

				dynamic_cast<CDefensiveEffect*>(pDefensiveEffect)->Set_CenterPos(vPos);

				dynamic_cast<CDefensiveEffect*>(pDefensiveEffect)->Set_PcleDir(-min);

				pDefensiveEffect->addParticle();
			}
		}
	}
}

CDeffensiveMatrix * CDeffensiveMatrix::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & Position)
{
	CDeffensiveMatrix* pInstance = new CDeffensiveMatrix(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Position)))
	{
		MSG_BOX("CDeffensiveMatrix Object Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDeffensiveMatrix::Free(void)
{
	CGameObject::Free();
}
