#include "stdafx.h"
#include "..\Header\MiddleBoss.h"
#include "PoolMgr.h"
#include "ObtainBullet.h"
#include "ObtainDefense.h"
#include "HealthPotion.h"
#include "MonsterUI.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "ComboUI.h"
#include "MonsterMapping.h"
#include "TransAxisBox.h"
#include "FlightBomb.h"
#include "Explosion.h"
#include "Veneer.h"

#include "Flight.h"

#include "LaserSpot.h"
#include "Zombie.h"
#include "Alien.h"
#include "TestCube.h"

#include "MBLaser.h"
#include "TriggerParticle.h"

#include "MFieldEffect.h"
#include "MHitEffect.h"
#include "MBLaserReady.h"
#include "MBBomb.h"

static _int m_iCnt = 0;

CMiddleBoss::CMiddleBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CMiddleBoss::~CMiddleBoss()
{
}

HRESULT CMiddleBoss::Ready_Object(const _vec3 & vPos, _tchar * Name)
{
	m_tAbility = new MIDDLEBOSSABILITY;
	m_tAbility->fMaxHp = 5000.f;
	m_tAbility->fCurrentHp = m_tAbility->fMaxHp;
	m_tAbility->fDamage = 5.f;
	m_tAbility->strObjTag = L"MiddleBoss";
	m_fDetectRange = 30.f;

	m_fBeforeHp = m_tAbility->fMaxHp;

	m_MonsterName = Name;

	m_STATE = MIDDLEBOSS_IDLE;
	m_WALK = MIDDLEBOSSWALK_1;
	m_IDLE = MIDDLEBOSSIDLE_1;
	m_NORMALATTACK = MIDDLEBOSS_NORMALATTACK_1;
	m_BOMBING = MIDDLEBOSS_BOMBING_1;
	m_CRASH = MIDDLEBOSS_CRASH_1;
	m_PATTERN = MIDDLEBOSS_SKILL_END;
	m_DEAD = MIDDLEBOSS_DEAD_1;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_pTransUICom->Set_Scale(3.f, 1.f, 1.5f);
	
	_vec3 vAnimationPos;
	m_pTransCom->Get_Info(INFO_POS, &vAnimationPos);

	m_pHitBoxTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pHitBoxTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pHitBoxTransCom->Static_Update();

	m_pSearchRange_TransCom->Set_Scale(&_vec3(100.f, 0.f, 100.f));
	m_pSearchRange_TransCom->Set_Pos(vAnimationPos.x, 0.f, vAnimationPos.z);
	m_pSearchRange_TransCom->Static_Update();

	m_pAttackRange_TransCom->Set_Scale(&_vec3(10.f, 10.f, 10.f));
	m_pAttackRange_TransCom->Set_Pos(vAnimationPos.x, 0.f, vAnimationPos.z);
	m_pAttackRange_TransCom->Static_Update();

	return S_OK;
}

