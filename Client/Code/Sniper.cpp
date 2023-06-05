#include "stdafx.h"
#include "..\Header\Sniper.h"

#include "CubePlayer.h"


CSniper::CSniper(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}


CSniper::~CSniper()
{
}

HRESULT CSniper::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
		
	m_tAbility = new GUNABILITY;

	m_tAbility->fBulletRate = 2.f;
	m_tAbility->fRemainBulletCnt = 50.f;
	m_tAbility->fBulletCount = 100.f;
	m_tAbility->fBulletAttack = 100.f;

	m_bEquiped = false;

	return S_OK;
}

_int CSniper::Update_Object(const _float & fTimeDelta)
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
			m_tAbility->fBulletAttack = 100.f*(iDmg + 1);
		}
	}

	return 0;
}

void CSniper::LateUpdate_Object(void)
{
	if (m_bEquiped)
	{
		FAILED_CHECK_RETURN(Get_Parts(), );

		TransAxis_Sniper();

		CGameObject::LateUpdate_Object();
	}
}

void CSniper::Render_Object(void)
{
}


HRESULT CSniper::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	return S_OK;
}

HRESULT CSniper::Add_Parts()
{
	if (!m_bPartInit)
	{
		CGameObject*m_pSniperCreate1 = CSniperPart1::Create(m_pGraphicDev);
		CGameObject*m_pSniperCreate2 = CSniperPart2::Create(m_pGraphicDev);
		CGameObject*m_pSniperCreate3 = CSniperPart3::Create(m_pGraphicDev);
		CGameObject*m_pSniperCreate4 = CSniperPart4::Create(m_pGraphicDev);


		TCHAR* szFinalName = new TCHAR[64];
		wsprintf(szFinalName, L"SniperPart1");
		Engine::Add_GameObject(L"Layer_GameLogic", m_pSniperCreate1, szFinalName);
		m_liszFinalName.push_back(szFinalName);

		TCHAR* szFinalName2 = new TCHAR[64];
		wsprintf(szFinalName2, L"SniperPart2");
		Engine::Add_GameObject(L"Layer_GameLogic", m_pSniperCreate2, szFinalName2);
		m_liszFinalName.push_back(szFinalName2);

		TCHAR* szFinalName3 = new TCHAR[64];
		wsprintf(szFinalName3, L"SniperPart3");
		Engine::Add_GameObject(L"Layer_GameLogic", m_pSniperCreate3, szFinalName3);
		m_liszFinalName.push_back(szFinalName3);

		TCHAR* szFinalName4 = new TCHAR[64];
		wsprintf(szFinalName4, L"SniperPart4");
		Engine::Add_GameObject(L"Layer_GameLogic", m_pSniperCreate4, szFinalName4);
		m_liszFinalName.push_back(szFinalName4);

		m_bPartInit = true;
		
	}
	return S_OK;
}

HRESULT CSniper::Get_Parts(void)
{
	m_pPart1 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_1", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart1, E_FAIL);
	m_pPart2 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_2", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart2, E_FAIL);
	m_pPart3 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_3", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart3, E_FAIL);
	m_pPart4 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_4", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPart4, E_FAIL);
	return S_OK;
}


void CSniper::Assemble(void)
{
	FAILED_CHECK_RETURN(Get_Parts(), );
	_vec3 vBodyPos;

	m_pPart4->Set_Pos(40.f, 2.f, 10.f);
	m_pPart4->Get_BeforeInfo(INFO_POS, &vBodyPos);

	m_pPart1->Set_Pos(vBodyPos.x - 1.8f*1.5f, vBodyPos.y + 0.65f*1.5f, vBodyPos.z);
	m_pPart2->Set_Pos(vBodyPos.x - 3.6f*1.5f, vBodyPos.y + 0.4f*1.5f, vBodyPos.z);
	m_pPart3->Set_Pos(vBodyPos.x - 2.f*1.5f, vBodyPos.y + 0.2f*1.5f, vBodyPos.z);
}

void CSniper::Animation_Fire(void)
{
}

void CSniper::TransAxis_Sniper(void)
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

	vWeaponPos = (vUp * -1.f) + (vLook);
	vWeaponPos += vPos;

	m_pPart4->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart4->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart4->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart4->Static_Update();

	vWeaponPos = (vUp * -3.5f) + (vLook * 2.);
	vWeaponPos += vPos;

	m_pPart3->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart3->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart3->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart3->Static_Update();

	vWeaponPos = (vUp * -10.f) + (vLook * 2.4f);
	vWeaponPos += vPos;

	m_pPart2->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart2->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart2->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart2->Static_Update();

	vWeaponPos = (vUp * -3.5f) + (vLook * 3.3f);
	vWeaponPos += vPos;

	m_pPart1->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart1->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart1->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart1->Static_Update();
}


void CSniper::Set_OnTerrain(void)
{
	/*m_pSniperWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"SniperPart5", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pSniperWorld, );

	_vec3		vPos;
	m_pSniperWorld->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pSniperWorld->Set_Pos(vPos.x, fHeight, vPos.z);*/
}


CSniper * CSniper::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSniper* pInstance = new CSniper(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSniper::Free(void)
{
	Safe_Delete<GUNABILITY*>(m_tAbility);
	CWeapon::Free();
}

