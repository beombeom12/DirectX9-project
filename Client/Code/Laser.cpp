#include "stdafx.h"
#include "..\Header\Laser.h"
#include "PoolMgr.h"
#include "Explosion.h"
#include "TriggerParticle.h"
#include "LaserEffect.h"
#include "Monster.h"
#include "KrakenBoss.h"


CLaser::CLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLaser::~CLaser()
{
}

HRESULT CLaser::Ready_Object(const _vec3 * pPos, const _vec3 * pDir, _float _fSpeed, _int _iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(0.05f, 0.05f, 2.f);
	m_pTransCom->m_vInfo[INFO_POS] = *pPos;
	m_pTransCom->Static_Update();

	m_pHitboxTransCom->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pHitboxTransCom->m_vInfo[INFO_POS] = *pPos;
	m_pHitboxTransCom->Static_Update();

	m_vDirection = *pDir;
	m_fSpeed = _fSpeed*3.f;

	return S_OK;
}

_int CLaser::Update_Object(const _float & fTimeDelta)
{


	if (m_bDead)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);


		// À§Ä¡ ¹Ù²ãÁà¾ßÇÔ   
		Bomb_effect();
		Bomb_Collision();

		CPoolMgr::GetInstance()->Collect_Laser(this);
		return -1;
	}

	m_fTimeDelta += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Move_Pos(&(m_vDirection * fTimeDelta * m_fSpeed));

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pHitboxTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Chase_Target_By_Direction(&m_vDirection, 0.f, fTimeDelta);


	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CLaser::LateUpdate_Object(void)
{
	Collision_check();


	CGameObject::LateUpdate_Object();
}

void CLaser::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture();
	m_pCube->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pHitboxTransCom->Get_WorldMatrixPointer());
	//m_pHitbox->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CLaser::Set_Pos(const _vec3 & vPos)
{
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_pHitboxTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pHitboxTransCom->Static_Update();
}

void CLaser::Collision_check(void)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);
	map<const _tchar*, CGameObject*> mapWall = pLayer->Get_GameObjectMap();

	for (auto& iter : mapWall)
	{
		CTransform* pWallTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pWallTransform, );
		CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pWallBox, );

		if (m_pCollision->Collision_Square(this->m_pTransCom, this->m_pHitbox, pWallTransform, pWallBox))
		{
			m_bDead = true;
			return;
		}
	}

	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y <= 0.f)
	{
		m_bDead = true;
	}

}

void CLaser::Bomb_effect(void)
{
	_vec3 vPos;                             //¿¬±â   
	_vec3 vDir = { 0.f,1.0f,0.f };
	CTriggerParticle* pTriggerParticle = nullptr;
	CLaserEffect* m_pLaserEffect = nullptr;

	m_pTransCom->Get_Info(INFO_POS, &vPos);
	if (!pTriggerParticle)
		pTriggerParticle = dynamic_cast<CTriggerParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TriggerParticle"));

	dynamic_cast<CTriggerParticle*>(pTriggerParticle)->Set_PclePos(vPos);
	dynamic_cast<CTriggerParticle*>(pTriggerParticle)->Set_PcleDir(vDir);
	for (_int i = 0; i < 15; ++i)
	{
		pTriggerParticle->addParticle();
	}

	if (!m_pLaserEffect)
		m_pLaserEffect = dynamic_cast<CLaserEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"LaserEffect"));
	//vPos.x -= 5.f;
	//vPos.y += 5.f;
	//vPos.z -= 5.f;
	for (_float i = -6.f; i < 6.f; i++)
	{
		for (_float j = -6.f; j < 6.f; j++)
		{
			for (_float k = -6.f; k < 6.f; k++)
			{
				D3DXVec3Normalize(&vDir, &_vec3(i, j, k));

				dynamic_cast<CLaserEffect*>(m_pLaserEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.25);
				dynamic_cast<CLaserEffect*>(m_pLaserEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.05f);
				dynamic_cast<CLaserEffect*>(m_pLaserEffect)->Set_PcleDir(-vDir);

				m_pLaserEffect->addParticle();
				for (_int i = 0; i < 1; ++i)
				{
					pTriggerParticle->addParticle();
				}
			}
		}
	}
	for (_float i = -6.f; i < 6.f; i++)
	{
		for (_float j = -6.f; j < 6.f; j++)
		{
			for (_float k = -6.f; k < 6.f; k++)
			{
				D3DXVec3Normalize(&vDir, &_vec3(i, j, k));

				dynamic_cast<CLaserEffect*>(m_pLaserEffect)->Set_PclePos(vPos + _vec3(i, j*0.1f, k)*0.05f);

				dynamic_cast<CLaserEffect*>(m_pLaserEffect)->Set_PcleDir(vDir);

				m_pLaserEffect->addParticle();
			}
		}
	}

}

void CLaser::Bomb_Collision(void)
{

	if (!Get_Layer(STAGE_MONSTER)->Get_GameList().empty())
	{
		m_pTransCom->Static_Update();
		for (auto& iter : Get_Layer(STAGE_MONSTER)->Get_GameList())
		{
			_vec3 vDest, vSour;
			m_pTransCom->Get_Info(INFO_POS, &vDest);
			dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC))->Get_Info(INFO_POS, &vSour);
			_float fDistance = sqrtf((vDest.x - vSour.x) * (vDest.x - vSour.x) + (vDest.y - vSour.y) * (vDest.y - vSour.y) + (vDest.z - vSour.z) * (vDest.z - vSour.z));

			if (fDistance < 5.f)
			{
				dynamic_cast<CMonster*>(iter)->Set_CollisionDmg();
			}
		}
	}

}

HRESULT CLaser::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });


	pComponent = m_pCube = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(CUBETEX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBETEX_COMP, pComponent });

	pComponent = m_pHitboxTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"HitboxTransCom", pComponent });

	pComponent = m_pHitbox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pComponent });





	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Red_Tex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Red_Tex", pComponent });


	return S_OK;
}

CLaser * CLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, const _vec3 * pDir, _float _fSpeed, _int _iIndex)
{
	CLaser* pInstance = new CLaser(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pPos, pDir, _fSpeed, _iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLaser::Free(void)
{
	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	CGameObject::Free();
}