_int CMiddleBoss::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		m_pComboUI->KillCntPlus();
		_float fMiddle_death = 1.5f;
		PlaySoundGun(L"Middle_Death.wav", SOUND_EFFECT, fMiddle_death);

		return -1;
	}

	m_fTimeDelta = fTimeDelta;

	if (m_bFirst)
	{
		m_bFirst = false;

		m_vPattern.push_back(MIDDLEBOSS_SKILL_NORMALATTACK);
		m_vPattern.push_back(MIDDLEBOSS_SKILL_BOMBING);
		m_vPattern.push_back(MIDDLEBOSS_SKILL_LASER);

		Engine::Get_Scene()->New_Layer(m_MonsterName);
		pMyLayer = Engine::Get_Layer(m_MonsterName);
		FAILED_CHECK_RETURN(Build(), -1);

		Load_Animation(L"../../Data/Thor/THOR_IDLE_1.dat", 0);
		Load_Animation(L"../../Data/Thor/THOR_IDLE_2.dat", 1);
		Load_Animation(L"../../Data/Thor/THOR_IDLE_3.dat", 2);
		Load_Animation(L"../../Data/Thor/THOR_IDLE_4.dat", 3);

		Load_Animation(L"../../Data/Thor/THOR_NORMALATTACK_1.dat", 4);
		Load_Animation(L"../../Data/Thor/THOR_NORMALATTACK_2.dat", 5);
		Load_Animation(L"../../Data/Thor/THOR_NORMALATTACK_3.dat", 6);
		Load_Animation(L"../../Data/Thor/THOR_NORMALATTACK_4.dat", 7);

		Load_Animation(L"../../Data/Thor/THOR_BOMBING_1.dat", 11);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_2.dat", 12);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_3.dat", 13);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_4.dat", 14);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_5.dat", 15);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_6.dat", 16);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_7.dat", 17);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_8.dat", 18);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_9.dat", 19);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_10.dat", 20);
		Load_Animation(L"../../Data/Thor/THOR_BOMBING_11.dat", 21);

		Load_Animation(L"../../Data/Thor/THOR_DEAD_1.dat", 22);
		Load_Animation(L"../../Data/Thor/THOR_DEAD_2.dat", 23);
		Load_Animation(L"../../Data/Thor/THOR_DEAD_3.dat", 24);

		m_vMonterPos.push_back(_vec3(118.f, 0.6f, 64.f));
		m_vMonterPos.push_back(_vec3(97.f, 0.6f, 64.f));
		m_vMonterPos.push_back(_vec3(118.f, 0.6f, 40.f));
		m_vMonterPos.push_back(_vec3(97.f, 0.6f, 40.f));
		m_vMonterPos.push_back(_vec3(118.f, 0.6f, 18.f));
		m_vMonterPos.push_back(_vec3(97.f, 0.6f, 18.f));
		m_vMonstertype.push_back(0);
		m_vMonstertype.push_back(1);
		m_pGraphicDev->LightEnable(2, false);
	}

	Update_Pattern(fTimeDelta);

	Look_Direction();

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	m_pTransUICom->Billboard_Transform(fTimeDelta);
	Hit_Check(fTimeDelta);

	_vec3 vMonsterPos;
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);

	for (auto& iter : pMyLayer->Get_GamePair())
	{
		if (0 == _tcscmp(iter.first, L"CORE"))
		{
			CTransAxisBox* pBox = dynamic_cast<CTransAxisBox*>(iter.second);
			_matrix matFinal;
			pBox->Get_Final(&matFinal);

			m_pHitBoxTransCom->Set_Pos(matFinal.m[3][0], matFinal.m[3][1], matFinal.m[3][2]);
		}
	}

	m_pSearchRange_TransCom->Set_Pos(vMonsterPos.x, 0.f, vMonsterPos.z);
	m_pAttackRange_TransCom->Set_Pos(vMonsterPos.x, 0.f, vMonsterPos.z);

	m_pTransCom->Get_Info(INFO_POS, &vUIPos);
	m_pTransUICom->Set_Pos(vUIPos.x, vUIPos.y + 8.f, vUIPos.z);

	if (m_bPatternStart)
	{
		m_fCreateMonFrame += fTimeDelta;

		if (m_fCreateMonFrame >= 5.f)
		{
			m_pGraphicDev->LightEnable(2, true);
			Create_Monster();
			m_fCreateMonFrame = 0.f;
		}
	}

	return 0;
}
void CMiddleBoss::LateUpdate_Object(void)
{
	Monster_Mapping();

	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_STATE = MIDDLEBOSS_END;

		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		m_pTransCom->Set_Pos(vPos.x, vPos.y - 0.1f, vPos.z);
		m_pTransCom->Static_Update();

		CGameObject* pGameObject = Get_GameObject(STAGE_GAMELOGIC, L"LaserSpot");
		dynamic_cast<CLaserSpot*>(pGameObject)->Attack_Permit(false);
		Dead_Effect();
		m_pMonsterUI->Off_Switch();
		dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"))->CameraShaking();
		if(vPos.y <= -20.f)
			this->Kill_Obj();
	}
	else
	{
		Set_OnTerrain();
		m_pTransCom->Static_Update();
	}

	if (!m_bFirst)
	{
		if (m_STATE == MIDDLEBOSS_IDLE)
		{
			Idle_Animation_Run();
			Run_Animation(100.f);
		}
		else if (m_STATE == MIDDLEBOSS_ATTACK)
		{
			if (m_PATTERN == MIDDLEBOSS_SKILL_NORMALATTACK)
			{
				NormalAttack_Animation_Run();
				if (m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_1 || m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_3)
					Run_Animation(1.f);
				if (m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_2 || m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_4)
					Run_Animation(20.f);
			}
			else if (m_PATTERN == MIDDLEBOSS_SKILL_CRASH)
			{
				Bombing_Animation_Run();
				Run_Animation(10.f);
			}
			else if (m_PATTERN == MIDDLEBOSS_SKILL_BOMBING)
			{
				BombState_Effect();
				Bombing_Animation_Run();
				if (m_BOMBING >= MIDDLEBOSS_BOMBING_4 && m_BOMBING <= MIDDLEBOSS_BOMBING_9)
					Run_Animation(10.f);
				else
					Run_Animation(50.f);
			}
			else if (m_PATTERN == MIDDLEBOSS_SKILL_LASER)
			{
				m_fLaserTime += m_fTimeDelta;
				if (m_fLaserTime <= .35f)
				{
					_float fSound = 1.f;
					PlaySoundW(L"MBLaserReady.wav", SOUND_MB, fSound);
				}
				if (m_fLaserTime <= 1.5f)
					LaserReady_Effect();
				if (m_fLaserTime >= 1.f)
				{
					CGameObject* pGameObject = Get_GameObject(STAGE_GAMELOGIC, L"LaserSpot");					
						dynamic_cast<CLaserSpot*>(pGameObject)->Attack_Permit(true);				
				}
				_vec3 vSpotPos;
				CTransform* pSpotTrans = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_GAMELOGIC, L"LaserSpot", TRANSFORM_COMP, ID_DYNAMIC));
				if (pMyLayer != nullptr)
				{
					for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
					{
						if (0 == _tcscmp(iter.first, L"CORE"))//코어위치
						{
							CTransAxisBox* pCoreBox = dynamic_cast<CTransAxisBox*>(iter.second);
							_matrix matFinal;
							pCoreBox->Get_Final(&matFinal);
							m_vCorePos = { matFinal.m[3][0], matFinal.m[3][1], matFinal.m[3][2] };
						}
					}
					if (m_fLaserTime >= 3.f)
					{
						pSpotTrans->Get_Info(INFO_POS, &vSpotPos); //점위치
						_float fSound = .15f;
						PlaySoundGun(L"MBLaser4.wav", SOUND_MB, fSound);
					}
					CMBLaser* m_pMBLaser = nullptr;
					if (!m_pMBLaser)
						m_pMBLaser = dynamic_cast<CMBLaser*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"MBLaser"));
					dynamic_cast<CMBLaser*>(m_pMBLaser)->Set_PclePos(m_vCorePos);
					dynamic_cast<CMBLaser*>(m_pMBLaser)->Set_PcleDir(vSpotPos - m_vCorePos);

					for (_int i = 0; i < 50; ++i)
					{
						m_pMBLaser->addParticle();
					}
				}
				_vec3 vDir = { 0.f,1.0f,0.f };
				CTriggerParticle* pTriggerParticle = nullptr;
				if (!pTriggerParticle)
					pTriggerParticle = dynamic_cast<CTriggerParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TriggerParticle"));
				dynamic_cast<CTriggerParticle*>(pTriggerParticle)->Set_PclePos(vSpotPos);
				dynamic_cast<CTriggerParticle*>(pTriggerParticle)->Set_PcleDir(vDir);
				for (_int i = 0; i < 15; ++i)
				{
					pTriggerParticle->addParticle();
				}
				// 여기에 레이저 추가
				if (m_fLaserTime >= 10.f)
				{
					m_STATE = MIDDLEBOSS_IDLE;
					m_fLaserTime = 0.f;
					CGameObject* pGameObject = Get_GameObject(STAGE_GAMELOGIC, L"LaserSpot");
					dynamic_cast<CLaserSpot*>(pGameObject)->Attack_Permit(false);
				}
			}
		}
		else if (m_STATE == MIDDLEBOSS_END)
		{
			Dead_Animation_Run();
			Run_Animation(30.f);
		}
	}

	CGameObject::LateUpdate_Object();
}


