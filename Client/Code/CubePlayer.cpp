#include "stdafx.h"
#include "..\Header\CubePlayer.h"
#include "Export_Function.h"
#include "Weapon.h"
#include "PlayerMapping.h"
#include "PoolMgr.h"
#include "Bullet.h"
#include "Uzi.h"
#include "Shotgun.h"
#include "Sniper.h"
#include "EveryParticle.h"
#include "Inventory.h"
#include "Shop.h"

#include "Supporter_Uzi.h"
#include "Ping.h"
#include "Flight.h"

#include "HyperionStrike.h"
#include "Shield.h"

#include "StaticCamera.h"
#include "FlightCamera.h"
#include "ComboUI.h"

#include "Stage.h"
#include "FinalStage.h"

CCubePlayer::CCubePlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCubePlayer::~CCubePlayer()
{
}

HRESULT CCubePlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tAbility = new ABILITY;
	m_tAbility->fMaxHp = 100;
	m_tAbility->fHp = m_tAbility->fMaxHp;
	m_tAbility->fMaxDefence = 100;
	m_tAbility->fDefence = 50;
	m_tAbility->iGunTexture = 5;

	m_pTransform->Set_Scale(0.4f, 0.5f, 0.4f);
	m_pTransform->Set_Pos(20.f, 0.6f, 20.f);
	m_pTransform->Static_Update();

	m_pSphereTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pSphereTransCom->Set_Pos(20.f, 0.6f, 20.f);
	m_pSphereTransCom->Static_Update();
	m_fSpeed = 7.f;

	m_bUzi = false;
	m_bShotgun = false;
	m_bSniper = false;
	SetCheckFrustum(false);

	return S_OK;
}

_int CCubePlayer::Update_Object(const _float & fTimeDelta)
{
	Update_NullCheck();

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	if (m_pComboUI != nullptr)
	{
		if (m_pComboUI->Get_ComboCnt() == 0.f)
		{
			m_fCombo += 0.05f;
			if (m_fCombo >= 1.f)
				m_fCombo = 1.f;
			m_fGreen = m_fCombo;
			m_fBlue = m_fCombo;
			m_fRange = 40.f;
		}
		else
		{
			m_fCombo = 0.f;
			m_fGreen = 1.f - (m_pComboUI->Get_ComboCnt() * 0.05f) + m_fCombo;
			m_fBlue = 1.f - (m_pComboUI->Get_ComboCnt() * 0.05f) + m_fCombo;
		
			if (m_fGreen > 0.6f)
				m_fGreen = 0.6f;
			if (m_fBlue > 0.6f)
				m_fBlue = 0.6f;

			m_fRange = 40.f;
			m_fCombo = (1.f - m_pComboUI->Get_ComboCnt() * 0.05f);
		}
	}

	m_fTimeDelta = fTimeDelta;
	m_fBulletTime += fTimeDelta;
	m_fLaserTimer += fTimeDelta;
	FAILED_CHECK_RETURN(Get_BodyTransform(), -1);
	Inventory_Check();
	CoolTimer();

	// 이동, 애니메이션 관련
	if (!(dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch())
		&& !(dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch()))
	{
		if (m_pBomb->Get_WorldMap() == false && (m_iKnuckStack == 0) && m_pFlight->Get_Control() == false)
		{
			Move();
		}
	}
	Key_Skill();
	Look_Direction();

	if (!(dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch())
		&& !(dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch()))
	{
		Animation();
	}

	Assemble();

	if (Get_DIKeyState(DIK_UP))
	{
		//	넉백 테스트
		//KnuckDown(1);
		//SlowDown(1);
	}
	if (Get_DIKeyState(DIK_LSHIFT))
	{
		//	대시 테스트, 몬스터 방향으로만 가능하게 변경 예정
		Dash();

	}

	CGameObject* pGameObject = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));

	if (dynamic_cast<CFlight*>(pGameObject)->Get_Ending() == false)
		m_pGraphicDev->LightEnable(1, true);
	else 
		m_pGraphicDev->LightEnable(1, false);

	Lighting();
	CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CCubePlayer::LateUpdate_Object(void)
{
	if (!m_bJump)
		Set_OnTerrain();

	TransAxis();	//	월드함수 갈아엎으려면 Update 이후에 작업해줘야함

	if (m_Weapon)
	{
		if (m_Weapon->Get_Ability()->fBulletRate - (m_fBulletTime*(m_iSpeedItem + 1)) <= 0.f)
		{
			Fire_Bullet();
		}
	}

	m_pCollision->Get_Item();
	m_pCollision->Get_GunItem();

	Gun_Check();

	CGameObject::LateUpdate_Object();
}

void CCubePlayer::Render_Object(void)
{
	FAILED_CHECK_RETURN(Set_Material(), );
}

HRESULT CCubePlayer::Set_Material()
{
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));

	Material.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Material.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Material.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	Material.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	Material.Power = 0.f;

	m_pGraphicDev->SetMaterial(&Material);

	return S_OK;
}

