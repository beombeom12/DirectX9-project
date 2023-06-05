#include "stdafx.h"
#include "..\Header\Shotgun.h"
#include "CubePlayer.h"

CShotgun::CShotgun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}


CShotgun::~CShotgun()
{
}

HRESULT CShotgun::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);	

	m_tAbility = new GUNABILITY;

	m_tAbility->fBulletRate = 1.f;
	m_tAbility->fRemainBulletCnt = 50.f;
	m_tAbility->fBulletCount = 300.f;
	m_tAbility->fBulletAttack = 35.f;

	m_bEquiped = false;

	return S_OK;
}

_int CShotgun::Update_Object(const _float & fTimeDelta)
{
	if (m_bEquiped)
	{
		m_fTimeDelta = fTimeDelta;

		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		CGameObject::Update_Object(fTimeDelta);

		CCubePlayer* m_pPlayer=nullptr;
		
		if (dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER")))
		{
			CCubePlayer* m_pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
			_int iDmg = m_pPlayer->Get_DmgItem();
			m_tAbility->fBulletAttack = 35.f*(iDmg + 1);
		}
	}

	return 0;
}

void CShotgun::LateUpdate_Object(void)
{
	if (m_bEquiped)
	{
		FAILED_CHECK_RETURN(Get_Parts(), );
		CGameObject::LateUpdate_Object();

		TransAxisShotgun();
	}
}

void CShotgun::Render_Object(void)
{
}

HRESULT CShotgun::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	return S_OK;
}

HRESULT CShotgun::Get_Parts(void)
{
	m_pPart1 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_1", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart1, E_FAIL);
	m_pPart2 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_2", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart2, E_FAIL);
	m_pPart3 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_3", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart3, E_FAIL);
	m_pPart4 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Shotgun_Part_4", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart4, E_FAIL);
	return S_OK;
}


void CShotgun::Assemble(void)
{
	FAILED_CHECK_RETURN(Get_Parts(), );

	_vec3 vBodyPos;
	m_pPart4->Set_Pos(20.f, 0.5f, 10.f);
	m_pPart4->Get_BeforeInfo(INFO_POS, &vBodyPos);

	m_pPart1->Set_Pos(vBodyPos.x - 0.27f, vBodyPos.y + 0.06f, vBodyPos.z);
	m_pPart2->Set_Pos(vBodyPos.x - 0.12f, vBodyPos.y + 0.03f, vBodyPos.z);
	m_pPart3->Set_Pos(vBodyPos.x - 0.09f, vBodyPos.y + 0.01125f, vBodyPos.z);

}

void CShotgun::Animation_Fire(void)
{
}

void CShotgun::TransAxisShotgun(void)
{
	CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
	_float fLookAngle, fLeftArmAngle, fRightArmAngle, fHandAngle;
	pPlayer->Get_Angle(fLookAngle, fLeftArmAngle, fRightArmAngle, fHandAngle);

	FAILED_CHECK_RETURN(Get_Parts(), );

	CTransform*		m_pRightHandWorld = nullptr;
	m_pRightHandWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"R_HAND", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pRightHandWorld, );

	_vec3 vWeaponPos, vPos, vRight, vUp, vLook, vAngle, vScale;
	m_pRightHandWorld->Get_Info(INFO_RIGHT, &vRight);
	m_pRightHandWorld->Get_Info(INFO_UP, &vUp);
	m_pRightHandWorld->Get_Info(INFO_LOOK, &vLook);
	m_pRightHandWorld->Get_Info(INFO_POS, &vPos);
	m_pRightHandWorld->Get_Scale(&vScale);

	vWeaponPos = vUp * -1.f;
	vWeaponPos += vPos;

	//m_pPart5->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	//m_pPart5->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	//m_pPart5->Set_Rotation(ROT_Y, -fLookAngle);
	//m_pPart5->Static_Update();

	vWeaponPos = (vUp * -1.5f) + (vLook);
	vWeaponPos += vPos;

	m_pPart4->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart4->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart4->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart4->Static_Update();

	vWeaponPos = (vUp * -2.2f) + (vLook * 1.3f);
	vWeaponPos += vPos;

	m_pPart3->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart3->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart3->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart3->Static_Update();

	vWeaponPos = (vUp * -2.6f) + (vLook * 1.6f);
	vWeaponPos += vPos;

	m_pPart2->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart2->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart2->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart2->Static_Update();

	vWeaponPos = (vUp * -4.3f) + (vLook * 2.f);
	vWeaponPos += vPos;

	m_pPart1->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart1->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart1->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart1->Static_Update();
}

void CShotgun::Set_OnTerrain(void)
{
	/*m_pShotgunWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"ShotgunPart5", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pShotgunWorld, );

	_vec3		vPos;
	m_pShotgunWorld->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pShotgunWorld->Set_Pos(vPos.x, fHeight, vPos.z);*/
}


CShotgun * CShotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShotgun* pInstance = new CShotgun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CShotgun::Free(void)
{
	Safe_Delete<GUNABILITY*>(m_tAbility);
	CWeapon::Free();
}