void CMiddleBoss::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pHitBoxTransCom->Get_WorldMatrixPointer());
	m_pHitBox->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pAnimationBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransUICom->Get_WorldMatrixPointer());

	m_pTextureUICom->Set_Texture(0);
	m_pBufferUICom->Resize_Buffer(m_tAbility->fCurrentHp / m_tAbility->fMaxHp);
	m_pBufferUICom->Render_Buffer();
}

_int CMiddleBoss::Update_Pattern(_float fTimeDelta)
{
	//왼손 오른손잡는 방향 잡아보자 

	_vec3 vShotgunLeft;
	_vec3 vShotgunRight;
	_vec3 m_vDirectionLeft;
	_vec3 m_vDirectionRight;

	for (auto& iter : pMyLayer->Get_GamePair())
	{
		if (0 == _tcscmp(iter.first, L"LEFTCANNON2"))
		{
			_matrix matBoss;
			dynamic_cast<CTransAxisBox*>(iter.second)->Get_Final(&matBoss);

			vShotgunLeft = { matBoss.m[3][0], matBoss.m[3][1], matBoss.m[3][2] };

			_vec3 vLook = { matBoss.m[2][0], matBoss.m[2][1], matBoss.m[2][2] };
			vShotgunLeft += vLook;
			
		}
		if (0 == _tcscmp(iter.first, L"RIGHTCANNON2"))
		{
			_matrix matBoss;
			dynamic_cast<CTransAxisBox*>(iter.second)->Get_Final(&matBoss);
			vShotgunRight = { matBoss.m[3][0], matBoss.m[3][1], matBoss.m[3][2] };

			_vec3 vLook = { matBoss.m[2][0], matBoss.m[2][1], matBoss.m[2][2] };
			vShotgunRight += vLook;
		}
	}

	if (m_pPlayerTransCom == nullptr)
	{
		m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	}
	
	if (m_pMonsterUI == nullptr)
		m_pMonsterUI = dynamic_cast<CMonsterUI*>(Engine::Get_GameObject(STAGE_UI, L"MonsterUI"));
	
	if (m_pComboUI == nullptr)
		m_pComboUI = dynamic_cast<CComboUI*>(Engine::Get_GameObject(STAGE_UI, L"ComboUI"));

	_vec3 vPlayerPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	_vec3 vPlayerScale;
	m_pPlayerTransCom->Get_Scale(&vPlayerScale);
	_vec3 vScale;
	m_pSphereTransCom->Get_Scale(&vScale);
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	_vec3 m_vDir;
	m_vDir = vPlayerPos - vPos;
	_float m_fPlayerCurrent = D3DXVec3Length(&m_vDir);
	_vec3 vAnimationPos;
	_vec3 vSearchScale, vAttackScale;
	
	m_pSearchRange_TransCom->Get_Scale(&vSearchScale);
	m_pAttackRange_TransCom->Get_Scale(&vAttackScale);
	
	_float HP = m_tAbility->fCurrentHp / m_tAbility->fMaxHp;

	srand((unsigned int)time(NULL) + m_iRandom);
	m_iRandom = rand() % 1000;
	
	int m_iRand = rand() % 10;
	
	/*if (!m_pCollision->Sphere_Collision(this->m_pSearchRange_TransCom, m_pPlayerTransCom, vPlayerScale.x, vSearchScale.x)
		&& (m_STATE != MIDDLEBOSS_ATTACK))
	{
		m_STATE = MIDDLEBOSS_MOVE;
		m_pTransCom->Chase_Target(&vPlayerPos, 1.f, fTimeDelta);
		m_ReloadTimer += fTimeDelta;
	}*/
	if (m_pCollision->Sphere_Collision(this->m_pSearchRange_TransCom, m_pPlayerTransCom, vPlayerScale.x, vSearchScale.x))
	{
		if (m_STATE == MIDDLEBOSS_MOVE)
			m_STATE = MIDDLEBOSS_IDLE;

		if (m_STATE == MIDDLEBOSS_IDLE)
		{
			//m_pTransCom->Chase_Target(&vPlayerPos, 0.f, fTimeDelta);

			// 문이 열리고 나면 조건으로 바꾸기

			m_ReloadTimer += fTimeDelta;

			if ((m_bPatternStart == false) && (m_ReloadTimer >= 3.f))
			{
				CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);

				for (auto& iter : *(pLayer->Get_GameObjectMapPtr()))
				{
					if (0 == _tcscmp(iter.first, L"CubeShop") || 0 == _tcscmp(iter.first, L"CubeShop2") || 0 == _tcscmp(iter.first, L"CubeShop3"))
					{
					}
					else
					{
						if (dynamic_cast<CTestCube*>(iter.second)->Get_Index() == 99)
						{
							if (dynamic_cast<CTestCube*>(iter.second)->Boss_Start())
							{
								m_bPatternStart = true;
								_float fSound = 1.f;
								PlaySoundW(L"MBPower.wav", SOUND_MB, fSound);
							}
						}
					}
				}
			}

			if ((m_ReloadTimer >= 3.f) && (m_bPatternStart == true))
			{
				m_STATE = MIDDLEBOSS_ATTACK;

				if (m_pCollision->Sphere_Collision(this->m_pAttackRange_TransCom, m_pPlayerTransCom, vPlayerScale.x, vAttackScale.x))
				{
					m_PATTERN = MIDDLEBOSS_SKILL_CRASH;
				}
				else
				{
					random_shuffle(m_vPattern.begin(), m_vPattern.end());
					m_PATTERN = m_vPattern.front();
				}

				{
					CGameObject* pGameObject = Get_GameObject(STAGE_GAMELOGIC, L"LaserSpot");
					CTransform* pSpotTrans = dynamic_cast<CTransform*>(pGameObject->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));

					_vec3 vPlayerPos;
					m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

					pSpotTrans->Set_Pos(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);
				}

				m_ReloadTimer = 0.f;
			}
		}
		else if (m_STATE == MIDDLEBOSS_ATTACK)
		{
			//m_pTransCom->Chase_Target(&vPlayerPos, 0.f, fTimeDelta);

			if (m_PATTERN == MIDDLEBOSS_SKILL_NORMALATTACK)
			{
				_vec3 m_vDirectionLeft = vPlayerPos - vShotgunLeft;
				_vec3 m_vDirectionRight = vPlayerPos - vShotgunRight;
				D3DXVec3Normalize(&m_vDirectionLeft, &m_vDirectionLeft);
				D3DXVec3Normalize(&m_vDirectionRight, &m_vDirectionRight);

				if ((m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_1) && m_AnimationTime >= 1.f)
				{
					_tchar* szName = new _tchar[256]{};
					wstring wName = L"Explosion_%d";

					wsprintfW(szName, wName.c_str(), g_iExplosion);
					m_TcharList.push_back(szName);

					g_iExplosion++;

					// 위치 바꿔줘야함
					CGameObject* pGameObject = CExplosion::Create(m_pGraphicDev, _vec3(vShotgunLeft.x, vShotgunLeft.y, vShotgunLeft.z), szName);
					NULL_CHECK_RETURN(pGameObject, -1);
					CLayer* pLayer = Get_Layer(STAGE_SKILL);
					FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), -1);

					CPoolMgr::GetInstance()->Reuse_ExBullet(m_pGraphicDev, &vShotgunLeft, &m_vDirectionLeft, 20.f);
				}
				if ((m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_3) && m_AnimationTime >= 1.f)
				{
					_tchar* szName = new _tchar[256]{};
					wstring wName = L"Explosion_%d";

					wsprintfW(szName, wName.c_str(), g_iExplosion);
					m_TcharList.push_back(szName);

					g_iExplosion++;

					// 위치 바꿔줘야함
					CGameObject* pGameObject = CExplosion::Create(m_pGraphicDev, _vec3(vShotgunRight.x, vShotgunRight.y, vShotgunRight.z), szName);
					NULL_CHECK_RETURN(pGameObject, -1);
					CLayer* pLayer = Get_Layer(STAGE_SKILL);
					FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), -1);

					CPoolMgr::GetInstance()->Reuse_ExBullet(m_pGraphicDev, &vShotgunRight, &m_vDirectionRight, 20.f);
				}
			}
			else if (m_PATTERN == MIDDLEBOSS_SKILL_BOMBING)
			{
				if ((m_BOMBING >= MIDDLEBOSS_BOMBING_4 && m_BOMBING >= MIDDLEBOSS_BOMBING_7) && m_fMissileItv >= 0.5f)
				{
					CLayer* pLayer = Get_Layer(STAGE_SKILL);
					_tchar* szName = new _tchar[256]{};
					wstring wName = L"Missile_%d";
					wsprintfW(szName, wName.c_str(), m_MissileCnt);
					m_TcharList.push_back(szName);

					_vec3 vRandom;
					if (m_MissileCnt % 5 == 0)
						vRandom = _vec3(vPlayerPos.x + m_iRand, 30.f, vPlayerPos.z + m_iRand);
					else if (m_MissileCnt % 5 == 1)
						vRandom = _vec3(vPlayerPos.x + m_iRand, 30.f, vPlayerPos.z - m_iRand);
					else if (m_MissileCnt % 5 == 2)
						vRandom = _vec3(vPlayerPos.x - m_iRand, 30.f, vPlayerPos.z + m_iRand);
					else if (m_MissileCnt % 5 == 3)
						vRandom = _vec3(vPlayerPos.x - m_iRand, 30.f, vPlayerPos.z - m_iRand);
					else if (m_MissileCnt % 5 == 4)
						vRandom = _vec3(vPlayerPos.x + m_iRand, 30.f, vPlayerPos.z + m_iRand);

					CGameObject* pGameObject = CVeneer::Create(m_pGraphicDev, vRandom);
					NULL_CHECK_RETURN(pGameObject, -1);
					FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), -1);

					// 위치 바꿔줘야함
					pGameObject = CFlightBomb::Create(m_pGraphicDev, vRandom, szName);
					NULL_CHECK_RETURN(pGameObject, -1);
					FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), -1);

					m_MissileCnt++;
					m_fMissileItv = 0.f;
				}
				else
				{
					m_fMissileItv += fTimeDelta;
				}
			}
			else if (m_PATTERN == MIDDLEBOSS_SKILL_CRASH)
			{
				CLayer* pLayer = Engine::Get_Layer(STAGE_CHARACTER);
				CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(pLayer->Get_GameObject(L"PLAYER"));

				pPlayer->KnuckDown(10.f, 20.f);
			}
		}
	}

	return 0;
}