void CCubePlayer::Key_Skill()
{
	if (Key_Down(DIK_F))
	{
		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"UZI1") && m_iSkillEnforce == 1)
		{
			if (static_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"))->Get_MainCam())
				static_cast<CFlight*>(Engine::Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER"))->Set_Control();
			else
			{
				static_cast<CFlight*>(Engine::Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER"))->Set_Control();
				static_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"))->Set_MainCam(true);
				static_cast<CFlightCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlightCamera"))->Set_MainCam(false);
			}
		}
		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN") && m_iSkillEnforce == 3)
		{
			_vec3 vPos;
			m_pTransform->Get_Info(INFO_POS, &vPos);
			CLayer* pLayer = Get_Layer(STAGE_SKILL);
			CGameObject* pGameObject = nullptr;

			if (Key_Pressing(DIK_W))
			{
				
				CGameObject* pGameObject = CHyperionStrike::Create(m_pGraphicDev, vPos, DIR_PX);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
			}
			else if (Key_Pressing(DIK_S))
			{
				CGameObject* pGameObject = CHyperionStrike::Create(m_pGraphicDev, vPos, DIR_MX);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
			}
			else if (Key_Pressing(DIK_A))
			{
				CGameObject* pGameObject = CHyperionStrike::Create(m_pGraphicDev, vPos, DIR_PZ);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
			}
			else if (Key_Pressing(DIK_D))
			{
				CGameObject* pGameObject = CHyperionStrike::Create(m_pGraphicDev, vPos, DIR_MZ);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
			}
			else
			{
				CGameObject* pGameObject = CHyperionStrike::Create(m_pGraphicDev, vPos, DIR_END);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
			}
		}
		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER") && m_iSkillEnforce == 5)
		{
			_float fGunSound = 1.f;
			Engine::PlaySoundGun(L"Meteor.wav", SOUND_EFFECT, fGunSound);
			dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Switch_Worldmap();
		}
	}
}

void CCubePlayer::CoolTimer(void)
{
	m_fGlobal_Cooltime += m_fTimeDelta;
	m_iKnuckStack -= 1;
	m_iDashStack -= 1;

	if (m_fGlobal_Cooltime >= 1.f)
		m_fGlobal_Cooltime = 1.f;
	if (m_iKnuckStack < 0)
		m_iKnuckStack = 0;
	if (m_iDashStack < 0)
		m_iDashStack = 0;
	if (m_fSpeed < 10.f)
		m_fSpeed += 1.f;


	_vec3 vPos;
	m_pBodyWorld->Get_Info(INFO_POS, &vPos);

	if (vPos.x >= 129.f || vPos.x <= -0.1f || vPos.z >= 129.f || vPos.z <= -0.1f)
	{
		m_iKnuckStack = 0;
		m_iDashStack = 20;
	}

	_vec3 vLook;
	m_pBodyWorld->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	m_pBodyWorld->Move_Pos(&(vLook* _float(m_iDashStack) * m_fTimeDelta));

	vLook *= -1.f;
	m_pBodyWorld->Move_Pos(&(vLook * _float(m_iKnuckStack) * m_fTimeDelta));
}

void CCubePlayer::KnuckDown(const _float & fDamage, const _float& fDistance)
{

	if (m_fGlobal_Cooltime >= 1.f)
	{
		m_tAbility->fHp -= fDamage;
		m_iKnuckStack = _int(fDistance);
		dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"))->HitPlayer();
		m_fGlobal_Cooltime = 0;
	}
}

void CCubePlayer::SlowDown(const _float & fDamage)
{
	if (m_fGlobal_Cooltime >= 1.f)
	{
		m_tAbility->fHp -= fDamage;
		m_fGlobal_Cooltime = 0;
	}
	m_fSpeed = 3.f;
}

void CCubePlayer::Dash(void)
{
	if (D3DXVec3Length(&m_vDirection) != 0)
	{
		m_iDashStack = 10;
		_vec3 vPos;														//대쉬이펙트
		_vec3 vDir;
		m_pTransform->Get_Info(INFO_POS, &vPos);
		m_pBodyWorld->Get_Info(INFO_LOOK, &vDir);
		vPos.y = +0.45f;
		dynamic_cast<CDashCube*>(m_pDashCube)->Set_PclePos(vPos);
		dynamic_cast<CDashCube*>(m_pDashCube)->Set_PcleDir(vDir);
		for (_int i = 0; i < 150; ++i)
		{
			m_pDashCube->addParticle();
		}
	}
}

void CCubePlayer::Update_NullCheck()
{
	if (!m_pShotParicle)
		m_pShotParicle = dynamic_cast<CShotParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ShotParticle"));

	if (!m_pBulletParicle)
		m_pBulletParicle = dynamic_cast<CBulletParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"BulletParticle"));

	if (!m_pProjectileParicle)
		m_pProjectileParicle = dynamic_cast<CProjectileParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ProjectileParticle"));

	if (!m_pCubeParticle)
		m_pCubeParticle = dynamic_cast<CCubeParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"CubeParticle"));

	if (!m_pCartridgeParticle)
		m_pCartridgeParticle = dynamic_cast<CCartridgeParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"CartridgeParticle"));

	if (!m_pDashCube)
		m_pDashCube = dynamic_cast<CDashCube*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"DashCube"));
	if (!m_pRoundEffect)
		m_pRoundEffect = dynamic_cast<CRoundEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"RoundEffect"));

	if (!m_pTriggerParticle)
		m_pTriggerParticle = dynamic_cast<CTriggerParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TriggerParticle"));

	if (!m_pTriggerFront)
		m_pTriggerFront = dynamic_cast<CTriggerFront*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TriggerFront"));

	if (!m_pTraceEffect)
		m_pTraceEffect = dynamic_cast<CTraceEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TraceEffect"));

	if (!m_pLaserPoint)
		m_pLaserPoint = dynamic_cast<CLaserPoint*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"LaserPoint"));

	if (m_pComboUI == nullptr)
		m_pComboUI = dynamic_cast<CComboUI*>(Engine::Get_GameObject(STAGE_UI, L"ComboUI"));

	Player_Mapping();

	if (m_pBomb == nullptr)
		m_pBomb = dynamic_cast<CPlayerMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"Map"));

	if (m_pFlight == nullptr)
		m_pFlight = dynamic_cast<CFlight*>(Engine::Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER"));
}

