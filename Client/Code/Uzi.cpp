#include "stdafx.h"
#include "..\Header\Uzi.h"
#include "CubePlayer.h"

CUzi::CUzi(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}


CUzi::~CUzi()
{
}

HRESULT CUzi::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	

	m_tAbility = new GUNABILITY;

	m_tAbility->fBulletAttack =5.f;
	m_tAbility->fBulletRate = 0.1f;
	m_tAbility->fRemainBulletCnt = 300.f;
	m_tAbility->fBulletCount = 300.f;

	m_bEquiped = false;

	return S_OK;
}

_int CUzi::Update_Object(const _float & fTimeDelta)
{
	if (m_bEquiped)
	{
		m_fTimeDelta = fTimeDelta;

		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
		CGameObject::Update_Object(fTimeDelta);
		CCubePlayer* m_pPlayer = nullptr;
		
		if (dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER")))
		{
			CCubePlayer* m_pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
			_int iDmg = m_pPlayer->Get_DmgItem();
			m_tAbility->fBulletAttack = 5.f*(iDmg + 1);
		}
	}
	return 0;	
}

void CUzi::LateUpdate_Object(void)
{	
	if (m_bEquiped)
	{
		FAILED_CHECK_RETURN(Get_Parts(), );
		CGameObject::LateUpdate_Object();

		if (Engine::Get_GameObject(STAGE_GUN, L"UZI1") == this)
			TransAxisUzi1();
		else if (Engine::Get_GameObject(STAGE_GUN, L"UZI2") == this)
			TransAxisUzi2();
	}
}

void CUzi::Render_Object(void)
{	
}

void CUzi::TransAxisUzi1(void)
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

	m_pPart5->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart5->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart5->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart5->Static_Update();

	vWeaponPos = (vUp * -1.5f) + (vLook);
	vWeaponPos += vPos;

	m_pPart4->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart4->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart4->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart4->Static_Update();

	vWeaponPos = (vUp * -2.f) + (vLook * 0.4f);
	vWeaponPos += vPos;

	m_pPart3->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart3->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart3->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart3->Static_Update();

	vWeaponPos = (vUp * -1.8f) + (vLook * 1.8f);
	vWeaponPos += vPos;

	m_pPart2->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart2->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart2->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart2->Static_Update();

	vWeaponPos = (vUp * -3.f) + (vLook * 1.8f);
	vWeaponPos += vPos;

	m_pPart1->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart1->Set_Rotation(ROT_X, fRightArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart1->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart1->Static_Update();
}

void CUzi::TransAxisUzi2(void)
{
	CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
	_float fLookAngle, fLeftArmAngle, fRightArmAngle, fHandAngle;
	pPlayer->Get_Angle(fLookAngle, fLeftArmAngle, fRightArmAngle, fHandAngle);

	FAILED_CHECK_RETURN(Get_Parts(), );

	CTransform*		m_pLeftHandWorld = nullptr;
	m_pLeftHandWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"L_HAND", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pLeftHandWorld, );

	_vec3 vWeaponPos, vPos, vRight, vUp, vLook, vAngle, vScale;
	m_pLeftHandWorld->Get_Info(INFO_RIGHT, &vRight);
	m_pLeftHandWorld->Get_Info(INFO_UP, &vUp);
	m_pLeftHandWorld->Get_Info(INFO_LOOK, &vLook);
	m_pLeftHandWorld->Get_Info(INFO_POS, &vPos);
	m_pLeftHandWorld->Get_Scale(&vScale);

	vWeaponPos = vUp * -1.f;
	vWeaponPos += vPos;

	m_pPart5->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart5->Set_Rotation(ROT_X, fLeftArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart5->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart5->Static_Update();

	vWeaponPos = (vUp * -1.5f) + (vLook);
	vWeaponPos += vPos;

	m_pPart4->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart4->Set_Rotation(ROT_X, fLeftArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart4->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart4->Static_Update();

	vWeaponPos = (vUp * -2.f) + (vLook * 0.4f);
	vWeaponPos += vPos;

	m_pPart3->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart3->Set_Rotation(ROT_X, fLeftArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart3->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart3->Static_Update();

	vWeaponPos = (vUp * -1.8f) + (vLook * 1.8f);
	vWeaponPos += vPos;

	m_pPart2->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart2->Set_Rotation(ROT_X, fLeftArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart2->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart2->Static_Update();

	vWeaponPos = (vUp * -3.f) + (vLook * 1.8f);
	vWeaponPos += vPos;

	m_pPart1->Set_Pos(vWeaponPos.x, vWeaponPos.y, vWeaponPos.z);
	m_pPart1->Set_Rotation(ROT_X, fLeftArmAngle + D3DXToRadian(90.f) + fHandAngle);
	m_pPart1->Set_Rotation(ROT_Y, -fLookAngle);
	m_pPart1->Static_Update();
}

HRESULT CUzi::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	return S_OK;
}

HRESULT CUzi::Get_Parts(void)
{
	if (Engine::Get_GameObject(STAGE_GUN, L"UZI1") == this)
	{
		m_pPart1 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart1, E_FAIL);
		m_pPart2 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_2", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart2, E_FAIL);
		m_pPart3 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_3", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart3, E_FAIL);
		m_pPart4 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_4", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart4, E_FAIL);
		m_pPart5 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_1_5", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart5, E_FAIL);
	}
	else if (Engine::Get_GameObject(STAGE_GUN, L"UZI2") == this)
	{
		m_pPart1 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart1, E_FAIL);
		m_pPart2 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_2", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart2, E_FAIL);
		m_pPart3 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_3", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart3, E_FAIL);
		m_pPart4 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_4", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart4, E_FAIL);
		m_pPart5 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_2_5", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart5, E_FAIL);
	}
	else if (Engine::Get_GameObject(STAGE_GUN, L"UZI_FLOOR") == this)
	{
		m_pPart1 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_Floor_1", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart1, E_FAIL);
		m_pPart2 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_Floor_2", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart2, E_FAIL);
		m_pPart3 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_Floor_3", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart3, E_FAIL);
		m_pPart4 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_Floor_4", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart4, E_FAIL);
		m_pPart5 = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Uzi_Part_Floor_5", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pPart5, E_FAIL);
	}
	return S_OK;
}


void CUzi::Assemble(void)
{
	m_pTransform->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pTransform->Set_Pos(30.f, 1.f, 30.f);

	Add_RenderGroup(RENDER_ALPHA, this);

	FAILED_CHECK_RETURN(Get_Parts(), );

	_vec3 vBodyPos;
	m_pPart5->Set_Pos(30.f, 1.f, 30.f);
	m_pPart5->Get_BeforeInfo(INFO_POS, &vBodyPos);

	m_pPart1->Set_Pos(vBodyPos.x + A1, vBodyPos.y + A2, vBodyPos.z);
	m_pPart2->Set_Pos(vBodyPos.x + B1, vBodyPos.y + B2, vBodyPos.z);
	m_pPart3->Set_Pos(vBodyPos.x + C1, vBodyPos.y + C2, vBodyPos.z);
	m_pPart4->Set_Pos(vBodyPos.x + D1, vBodyPos.y + D2, vBodyPos.z);

	if (Get_DIKeyState(DIK_F1))
	{
		if (Get_DIKeyState(DIK_UP))
			A1 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			A1 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F2))
	{
		if (Get_DIKeyState(DIK_UP))
			A2 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			A2 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F3))
	{
		if (Get_DIKeyState(DIK_UP))
			B1 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			B1 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F4))
	{
		if (Get_DIKeyState(DIK_UP))
			B2 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			B2 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F5))
	{
		if (Get_DIKeyState(DIK_UP))
			C1 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			C1 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F6))
	{
		if (Get_DIKeyState(DIK_UP))
			C2 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			C2 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F7))
	{
		if (Get_DIKeyState(DIK_UP))
			D1 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			D1 -= 0.01f;
	}
	if (Get_DIKeyState(DIK_F8))
	{
		if (Get_DIKeyState(DIK_UP))
			D2 += 0.01f;
		if (Get_DIKeyState(DIK_DOWN))
			D2 -= 0.01f;
	}
}

void CUzi::Animation_Fire(void)
{
}

CUzi * CUzi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUzi* pInstance = new CUzi(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;	
}

void CUzi::Free(void)
{
	Safe_Delete<GUNABILITY*>(m_tAbility);
	CWeapon::Free();
}