void CMiddleBoss::Hit_Check(_float _deltaTime)
{
	//m_pTransCom->Static_Update();

	CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
	CWeapon* pWeapon = pPlayer->Get_Weapon();
	if (pWeapon)
	{
		_vec3 vSrcPos;
		pWeapon->Get_Transform()->Get_Info(INFO_POS, &vSrcPos);

		_vec3 vDir;
		if (m_pCollision->HitScan(g_hWnd, &vSrcPos, this->m_pBufferCom, this->m_pHitBoxTransCom, &vDir))
		{
			if (pWeapon->Get_Shoot() == true)
			{
				m_tAbility->fCurrentHp -= pWeapon->Get_Ability()->fBulletAttack;
				Hit_Effect();
				m_pComboUI->On_Switch();
				m_pComboUI->ComboCntPlus();
	
				pWeapon->Set_Shoot(false);
			}

			if (m_tAbility->fCurrentHp <= 0.f)
			{
				m_tAbility->fCurrentHp = 0.f;
			}
		}
		if (m_fBeforeHp != m_tAbility->fCurrentHp)
		{
			m_fBeforeHp = m_tAbility->fCurrentHp;

			m_pMonsterUI->Set_Name(m_tAbility->strObjTag);
			m_pMonsterUI->Set_Hp(m_tAbility->fCurrentHp);
			m_pMonsterUI->Set_MaxHp(m_tAbility->fMaxHp);
			m_pMonsterUI->On_Switch();
		}
		else
		{
			m_fUISwitchTime += _deltaTime;
			if (m_fUISwitchTime >= 5.f)
			{
				m_pMonsterUI->Off_Switch();
				m_fUISwitchTime = 0.f;
			}
		}
	}
}