void CCubePlayer::Set_OnTerrain(void)
{
	m_pBodyWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, );

	_vec3		vPos;
	m_pBodyWorld->Get_BeforeInfo(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", TERRAINTEX_COMP, ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ) + 0.6f;

	m_pBodyWorld->Set_Pos(vPos.x, fHeight, vPos.z);
}

void CCubePlayer::Assemble(void)
{
	FAILED_CHECK_RETURN(Get_BodyTransform(), );

	_vec3 vBodyPos, vBodyAfterPos;
	m_pBodyWorld->Get_BeforeInfo(INFO_POS, &vBodyPos);
	m_pBodyWorld->Get_Info(INFO_POS, &vBodyAfterPos);

	m_pTransform->Set_Pos(vBodyPos.x, vBodyPos.y, vBodyPos.z);
	m_pSphereTransCom->Set_Pos(vBodyPos.x, vBodyPos.y, vBodyPos.z);
	m_pHeadWorld->Set_Pos(vBodyPos.x, vBodyPos.y + 0.3f, vBodyPos.z);
	m_pLeftArmWorld->Set_Pos(vBodyPos.x - 0.15f, vBodyPos.y + 0.1f, vBodyPos.z);
	m_pRightArmWorld->Set_Pos(vBodyPos.x + 0.15f, vBodyPos.y + 0.1f, vBodyPos.z);
	m_pLeftLegWorld->Set_Pos(vBodyPos.x - 0.05f, vBodyPos.y - 0.3f, vBodyPos.z);
	m_pRightLegWorld->Set_Pos(vBodyPos.x + 0.05f, vBodyPos.y - 0.3f, vBodyPos.z);

	m_pLeftHandWorld->Set_Pos(vBodyPos.x - 0.15f, vBodyPos.y - 0.1f, vBodyPos.z);
	m_pRightHandWorld->Set_Pos(vBodyPos.x + 0.15f, vBodyPos.y - 0.1f, vBodyPos.z);
	m_pLeftFootWorld->Set_Pos(vBodyPos.x - 0.05f, vBodyPos.y - 0.5f, vBodyPos.z);
	m_pRightFootWorld->Set_Pos(vBodyPos.x + 0.05f, vBodyPos.y - 0.5f, vBodyPos.z);

	if (m_bFirst)
	{
		m_bFirst = false;

		m_pLeftArmWorld->Static_Update();
		m_pRightArmWorld->Static_Update();
		m_pLeftLegWorld->Static_Update();
		m_pRightLegWorld->Static_Update();
		m_pLeftHandWorld->Static_Update();
		m_pRightHandWorld->Static_Update();
		m_pLeftFootWorld->Static_Update();
		m_pRightHandWorld->Static_Update();
	}
}

void CCubePlayer::Animation(void)
{
	if (Get_DIKeyState(DIK_W) || Get_DIKeyState(DIK_A) || Get_DIKeyState(DIK_S) || Get_DIKeyState(DIK_D) || Get_DIKeyState(DIK_SPACE) || Get_DIKeyState(DIK_LSHIFT))
	{
		if (m_fLeftArmAngle > 0.6f)
			m_bWalkAngle = false;
		if (m_fLeftArmAngle < -0.6f)
			m_bWalkAngle = true;

		if (m_bWalkAngle == true)
			m_fLeftArmAngle += m_fTimeDelta * 4.f;
		else if (m_bWalkAngle == false)
			m_fLeftArmAngle -= m_fTimeDelta * 4.f;

		m_fRightArmAngle = -m_fLeftArmAngle;

		if (m_fLeftLegAngle > 0.6f)
			m_bWalkAngle_Leg = false;
		if (m_fLeftLegAngle < -0.6f)
			m_bWalkAngle_Leg = true;

		if (m_bWalkAngle_Leg == true)
			m_fLeftLegAngle += m_fTimeDelta * 4.f;
		else if (m_bWalkAngle_Leg == false)
			m_fLeftLegAngle -= m_fTimeDelta * 4.f;

		m_fRightLegAngle = -m_fLeftLegAngle;

		if (m_fHandAngle > -D3DX_PI / 2.f)
			m_fHandAngle -= m_fTimeDelta * 4;
	}
	if (Get_DIMouseState(DIM_RB) ||
		(m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")) ||
		(m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
	{
		// 우지 견착
		if ((m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"UZI1")))
		{
			m_fLeftArmAngle = D3DXToRadian(-90.f) + m_fDownAngle;
			m_fRightArmAngle = D3DXToRadian(-90.f) + m_fDownAngle;
			m_fHandAngle = 0.f;

		}
		// 샷건 / 스나 견착
		if ((m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")) ||
			(m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
		{
			m_fLeftArmAngle = D3DXToRadian(-75.f) + m_fDownAngle;
			m_fRightArmAngle = m_fDownAngle;
			m_fHandAngle = D3DXToRadian(-90.f);
		}
	}

	if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))
	{
		if (Get_DIMouseState(DIM_RB))
			m_bSinperZoom = true;
		else
			m_bSinperZoom = false;
	}

	if (!(Get_DIKeyState(DIK_W) ||
		Get_DIKeyState(DIK_A) ||
		Get_DIKeyState(DIK_S) ||
		Get_DIKeyState(DIK_D) ||
		Get_DIKeyState(DIK_SPACE) ||
		Get_DIKeyState(DIK_LSHIFT) ||
		Get_DIMouseState(DIM_RB)))
	{

		if (!(m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")) &&
			!(m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
		{
			if (m_fLeftArmAngle != 0.f)
			{
				if (m_fLeftArmAngle > 0.f)
				{
					m_fLeftArmAngle -= D3DXToRadian(10.f);
					if (m_fLeftArmAngle < 0.f)
						m_fLeftArmAngle = 0.f;
				}
				else if (m_fLeftArmAngle < 0.f)
				{
					m_fLeftArmAngle += D3DXToRadian(10.f);
					if (m_fLeftArmAngle > 0.f)
						m_fLeftArmAngle = 0.f;
				}
			}

			if (m_fRightArmAngle != 0.f)
			{
				if (m_fRightArmAngle > 0.f)
				{
					m_fRightArmAngle -= D3DXToRadian(10.f);
					if (m_fRightArmAngle < 0.f)
						m_fRightArmAngle = 0.f;
				}
				else if (m_fRightArmAngle < 0.f)
				{
					m_fRightArmAngle += D3DXToRadian(10.f);
					if (m_fRightArmAngle > 0.f)
						m_fRightArmAngle = 0.f;
				}
			}

			if (m_fHandAngle != 0.f)
			{
				if (m_fHandAngle > 0.f)
				{
					m_fHandAngle -= D3DXToRadian(10.f);
					if (m_fHandAngle < 0.f)
						m_fHandAngle = 0.f;
				}
				else if (m_fHandAngle < 0.f)
				{
					m_fHandAngle += D3DXToRadian(10.f);
					if (m_fHandAngle > 0.f)
						m_fHandAngle = 0.f;
				}
			}
		}

		if (m_fLeftLegAngle != 0.f)
		{
			if (m_fLeftLegAngle > 0.f)
			{
				m_fLeftLegAngle -= D3DXToRadian(10.f);
				if (m_fLeftLegAngle < 0.f)
					m_fLeftLegAngle = 0.f;
			}
			else if (m_fLeftLegAngle < 0.f)
			{
				m_fLeftLegAngle += D3DXToRadian(10.f);
				if (m_fLeftLegAngle > 0.f)
					m_fLeftLegAngle = 0.f;
			}
		}

		if (m_fRightLegAngle != 0.f)
		{
			if (m_fRightLegAngle > 0.f)
			{
				m_fRightLegAngle -= D3DXToRadian(10.f);
				if (m_fRightLegAngle < 0.f)
					m_fRightLegAngle = 0.f;
			}
			else if (m_fRightLegAngle < 0.f)
			{
				m_fRightLegAngle += D3DXToRadian(10.f);
				if (m_fRightLegAngle > 0.f)
					m_fRightLegAngle = 0.f;
			}
		}
	}
}

void CCubePlayer::Move()
{

	_float fSound = .5f;
	if (Get_DIKeyState(DIK_W))
		PlaySoundW(L"Walk.mp3", SOUND_REFLECT, fSound);
	_vec3 vPos;
	m_pBodyWorld->Get_Info(INFO_POS, &vPos);

	_vec3	vDir(0, 0, 0);
	_vec3	vNormal(0, 0, 0);

	if (Get_DIKeyState(DIK_W) && Get_DIKeyState(DIK_A))
	{
		_vec3 vLook, vRight;
		m_pBodyWorld->Get_Info(INFO_LOOK, &vLook);
		m_pBodyWorld->Get_Info(INFO_RIGHT, &vRight);
		vRight *= -1.f;

		vDir = vLook + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_W) && Get_DIKeyState(DIK_D))
	{
		_vec3 vLook, vRight;
		m_pBodyWorld->Get_Info(INFO_LOOK, &vLook);
		m_pBodyWorld->Get_Info(INFO_RIGHT, &vRight);

		vDir = vLook + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_S) && Get_DIKeyState(DIK_D))
	{
		_vec3 vLook, vRight;
		m_pBodyWorld->Get_Info(INFO_LOOK, &vLook);
		m_pBodyWorld->Get_Info(INFO_RIGHT, &vRight);
		vLook *= -1.f;

		vDir = vLook + vRight;

D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_S) && Get_DIKeyState(DIK_A))
	{
		_vec3 vLook, vRight;
		m_pBodyWorld->Get_Info(INFO_LOOK, &vLook);
		m_pBodyWorld->Get_Info(INFO_RIGHT, &vRight);
		vLook *= -1.f;
		vRight *= -1.f;

		vDir = vLook + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_W))
	{
		m_pBodyWorld->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		
	}
	else if (Get_DIKeyState(DIK_S))
	{
		m_pBodyWorld->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		vDir = -vDir;
	}
	else if (Get_DIKeyState(DIK_A))
	{
		m_pBodyWorld->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		vDir = -vDir;
	}
	else if (Get_DIKeyState(DIK_D))
	{
		m_pBodyWorld->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
	}

	if (m_bCanStaticField)
	{
		if (Key_Down(DIK_G))
		{
			_float fGunSound = .5f;
			Engine::PlaySoundGun(L"Grenade.wav", SOUND_EFFECT, fGunSound);
			m_pProjectileParicle->addParticle();
		}
	}
	if (m_bCanShield)
	{
		if (Key_Down(DIK_H))								// 총알 멈추는거
		{
			if (m_pShield != nullptr)
			{
				m_pShield->Kill_Obj();
			}
			_vec3 vPos;
			_vec3 vDir;
			m_pTransform->Get_Info(INFO_POS, &vPos);
			m_pBodyWorld->Get_Info(INFO_LOOK, &vDir);
			CLayer* pLayer = Get_Layer(STAGE_SKILL);
			m_pShield = CShield::Create(m_pGraphicDev, vPos, vDir);
			NULL_CHECK_RETURN(m_pShield, );
			FAILED_CHECK_RETURN(pLayer->Add_GameList(m_pShield), );
		}
		if (Key_Pressing(DIK_H))
		{
			if (m_pShield != nullptr)
				m_pShield->On_Move();

		}
		if (!(Get_DIKeyState(DIK_H)))
		{
			if (m_pShield != nullptr)

				m_pShield->Off_Move();
		}
	}
	if (Key_Pressing(DIK_C))
	{
		
		/*if (!m_pKrakenSmoke)
			m_pKrakenSmoke = dynamic_cast<CMFieldEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"KrakenParticle"));
		if (m_pKrakenSmoke != nullptr)
		{
			m_pKrakenSmoke->Set_PclePos(vPos);
			for (int i = 0; i < 150; ++i)
			{
				m_pKrakenSmoke->addParticle();
			}

		}*/


		/*if (!m_pKrakenHit)
			m_pKrakenHit = dynamic_cast<CKrakenHit*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"KrakenHit"));
		if (m_pKrakenHit != nullptr)
		{
			m_pKrakenHit->Set_PclePos(vPos);
			for (int i = 0; i < 150; ++i)
			{
				m_pKrakenHit->addParticle();
			}
		}*/

		


		//_vec3 vPos;														//대쉬이펙트하려던것
		//	_vec3 vDir;
		//	m_pTransform->Get_Info(INFO_POS, &vPos);
		//	_vec3 min = { -1.0f ,-1.0f ,-1.0f };
		//	m_pTransform->Get_Info(INFO_POS, &vPos);
		//	vPos.x -= 5.f;
		//	vPos.y += 5.f;
		//	vPos.z -= 5.f;
		//	for (_int i = -5; i < 5; i++)
		//	{
		//		for (_int j = -5; j < 5; j++)
		//		{
		//			for (_int k = -5; k < 5; k++)
		//			{
		//				D3DXVec3Normalize(&min, &_vec3(i, j, k));						

		//				dynamic_cast<CRoundEffect*>(m_pRoundEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.1);

		//				dynamic_cast<CRoundEffect*>(m_pRoundEffect)->Set_PcleDir(-min);

		//				m_pRoundEffect->addParticle();
		//			}
		//		}
		//	}
	/*		dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"));
		CTransform* pTransform = nullptr;
		pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_2", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform, );
		pTransform->Get_Info(INFO_POS, &vPos);
		_vec3 vPos1;
		_vec3 vDir1;
		m_pBodyWorld->Get_Info(INFO_POS, &vPos);
		m_pHeadWorld->Get_Info(INFO_LOOK, &vDir);
		dynamic_cast<CTraceEffect*>(m_pTraceEffect)->Set_PclePos(vPos1);
		dynamic_cast<CTraceEffect*>(m_pTraceEffect)->Set_PcleDir(vDir1);
		for (_int i = 0; i < 150; ++i)
		{
			m_pTraceEffect->addParticle();
		}*/
	}


	if (Key_Down(DIK_T))
	{
		CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
		NULL_CHECK_RETURN(pTerrainBufferCom, );

		CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pTerrainTransformCom, );

		_vec3 vPos;
		vPos = m_pCalculatorCom->Peek_Target_Vector(g_hWnd, &_vec3(0.f, 0.f, 0.f), pTerrainBufferCom, pTerrainTransformCom);
		_vec3 vSetPos = _vec3(vPos.x, vPos.y + 0.5f, vPos.z);

		CLayer* pLayer = Get_Layer(STAGE_SKILL);
		CGameObject* pGameObject = CPing::Create(m_pGraphicDev, vSetPos);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		for (auto& iter : Get_Layer(STAGE_SUPPORTER)->Get_GameObjectMap())
		{
			dynamic_cast<CSupporter*>(iter.second)->SetOrdered(true);
		}
	}

	if (Get_DIKeyState(DIK_SPACE))
	{
		m_bJump = true;
	}

	if (Get_DIKeyState(DIK_LSHIFT))
	{
		m_bJump = false;
	}
	m_pCollision->Wall_Collision_Check(this->m_pTransform, this->m_pHitBox, &vDir);

	m_pBodyWorld->Move_Pos(&(vDir * m_fSpeed * m_fTimeDelta));

	//_int iCollision = m_pCollision->Wall_Collision(&vNormal);

	//if (-1 != iCollision)
	//{
	//	float fDot = D3DXVec3Dot(&vNormal, &vDir);
	//	float fDiagonal = acosf(fDot);

	//	if (iCollision == WALL_RIGHT || iCollision == WALL_LEFT || iCollision == WALL_BACK)
	//	{
	//		if (D3DXToDegree(fDiagonal) > 90.f)
	//		{
	//			_vec3 vSliding = vDir;
	//			m_pCollision->Wall_Collision_By_DotSliding(&vSliding);

	//			m_pBodyWorld->Move_Pos(&(vSliding * m_fSpeed * m_fTimeDelta));
	//		}
	//		else
	//		{
	//			m_pBodyWorld->Move_Pos(&(vDir * m_fSpeed * m_fTimeDelta));
	//		}
	//	}
	//	if (iCollision == WALL_FRONT)
	//	{
	//		if (D3DXToDegree(fDiagonal) < 90.f)
	//		{
	//			_vec3 vSliding = vDir;
	//			m_pCollision->Wall_Collision_By_DotSliding(&vSliding);

	//			m_pBodyWorld->Move_Pos(&(vSliding * m_fSpeed * m_fTimeDelta));
	//		}
	//		else
	//		{
	//			m_pBodyWorld->Move_Pos(&(vDir * m_fSpeed * m_fTimeDelta));
	//		}
	//	}
	//}
	//else	//	충돌하지 않았으며 충돌한 방향과 반대 방향으로 진행하는 이동 처리
	//{
	//	m_pBodyWorld->Move_Pos(&(vDir * m_fSpeed * m_fTimeDelta));
	//}

	m_vDirection = vDir;
}

void CCubePlayer::TransAxis(void)
{
	_vec3	vBefore, vAfter;

	FAILED_CHECK_RETURN(Get_BodyTransform(), );

	m_pBodyWorld->Get_Info(INFO_POS, &vBefore);
	m_pBodyWorld->Get_BeforeInfo(INFO_POS, &vAfter);

	m_pLeftArmWorld->Rotation_Axis_Animation(-0.1f, -0.15f, m_fLeftArmAngle, -m_fLookAngle);
	m_pRightArmWorld->Rotation_Axis_Animation(-0.1f, 0.15f, m_fRightArmAngle, -m_fLookAngle);

	if ((nullptr != m_Weapon) && Get_DIMouseState(DIM_RB) || ((m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")) ||
		(m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))))
	{
		m_pLeftHandWorld->Rotation_Axis_Special(-0.3f, -0.15f, m_fLeftArmAngle, -m_fLookAngle, -0.1f, -m_fHandAngle);
	}
	else
	{
		m_pLeftHandWorld->Rotation_Axis_Animation(-0.3f, -0.15f, m_fLeftArmAngle, -m_fLookAngle, -0.1f, m_fHandAngle);
	}

	m_pRightHandWorld->Rotation_Axis_Animation(-0.3f, 0.15f, m_fRightArmAngle, -m_fLookAngle, -0.1f, m_fHandAngle);

	m_pLeftLegWorld->Rotation_Axis_Animation(-0.1f, -0.05f, m_fLeftLegAngle, -m_fLookAngle);
	m_pRightLegWorld->Rotation_Axis_Animation(-0.1f, 0.05f, m_fRightLegAngle, -m_fLookAngle);
	m_pLeftFootWorld->Rotation_Axis_Animation(-0.3f, -0.05f, m_fLeftLegAngle, -m_fLookAngle, -0.1f, fabs(m_fLeftLegAngle));
	m_pRightFootWorld->Rotation_Axis_Animation(-0.3f, 0.05f, m_fRightLegAngle, -m_fLookAngle, -0.1f, fabs(m_fRightLegAngle));
}

void CCubePlayer::Look_Direction(void)
{
	FAILED_CHECK_RETURN(Get_BodyTransform(), );

	_vec3	vHead, vBody;
	m_pHeadWorld->Get_BeforeInfo(INFO_LOOK, &vHead);
	m_pBodyWorld->Get_BeforeInfo(INFO_LOOK, &vBody);

	_long MoveX = Get_DIMouseMove(DIMS_X);
	_long MoveY = Get_DIMouseMove(DIMS_Y);
	_long MoveZ = Get_DIMouseMove(DIMS_Z);

	m_pBodyWorld->Rotation(ROT_Y, D3DXToRadian(MoveX / 10.f));
	m_fLookAngle -= D3DXToRadian(MoveX / 10.f);
	m_fDownAngle += D3DXToRadian(MoveY / 10.f);
}

void CCubePlayer::Fire_Bullet(void)
{

	if (Get_DIMouseState(DIM_RB))
	{
		if (Get_DIMouseState(DIM_LB))
		{
			if (m_Weapon->Get_Ability()->fRemainBulletCnt > 0)
			{
				m_pBulletParicle->addParticle();
				//m_pShotParicle->addParticle();

				m_Weapon->Set_MinusBullet();
				m_Weapon->Set_Shoot(true);
				m_fBulletTime = 0.f;
				_vec3 vBPos;									  //불꽃이팩트
				_vec3 vRHPos;

				_vec3 vBDir;

				m_pTransform->Get_Info(INFO_POS, &vBPos);
				m_pRightHandWorld->Get_Info(INFO_POS, &vRHPos);
				m_pBodyWorld->Get_Info(INFO_LOOK, &vBDir);
				vRHPos += vBDir*10.f;
				vRHPos.y += 0.1f;
				dynamic_cast<CTriggerParticle*>(m_pTriggerParticle)->Set_PclePos(vRHPos);
				dynamic_cast<CTriggerParticle*>(m_pTriggerParticle)->Set_PcleDir(vBDir);
				//불꽃이팩트

				//dynamic_cast<CTriggerFront*>(m_pTriggerFront)->Set_PclePos(vRHPos);


				_vec3 vLHPos;									//탄피이팩트
				_vec3 vLDir;

				m_pLeftHandWorld->Get_Info(INFO_POS, &vLHPos);
				vLDir = vLHPos - vBPos;
				vLDir.y *= -1.f;
				dynamic_cast<CCartridgeParticle*>(m_pCartridgeParticle)->Set_PcleDir(vLDir);


				if (m_iWeaponState == 2)
				{
					_float fGunSound = .3f;
					Engine::PlaySoundGun(L"RifleShot.wav", SOUND_EFFECT, fGunSound);
					for (_int i = 0; i < 2; ++i)
					{
						m_pCartridgeParticle->addParticle();
					}
					for (_int i = 0; i < 3; ++i)
					{
						m_pTriggerParticle->addParticle();
					}
					dynamic_cast<CTriggerParticle*>(m_pTriggerParticle)->Set_PclePos(vLHPos + vBDir*10.f);
					for (_int i = 0; i < 3; ++i)
					{
						m_pTriggerParticle->addParticle();
					}
					for (_int i = 0; i < 25; ++i)
					{
						m_pTriggerFront->addParticle();
					}

				}
				if (m_iWeaponState == 3)
				{
					_float fGunSound = .3f;
					Engine::PlaySoundGun(L"ShotgunSound.wav", SOUND_EFFECT, fGunSound);
					for (_int i = 0; i < 64; ++i)
					{
						m_pCartridgeParticle->addParticle();
					}
					for (_int i = 0; i < 25; ++i)
					{
						m_pTriggerParticle->addParticle();
					}
					for (_int i = 0; i < 100; ++i)
					{
						m_pTriggerFront->addParticle();
					}

				}
				if (m_iWeaponState == 4)
				{
					_float fGunSound = 1.f;
					Engine::PlaySoundGun(L"SniperSound.wav", SOUND_EFFECT, fGunSound);
					for (_int i = 0; i < 1; ++i)
					{
						m_pCartridgeParticle->addParticle();
					}
					for (_int i = 0; i < 25; ++i)
					{
						m_pTriggerParticle->addParticle();
					}
					for (_int i = 0; i < 100; ++i)
					{
						m_pTriggerFront->addParticle();
					}
					m_bSniperEffect = true;
				}
			}
		}
	}

	if (m_bSniperEffect)
	{
		m_bSniperEffect = false;
		_vec3 vPos;														//대쉬이펙트하려던것
		_vec3 vDir;
		dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GUN, L"Sniper_Part_2", TRANSFORM_COMP, ID_DYNAMIC))->Get_Info(INFO_POS, &vPos);
		m_pHeadWorld->Get_Info(INFO_LOOK, &vDir);
		dynamic_cast<CLaserPoint*>(m_pLaserPoint)->Set_PclePos(vPos);
		dynamic_cast<CLaserPoint*>(m_pLaserPoint)->Set_PcleDir(vDir);

		m_fLaserTimer = 0.f;

		for (_int i = 0; i < 150; ++i)
		{
			m_pLaserPoint->addParticle();
		}
		if (m_fLaserTimer <= 10.f)
		{
			for (_int i = 0; i < 150; ++i)
			{
				m_pLaserPoint->addParticle();
			}
		}
	}
}