HRESULT CMiddleBoss::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Thor/THOR.dat",      // 파일의 경로와 이름	
		GENERIC_READ,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		OPEN_EXISTING,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	DWORD   dwByte = 0;

	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
	_int	iDrawNum = 0;
	_float	fAxisX = 0.f, fAxisY = 0.f, fAxisZ = 0.f;
	_int	iChildCnt = 0;
	_int	iParentCnt = 0;

	int iSize = 0;
	ReadFile(hFile, &iSize, sizeof(int), &dwByte, nullptr);

	list<const _tchar*>	LoadOrder;
	for (int i = 0; i < iSize; ++i)
	{
		_tchar* szName = new _tchar[256]{};
		ReadFile(hFile, szName, sizeof(_tchar[256]), &dwByte, nullptr);
		LoadOrder.push_back(szName);
		m_TcharList.push_back(szName);
	}

	while (true)
	{
		ReadFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &iDrawNum, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &fAxisX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fAxisY, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fAxisZ, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &iChildCnt, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &iParentCnt, sizeof(_int), &dwByte, nullptr);

		CGameObject *pGameObject = nullptr;

		pGameObject = CTransAxisBox::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pMyLayer, E_FAIL);

		FAILED_CHECK_RETURN(pMyLayer->Add_GamePair(LoadOrder.front(), pGameObject), E_FAIL);

		pGameObject->Set_DrawTexIndex(iDrawNum);

		dynamic_cast<CTransAxisBox*>(pGameObject)->Set_Axis(fAxisX, fAxisY, fAxisZ);

		CTransform* Transcom = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Proto_TransformCom", ID_STATIC));

		Transcom->Set_Info(INFO_RIGHT, &vRight);
		Transcom->Set_Info(INFO_UP, &vUp);
		Transcom->Set_Info(INFO_LOOK, &vLook);
		Transcom->Set_Info(INFO_POS, &vPos);
		Transcom->Set_Angle(&vAngle);
		Transcom->Set_Scale(&vScale);

		Transcom->Worldspace_By_Quarternion();

		_vec3	vPos;
		Transcom->Get_Info(INFO_POS, &vPos);

		_vec3	vPlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);

		if (0 == _tcscmp(LoadOrder.front(), L"A_ROOT"))	//	루트 이름은 항상 고정이므로 이렇게 지정해둠, 루트를 몸통으로 사용해도 좋고 아주 작게 만들어서 바닥으로 해도 좋고? 근데 거기까진 안해봤습니다ㅎ
			Transcom->Set_Pos(vPos.x + vPlayerPos.x, vPos.y + vPlayerPos.y, vPos.z + vPlayerPos.z);
		else
			Transcom->Set_Pos(vPos.x, vPos.y, vPos.z);

		Transcom->Worldspace_By_Quarternion();

		LoadOrder.pop_front();

		if (iChildCnt)
		{
			for (int i = 0; i < iChildCnt; ++i)
			{
				_tchar* szName = new _tchar[256]{};
				ReadFile(hFile, szName, sizeof(_tchar[256]), &dwByte, nullptr);
				dynamic_cast<CTransAxisBox*>(pGameObject)->m_ChildKey.push_back(szName);
				m_TcharList.push_back(szName);
			}
		}

		if (iParentCnt)
		{
			for (int i = 0; i < iParentCnt; ++i)
			{
				_tchar* szName = new _tchar[256]{};
				ReadFile(hFile, szName, sizeof(_tchar[256]), &dwByte, nullptr);
				dynamic_cast<CTransAxisBox*>(pGameObject)->m_ParentKey.push_back(szName);
				m_TcharList.push_back(szName);
			}
		}

		iSize--;

		if (0 == dwByte)
			break;

		if (0 == iSize)
			break;
	}

	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		for (auto& List : dynamic_cast<CTransAxisBox*>(iter.second)->m_ParentKey)
		{
			auto	MapFindIter = find_if(pMyLayer->Get_GamePairPtr()->begin(), pMyLayer->Get_GamePairPtr()->end(), CTag_Finder(List));

			dynamic_cast<CTransAxisBox*>(iter.second)->m_ParentList.push_back(dynamic_cast<CTransAxisBox*>(MapFindIter->second));
		}
	}

	CloseHandle(hFile);
	return S_OK;
}

void CMiddleBoss::Load_Animation(wstring FileName, _uint AnimationID)
{
	HANDLE      hFile = CreateFile(FileName.c_str(),      // 파일의 경로와 이름
		GENERIC_READ,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		OPEN_EXISTING,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD   dwByte = 0;

	int iSize = 0;
	ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	list<const _tchar*>	LoadOrder;
	for (int i = 0; i < iSize; ++i)
	{
		_tchar* szName = new _tchar[256]{};
		ReadFile(hFile, szName, sizeof(_tchar[256]), &dwByte, nullptr);
		LoadOrder.push_back(szName);
		m_TcharList.push_back(szName);
	}

	for (auto& iter : LoadOrder)
	{
		auto	MapFindIter = find_if(pMyLayer->Get_GamePairPtr()->begin(), pMyLayer->Get_GamePairPtr()->end(), CTag_Finder(iter));

		CQuarternion* Quaternion = dynamic_cast<CQuarternion*>(MapFindIter->second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));

		if (Quaternion->Get_WorldVector()->size() >= 2)
		{
			CloseHandle(hFile);
			return;
		}

		_int iAnimationSize = 0;
		ReadFile(hFile, &iAnimationSize, sizeof(_int), &dwByte, nullptr);
		for (int i = 0; i < iAnimationSize; ++i)
		{
			_matrix matAnimation;
			ReadFile(hFile, &matAnimation, sizeof(D3DXMATRIX), &dwByte, nullptr);
			Quaternion->Fill_Animation(&matAnimation, AnimationID);
		}

		iSize--;

		if (0 == dwByte)
			break;

		if (0 == iSize)
			break;
	}

	CloseHandle(hFile);
}