void CCubePlayer::Gun_Check(void)
{
	// 만약 땅에 떨어진 UZI랑 충돌을 했다면 m_vecWeapon에 pushback으로 우지가 들어간다.
	// ex코드
	if (m_bUzi == true)
	{
		dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Off_Sniper();
		dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Off_ShotGun();
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Set_Uzi();
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Set_Uzi();

		m_Weapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"));
		m_vecWeapon.push_back(m_Weapon);
		m_tAbility->iGunTexture = 0;
		m_bUzi = false;
	}
	if (m_bShotgun == true)
	{
		dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Off_Sniper();
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Off_Uzi();
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Off_Uzi();
		dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Set_ShotGun();

		m_Weapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"));
		m_vecWeapon.push_back(m_Weapon);
		m_tAbility->iGunTexture = 2;
		m_bShotgun = false;
	}
	if (m_bSniper == true)
	{
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Off_Uzi();
		dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Off_Uzi();
		dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Off_ShotGun();
		dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Set_Sniper();

		m_Weapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"));
		m_vecWeapon.push_back(m_Weapon);
		m_tAbility->iGunTexture = 4;
		m_bSniper = false;
	}

	if (Get_DIKeyState(DIK_1) & 0x80 || m_iSetWeaponState == 2)
	{
		if (!m_vecWeapon.empty())
		{
			if (m_vecWeapon[0] != nullptr)
			{
				m_Weapon = m_vecWeapon[0];
				dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Off_Sniper();
				dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Off_ShotGun();
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Set_Uzi();
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Set_Uzi();
				m_tAbility->iGunTexture = 0; // 혹여나 총 업그레이드해서 다른 총으로 보이게 된다면 이 숫자와 UI/Gun 에 들어있는 숫자와 비교해서 넣으면됨.
				m_iSetWeaponState = 0;
				//m_iWeaponState = 2;d
			}
		}
	}
	if (Get_DIKeyState(DIK_2) & 0x80 || m_iSetWeaponState == 3)
	{
		if (m_vecWeapon.size() >= 2)
		{
			if (m_vecWeapon[1] != nullptr)
			{
				m_Weapon = m_vecWeapon[1];
				dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Off_Sniper();
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Off_Uzi();
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Off_Uzi();
				dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Set_ShotGun();
				m_tAbility->iGunTexture = 2;

				m_iSetWeaponState = 0;
			}
		}
	}
	if (Get_DIKeyState(DIK_3) & 0x80 || m_iSetWeaponState == 4)
	{
		if (m_vecWeapon.size() >= 3)
		{
			if (m_vecWeapon[2] != nullptr)
			{
				m_Weapon = m_vecWeapon[2];
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1"))->Off_Uzi();
				dynamic_cast<CUzi*>(Engine::Get_GameObject(STAGE_GUN, L"UZI2"))->Off_Uzi();
				dynamic_cast<CShotgun*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))->Off_ShotGun();
				dynamic_cast<CSniper*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))->Set_Sniper();
				m_tAbility->iGunTexture = 4;

				m_iSetWeaponState = 0;
			}
		}
	}
	if (Get_DIKeyState(DIK_4) & 0x80)
	{
		if (m_vecWeapon.size() >= 4)
		{
			if (m_vecWeapon[3] != nullptr)
			{
				m_Weapon = m_vecWeapon[3];
				m_tAbility->iGunTexture = 3;
			}
		}
	}
	if (Get_DIKeyState(DIK_5) & 0x80)
	{
		if (m_vecWeapon.size() >= 5)
		{
			if (m_vecWeapon[4] != nullptr)
			{
				m_Weapon = m_vecWeapon[4];
				m_tAbility->iGunTexture = 4;
			}
		}
	}
}