void CMiddleBoss::Run_Animation(const _float & AnimationSpeed)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		if (Qtan->Get_WorldVector()->size() < 2)
			return;

		_matrix matFront = (Qtan->Get_WorldVector())->front();
		_matrix matLast = (Qtan->Get_WorldVector())->back();

		D3DXQUATERNION q1, q2, qSLerp;
		_vec3 vScale1, vTrans1, vScale2, vTrans2, vScaleLerp, vTransLerp;

		D3DXMatrixDecompose(&vScale1, &q1, &vTrans1, &matFront);
		D3DXMatrixDecompose(&vScale2, &q2, &vTrans2, &matLast);

		D3DXVec3Lerp(&vScaleLerp, &vScale1, &vScale2, m_AnimationTime);
		D3DXQuaternionSlerp(&qSLerp, &q1, &q2, m_AnimationTime);
		D3DXVec3Lerp(&vTransLerp, &vTrans1, &vTrans2, m_AnimationTime);

		m_AnimationTime += (m_fTimeDelta / AnimationSpeed);

		_float pitch, yaw, roll;

		FLOAT sqw = qSLerp.w * qSLerp.w;
		FLOAT sqx = qSLerp.x * qSLerp.x;
		FLOAT sqy = qSLerp.y * qSLerp.y;
		FLOAT sqz = qSLerp.z * qSLerp.z;

		pitch = asinf(2.f * (qSLerp.w * qSLerp.x - qSLerp.y * qSLerp.z));
		yaw = atan2f(2.0f * (qSLerp.x * qSLerp.z + qSLerp.w * qSLerp.y), (-sqx - sqy + sqz + sqw));
		roll = atan2f(2.0f * (qSLerp.x * qSLerp.y + qSLerp.w * qSLerp.z), (-sqx + sqy - sqz + sqw));

		_matrix matNewWorld, matNewScale, matNewRot, matNewTrans;
		D3DXMatrixScaling(&matNewScale, vScaleLerp.x, vScaleLerp.y, vScaleLerp.z);
		D3DXMatrixRotationQuaternion(&matNewRot, &qSLerp);
		D3DXMatrixTranslation(&matNewTrans, vTransLerp.x, vTransLerp.y, vTransLerp.z);
		matNewWorld = matNewScale * matNewRot * matNewTrans;

		CTransform*	BoxTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));
		_matrix matWorld;
		BoxTransform->Set_WorldMatrix(&matNewWorld);
		BoxTransform->Set_Scale(&vScaleLerp);
		BoxTransform->Set_Angle(&_vec3(yaw, pitch, roll));

		_vec3		vPlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
			BoxTransform->Set_Pos(vTransLerp.x + vPlayerPos.x, vTransLerp.y + vPlayerPos.y, vTransLerp.z + vPlayerPos.z);
		else
			BoxTransform->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
	}
}

void CMiddleBoss::Look_Direction(void)
{
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	D3DXQUATERNION qRot;
	D3DXMatrixDecompose(&_vec3(), &qRot, &_vec3(), &matWorld);

	_float pitch, yaw, roll;

	FLOAT sqw = qRot.w * qRot.w;
	FLOAT sqx = qRot.x * qRot.x;
	FLOAT sqy = qRot.y * qRot.y;
	FLOAT sqz = qRot.z * qRot.z;

	pitch = asinf(2.f * (qRot.w * qRot.x - qRot.y * qRot.z));
	yaw = atan2f(2.0f * (qRot.x * qRot.z + qRot.w * qRot.y), (-sqx - sqy + sqz + sqw));
	roll = atan2f(2.0f * (qRot.x * qRot.y + qRot.w * qRot.z), (-sqx + sqy - sqz + sqw));

	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	for (auto& iter : ListBox)
	{
		if (0 == _tcscmp(iter.first, L"A_ROOT"))
		{
			_vec3 vAngle;
			CTransform* Transform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));
			Transform->Get_Angle(&vAngle);
			Transform->Set_Angle(&_vec3(yaw, vAngle.x, vAngle.z));
		}
	}
}

void CMiddleBoss::Walk_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}
		if (m_WALK == MIDDLEBOSSWALK_1)
			m_WALK = MIDDLEBOSSWALK_2;
		else if (m_WALK == MIDDLEBOSSWALK_2)
			m_WALK = MIDDLEBOSSWALK_3;
		else if (m_WALK == MIDDLEBOSSWALK_3)
			m_WALK = MIDDLEBOSSWALK_1;

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(4 + m_WALK);
	}

	
}

void CMiddleBoss::Set_OnTerrain(void)
{
	_vec3 vAnimationPos;

	_vec3		vPos;
	m_pTransCom->Get_BeforeInfo(INFO_POS, &vAnimationPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", TERRAINTEX_COMP, ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vAnimationPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ) + 0.6f;

	m_pTransCom->Set_Pos(vAnimationPos.x, fHeight + 12.f, vAnimationPos.z);
}

_bool CMiddleBoss::Collision_Wall(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_vec3	vDir(0, 0, 0);
	_vec3	vNormal(0, 0, 0);

	_int iCollision = m_pCollision->Wall_Collision_For_Monster(&vNormal, this->m_pTransCom, this->m_pHitBox);

	if (-1 != iCollision)
	{
		float fDot = D3DXVec3Dot(&vNormal, &vDir);
		float fDiagonal = acosf(fDot);

		if (iCollision == WALL_RIGHT || iCollision == WALL_LEFT || iCollision == WALL_BACK)
		{
			if (D3DXToDegree(fDiagonal) > 90.f)
			{
				_vec3 vSliding = vDir;
				m_pCollision->Wall_Collision_By_DotSliding_For_Monster(&vSliding, this->m_pTransCom, this->m_pHitBox);

				m_pTransCom->Move_Pos(&(vSliding * m_fSpeed * fTimeDelta));
			}
			else
			{
				vDir = _vec3(1, 0, 0);
				m_pTransCom->Move_Pos(&(vDir * m_fSpeed * fTimeDelta));
			}
			return true;
		}
		if (iCollision == WALL_FRONT)
		{
			if (D3DXToDegree(fDiagonal) < 90.f)
			{
				_vec3 vSliding = vDir;
				m_pCollision->Wall_Collision_By_DotSliding_For_Monster(&vSliding, this->m_pTransCom, this->m_pHitBox);

				m_pTransCom->Move_Pos(&(vSliding * m_fSpeed * fTimeDelta));
			}
			else
			{
				vDir = _vec3(-1, 0, 0);
				m_pTransCom->Move_Pos(&(vDir * m_fSpeed * fTimeDelta));
			}
			return true;
		}
	}
	else
	{
		m_pTransCom->Move_Pos(&(vDir * m_fSpeed * fTimeDelta));
		return false;
	}

	return false;
}

void CMiddleBoss::Idle_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_IDLE == MIDDLEBOSSIDLE_1)
			m_IDLE = MIDDLEBOSSIDLE_2;
		else if (m_IDLE == MIDDLEBOSSIDLE_2)
			m_IDLE = MIDDLEBOSSIDLE_3;
		else if (m_IDLE == MIDDLEBOSSIDLE_3)
			m_IDLE = MIDDLEBOSSIDLE_4;
		else if (m_IDLE == MIDDLEBOSSIDLE_4)
			m_IDLE = MIDDLEBOSSIDLE_1;

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(0 + m_IDLE);
	}
	
}