void CCubePlayer::Inventory_Check(void)
{
	if (Engine::Get_Scene()->Get_SceneId() == STAGE_SCENE)
	{
		m_iDmgItem = dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_WeaponDmg();
		m_iSpeedItem = dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_WeaponSpeed();
		m_iSkillEnforce = dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_EnforceCheck();
	}
	else if (Engine::Get_Scene()->Get_SceneId() == FINAL_SCENE)
	{
		m_iDmgItem = 1;
		m_iSpeedItem = 1;

		On_StaticField();
		On_Shield();

		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"UZI1"))
		{
			m_iSkillEnforce = 1;
		}
		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))
		{
			m_iSkillEnforce = 3;
		}
		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))
		{
			m_iSkillEnforce = 5;
		}
	}

	switch (m_iSkillEnforce)
	{
	case 0:
		//cout << 0 << endl;
		break;
	case 1://우지1강화
		   //cout << 1 << endl;
		break;
	case 2://우지2강화
		   //cout << 2 << endl;
		break;
	case 3://샷건1강화
		   //cout << 3 << endl;
		break;
	case 4://샷건2강화
		   //cout << 4 << endl;
		break;
	case 5://스나1강화
		   //cout << 5 << endl;
		break;
	case 6://스나2강화
		   //cout << 6 << endl;
		break;
	default:
		break;
	}


	{
		if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"UZI1"))
		{
			m_iWeaponState = 2;
		}
		else if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN"))
		{
			m_iWeaponState = 3;
		}
		else if (m_Weapon == Engine::Get_GameObject(STAGE_GUN, L"SNIPER"))
		{
			m_iWeaponState = 4;
		}
		else
			m_iWeaponState = 0;
	}
}

void CCubePlayer::Jump(void)
{
}

HRESULT CCubePlayer::Get_BodyTransform(void)
{
	m_pHeadWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"HEAD", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pHeadWorld, E_FAIL);
	m_pBodyWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pLeftArmWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"L_ARM", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pRightArmWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"R_ARM", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pLeftLegWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"L_LEG", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pRightLegWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"R_LEG", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pLeftHandWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"L_HAND", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pRightHandWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"R_HAND", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pLeftFootWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"L_FOOT", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	m_pRightFootWorld = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"R_FOOT", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pBodyWorld, E_FAIL);
	return S_OK;
}

HRESULT CCubePlayer::Player_Mapping(void)
{
	if (!m_MappingInit)
	{
		CGameObject*	m_pMapMonster = CPlayerMapping::Create(m_pGraphicDev);
		TCHAR* szCntName = new TCHAR[64];
		wsprintf(szCntName, L"Map");
		Engine::Add_GameObject(STAGE_MAPPING, m_pMapMonster, szCntName);
		m_listMonsterCnt.push_back(szCntName);

		m_pBaseMapping = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_MAPPING, szCntName, TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pBaseMapping, E_FAIL);
		m_MappingInit = true;
	}

	return S_OK;
}

void CCubePlayer::Key_Input(const _float & fTimeDelta)
{
}