void CMiddleBoss::NormalAttack_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_1)
			m_NORMALATTACK = MIDDLEBOSS_NORMALATTACK_2;
		else if(m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_2)
			m_NORMALATTACK = MIDDLEBOSS_NORMALATTACK_3;
		else if(m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_3)
			m_NORMALATTACK = MIDDLEBOSS_NORMALATTACK_4;
		else if (m_NORMALATTACK == MIDDLEBOSS_NORMALATTACK_4)
		{
			m_NORMALATTACK = MIDDLEBOSS_NORMALATTACK_1;
			m_STATE = MIDDLEBOSS_IDLE;
		}

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(4 + m_NORMALATTACK);
	}
}

void CMiddleBoss::Bombing_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_BOMBING == MIDDLEBOSS_BOMBING_1)
			m_BOMBING = MIDDLEBOSS_BOMBING_2;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_2)
			m_BOMBING = MIDDLEBOSS_BOMBING_3;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_3)
			m_BOMBING = MIDDLEBOSS_BOMBING_4;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_4)
			m_BOMBING = MIDDLEBOSS_BOMBING_5;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_5)
			m_BOMBING = MIDDLEBOSS_BOMBING_6;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_6)
			m_BOMBING = MIDDLEBOSS_BOMBING_7;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_7)
			m_BOMBING = MIDDLEBOSS_BOMBING_8;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_8)
			m_BOMBING = MIDDLEBOSS_BOMBING_9;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_9)
			m_BOMBING = MIDDLEBOSS_BOMBING_10;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_10)
			m_BOMBING = MIDDLEBOSS_BOMBING_11;
		else if (m_BOMBING == MIDDLEBOSS_BOMBING_11)
		{
			m_BOMBING = MIDDLEBOSS_BOMBING_1;
			m_STATE = MIDDLEBOSS_IDLE;
		}

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(11 + m_BOMBING);
	}
}

void CMiddleBoss::Dead_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_DEAD == MIDDLEBOSS_DEAD_1)
			m_DEAD = MIDDLEBOSS_DEAD_2;
		else if (m_DEAD == MIDDLEBOSS_DEAD_2)
			m_DEAD = MIDDLEBOSS_DEAD_3;

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(22 + m_DEAD);
	}
}

HRESULT CMiddleBoss::Monster_Mapping(void)
{
	_vec3		vPos;
	m_pHitBoxTransCom->Get_Info(INFO_POS, &vPos);
	if (!m_MappingInit)
	{
		CGameObject*	m_pMapMonster = CMonsterMapping::Create(m_pGraphicDev);
		wsprintf(m_szCntName, L"");
		const _tchar*	szNumbering = L"MapMonster_%d";
		wsprintf(m_szCntName, szNumbering, m_iCnt);
		Engine::Add_GameObject(STAGE_MAPPING, m_pMapMonster, m_szCntName);
		m_listMonsterCnt.push_back(m_szCntName);

		m_pMonsterMapping = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_MAPPING, m_szCntName, TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pMonsterMapping, E_FAIL);
		++m_iCnt;
		m_MappingInit = true;

	}
	m_pMonsterMapping->Set_Pos(vPos.x, vPos.y, vPos.z);
	return S_OK;

}

HRESULT CMiddleBoss::Create_Monster()
{
	random_shuffle(m_vMonterPos.begin(), m_vMonterPos.end());
	_vec3 vPos = m_vMonterPos.front();

	random_shuffle(m_vMonstertype.begin(), m_vMonstertype.end());
	_int i = m_vMonstertype.front();

	D3DLIGHT9 d3dLight;
	ZeroMemory(&d3dLight, sizeof(d3dLight));

	d3dLight.Type = D3DLIGHT_POINT;
	d3dLight.Range = 10.f;
	d3dLight.Diffuse.r = 0.f;
	d3dLight.Diffuse.g = 0.f;
	d3dLight.Diffuse.b = 1.f;
	d3dLight.Ambient.r = 0.f;
	d3dLight.Ambient.g = 0.f;
	d3dLight.Ambient.b = 1.f;
	d3dLight.Specular.r = 1.0f;
	d3dLight.Specular.g = 1.0f;
	d3dLight.Specular.b = 1.0f;
	d3dLight.Attenuation0 = 0.1f;
	d3dLight.Position = vPos;
	FAILED_CHECK_RETURN(m_pGraphicDev->SetLight(2, &d3dLight), E_FAIL);

	CGameObject* pGameObject = nullptr;
	CLayer* pLayer = Get_Layer(STAGE_MONSTER);

	if (i == 0)
	{
		_tchar* szName = new _tchar[128]{};
		wstring wName = L"ZomBie_boss_%d";
		wsprintfW(szName, wName.c_str(), m_iMonsterCnt);
		NameList.push_back(szName);
		pGameObject = CZombie::Create(m_pGraphicDev, vPos, szName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), E_FAIL);
		m_iMonsterCnt++;
	}
	else if (i == 1)
	{
		_tchar* szName = new _tchar[128]{};
		wstring wName = L"Alien_boss_%d";
		wsprintfW(szName, wName.c_str(), m_iMonsterCnt);
		NameList.push_back(szName);
		pGameObject = CAlien::Create(m_pGraphicDev, vPos, szName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), E_FAIL);
		m_iMonsterCnt++;
	}
	return S_OK;
}