HRESULT CCubePlayer::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });

	pInstance = m_pHitBox = dynamic_cast<CHitBox*>(Engine::Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pInstance });

	pInstance = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ COLLISION_COMP, pInstance });

	pInstance = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pInstance });

	// For Sphere
	pInstance = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Engine::Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pInstance });

	pInstance = m_pSphereTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Sphere_TransCom", pInstance });

	return S_OK;
}

HRESULT CCubePlayer::Lighting()
{
	D3DLIGHT9 d3dLight;
	// Initialize the structure.
	ZeroMemory(&d3dLight, sizeof(d3dLight));

	// Set up a white point light.
	d3dLight.Type = D3DLIGHT_POINT;

	d3dLight.Diffuse.r = m_fRed;
	d3dLight.Diffuse.g = m_fGreen;
	d3dLight.Diffuse.b = m_fBlue;
	d3dLight.Ambient.r = m_fRed;
	d3dLight.Ambient.g = m_fGreen;
	d3dLight.Ambient.b = m_fBlue;
	d3dLight.Specular.r = 1.0f;
	d3dLight.Specular.g = 1.0f;
	d3dLight.Specular.b = 1.0f;

	// Position it high in the scene and behind the user.
	// Remember, these coordinates are in world space, so
	// the user could be anywhere in world space, too. 
	// For the purposes of this example, assume the user
	// is at the origin of world space.

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	d3dLight.Position.x = vPos.x;
	d3dLight.Position.y = vPos.y + 3.f;
	d3dLight.Position.z = vPos.z;

	// Don't attenuate.
	d3dLight.Attenuation0 = 0.5f;
	d3dLight.Range = m_fRange;

	// Set the property information for the first light.
	FAILED_CHECK_RETURN(m_pGraphicDev->SetLight(1, &d3dLight), E_FAIL);

	return S_OK;
}

CCubePlayer * CCubePlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubePlayer* pInstance = new CCubePlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CCubePlayer::Free(void)
{
	CGameObject::Free();
	for (auto& iter : m_listMonsterCnt)
	{
		if (iter != nullptr)
			delete iter;
	}

	Safe_Delete<ABILITY*>(m_tAbility);

	m_listMonsterCnt.clear();
}