HRESULT CMiddleBoss::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Engine::Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pComponent });

	pComponent = m_pHitBoxTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"HitBox_Transform", pComponent });

	pComponent = m_pTransUICom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransUICom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformUICom", pComponent });

	pComponent = m_pBufferUICom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_MONTER_HP_COMP));
	NULL_CHECK_RETURN(m_pBufferUICom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_MONTER_HP_COMP, pComponent });

	pComponent = m_pTextureUICom = dynamic_cast<CTexture*>(Clone_Proto(L"Monster_General_HP"));
	NULL_CHECK_RETURN(m_pTextureUICom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Monster_General_HP", pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pComponent });

	pComponent = m_pSearchRange_TransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"SearchRange", pComponent });

	pComponent = m_pAttackRange_TransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"AttackRange", pComponent });

	pComponent = m_pRunawayRange_TransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"RunawayRange", pComponent });

	pComponent = m_pSphereTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Sphere_TransCom", pComponent });

	pComponent = m_pAnimationBox = dynamic_cast<CCubeCol*>(Clone_Proto(CUBECOL_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBECOL_COMP, pComponent });

	return S_OK;
}
void CMiddleBoss::Hit_Effect(void)
{
	_float fSound = 1.f;
	PlaySoundW(L"MBDestroy.wav", SOUND_MB, fSound);
	if (pMyLayer != nullptr)
	{
		for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
		{
			if (0 == _tcscmp(iter.first, L"CORE"))//코어위치
			{
				CTransAxisBox* pCoreBox = dynamic_cast<CTransAxisBox*>(iter.second);
				_matrix matFinal;
				pCoreBox->Get_Final(&matFinal);
				m_vCorePos = { matFinal.m[3][0], matFinal.m[3][1], matFinal.m[3][2] };
			}
		}
	}
	CMHitEffect* pMHitEffect = nullptr;
	if (!pMHitEffect)
		pMHitEffect = dynamic_cast<CMHitEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"MHitEffect"));
	//_vec3 vPos;                                          //보스죽는이팩트
	_vec3 vDir;
	_vec3 min = { -1.0f ,-1.0f ,-1.0f };
	for (_int i = -5; i < 5; i++)
	{
		for (_int j = -5; j < 5; j++)
		{
			for (_int k = -5; k < 5; k++)
			{
				D3DXVec3Normalize(&min, &_vec3(i, j, k));
				dynamic_cast<CMHitEffect*>(pMHitEffect)->Set_PclePos(m_vCorePos + _vec3(i, j, k)*.1f);
				dynamic_cast<CMHitEffect*>(pMHitEffect)->Set_PcleDir(min*0.25f);
				pMHitEffect->addParticle();
			}
		}
	}
}
void CMiddleBoss::LaserReady_Effect(void)
{
	CMBLaserReady* pMBLaserReady = nullptr;
	if (!pMBLaserReady)
		pMBLaserReady = dynamic_cast<CMBLaserReady*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"MBLaserReady"));
	//_vec3 vPos;                                          //보스죽는이팩트
	_vec3 vDir;
	_vec3 min = { -1.0f ,-1.0f ,-1.0f };
	for (_int i = -5; i < 5; i++)
	{
		for (_int j = -5; j < 5; j++)
		{
			for (_int k = -5; k < 5; k++)
			{
				//D3DXVec3Normalize(&min, &_vec3(i, j, k));
				dynamic_cast<CMBLaserReady*>(pMBLaserReady)->Set_PclePos(m_vCorePos + _vec3(i, j, k)*1.f);
				dynamic_cast<CMBLaserReady*>(pMBLaserReady)->Set_PcleDir(-_vec3(i, j, k)*1.f);
				pMBLaserReady->addParticle();
			}
		}
	}
}
void CMiddleBoss::BombState_Effect(void)
{
	CMBBomb* pMBBomb = nullptr;
	if (!pMBBomb)
		pMBBomb = dynamic_cast<CMBBomb*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"MBBomb"));
	//_vec3 vPos;                                          //보스죽는이팩트
	_vec3 vDir;
	_vec3 min = { -1.0f ,-1.0f ,-1.0f };
	for (_int i = -5; i < 6; i++)
	{
		for (_int j = -5; j < 6; j++)
		{
			for (_int k = -5; k < 6; k++)
			{
				D3DXVec3Normalize(&min, &_vec3(i, j, k));
				dynamic_cast<CMBBomb*>(pMBBomb)->Set_PclePos(m_vCorePos + _vec3(i, j - 1.f, k)*.45f);
				dynamic_cast<CMBBomb*>(pMBBomb)->Set_PcleDir((-min)* .5f);
				pMBBomb->addParticle();
			}
		}
	}
}
void CMiddleBoss::Dead_Effect(void)
{
	_float fSound = 1.f;
	PlaySoundW(L"MBDead.wav", SOUND_MB, fSound);
	CStaticCamera*      pStaticCam = nullptr;
	if (pStaticCam == nullptr)
		pStaticCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"));
	pStaticCam->CameraShaking();
	if (pMyLayer != nullptr)
	{
		for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
		{
			if (0 == _tcscmp(iter.first, L"CORE"))//코어위치
			{
				CTransAxisBox* pCoreBox = dynamic_cast<CTransAxisBox*>(iter.second);
				_matrix matFinal;
				pCoreBox->Get_Final(&matFinal);
				m_vCorePos = { matFinal.m[3][0], matFinal.m[3][1], matFinal.m[3][2] };
			}
		}
	}
	CMFieldEffect* pMFieldEffect = nullptr;
	if (!pMFieldEffect)
		pMFieldEffect = dynamic_cast<CMFieldEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"MFieldEffect"));
	//_vec3 vPos;                                          //보스죽는이팩트
	_vec3 vDir;
	_vec3 min = { -1.0f ,-1.0f ,-1.0f };
	for (_int i = -3; i < 3; i++)
	{
		for (_int j = -3; j < 3; j++)
		{
			for (_int k = -3; k < 3; k++)
			{
				D3DXVec3Normalize(&min, &_vec3(i, j, k));
				dynamic_cast<CMFieldEffect*>(pMFieldEffect)->Set_PclePos(m_vCorePos + _vec3(i, j, k)*2.5f);
				dynamic_cast<CMFieldEffect*>(pMFieldEffect)->Set_PcleDir(-_vec3(i, j, k));
				pMFieldEffect->addParticle();
			}
		}
	}
}

CMiddleBoss * CMiddleBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, _tchar * Name)
{
	CMiddleBoss * pInstance = new CMiddleBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, Name)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiddleBoss::Free(void)
{
	/*if (pMyLayer != nullptr)
	{
		for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
		{
			iter.second->Kill_Obj();
		}
	}*/

	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	Safe_Delete_Array(m_szCntName);
	
	m_listMonsterCnt.clear();	
	CGameObject::Free();
	Safe_Delete<MIDDLEBOSSABILITY*>(m_tAbility);
}
