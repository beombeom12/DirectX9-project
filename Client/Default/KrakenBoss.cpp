#include "stdafx.h"
#include "KrakenBoss.h"
#include "MonsterUI.h"
#include "ComboUI.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "TransAxisBox.h"
#include "KrakenLeg.h"
#include "PoolMgr.h"
#include "Meteor.h"
#include "StaticCamera.h"
#include "BattleCursier.h"
#include "Flight.h"
#include "Warning_AnnihilateUI.h"
#include "Zombie.h"
#include "FireMan.h"
#include "Alien.h"

CKrakenBoss::CKrakenBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CKrakenBoss::~CKrakenBoss()
{
}

HRESULT CKrakenBoss::Ready_Object(const _vec3 & vPos, _tchar * Name)
{
	m_tAbility = new KRAKENABILITY;
	m_tAbility->fMaxHp = 10000.f;
	m_tAbility->fCurrentHp = m_tAbility->fMaxHp;
	m_tAbility->fDamage = 20.f;
	m_tAbility->strObjTag = L"Kraken";

	m_MonsterName = Name;

	m_STATE = KRAKEN_IDLE;
	m_IDLE = KRAKENIDLE_1;
	m_APPEAR = KRAKENAPPEAR_1;
	m_SMASH = KRAKENSMASH_1;
	m_INKSHOT = KRAKENINKSHOT_1;
	m_ROLLING = KRAKENROLLING_1;
	m_LURKER = KRAKENLURKER_1;
	m_PATTERN = KRAKEN_SKILL_END;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(&_vec3(10.f, 20.f, 10.f));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_pTransUICom->Set_Scale(1.f, 1.f, 1.5f);

	_vec3 vAnimationPos;
	m_pTransCom->Get_Info(INFO_POS, &vAnimationPos);


	m_pHitBoxTransCom->Set_Scale(&_vec3(10.f, 20.f, 10.f));
	m_pHitBoxTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pHitBoxTransCom->Static_Update();

	m_pSphereTransCom->Set_Scale(&_vec3(40.f, 40.f, 40.f));
	m_pSphereTransCom->Set_Pos(vAnimationPos.x, 0.f, vAnimationPos.z);
	m_pSphereTransCom->Static_Update();

	PlaySoundW(L"ZLuPss01.wav", SOUND_EFFECT, 1.f);
	return S_OK;
}

_int CKrakenBoss::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		m_pComboUI->KillCntPlus();
		return -1;
	}


	m_fTimeDelta = fTimeDelta;

	if (m_bFirst)
	{
		m_bFirst = false;

		//m_vPattern.push_back(KRAKEN_SKILL_SMASH);
		m_vPattern.push_back(KRAKEN_SKILL_INKSHOT);
		//m_vPattern.push_back(KRAKEN_SKILL_ROLLING);
		//m_vPattern.push_back(KRAKEN_SKILL_LURKER);
		m_vPattern.push_back(KRAKEN_SKILL_5);

		Engine::Get_Scene()->New_Layer(m_MonsterName);
		pMyLayer = Engine::Get_Layer(m_MonsterName);
		FAILED_CHECK_RETURN(Build(), -1);

		Load_Animation(L"../../Data/KraKen/KRAKEN_IDLE_1.dat", 0);
		Load_Animation(L"../../Data/KraKen/KRAKEN_IDLE_2.dat", 1);

		m_vMonstertype.push_back(0);
		m_vMonstertype.push_back(1);
		m_vMonstertype.push_back(2);
	}

	Update_Pattern(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	m_pTransUICom->Billboard_Transform(fTimeDelta);
	Hit_Check(fTimeDelta);

	_vec3 vMonsterPos;
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);
	m_pHitBoxTransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
	m_pSphereTransCom->Set_Pos(vMonsterPos.x, 0.f, vMonsterPos.z);
	m_pTransCom->Get_Info(INFO_POS, &vUIPos);
	m_pTransUICom->Set_Pos(vUIPos.x, vUIPos.y + 2.4f, vUIPos.z);

	//if (m_bPatternStart)
	{
		m_fMonCreFrame += fTimeDelta;

		if (m_fMonCreFrame >= 5.f)
		{
			Create_Monster();
			m_fMonCreFrame = 0.f;
		}
	}

	return 0;
}

void CKrakenBoss::Render_Object(void)
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(11);
	m_pBufferCom->Render_Buffer();*/

	/*m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pHitBoxTransCom->Get_WorldMatrixPointer());
	m_pHitBox->Render_Buffer();*/

	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pSphereTransCom->Get_WorldMatrixPointer());

	m_pSphereBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransUICom->Get_WorldMatrixPointer());

	m_pTextureUICom->Set_Texture(0);
	m_pBufferUICom->Resize_Buffer(m_tAbility->fCurrentHp / m_tAbility->fMaxHp);
	m_pBufferUICom->Render_Buffer();
}

void CKrakenBoss::LateUpdate_Object(void)
{
	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_pMonsterUI->Off_Switch();
		
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		m_pTransCom->Set_Pos(vPos.x, vPos.y - 0.1f, vPos.z);
		m_pTransCom->Static_Update();

		if (vPos.y <= -50.f)
		{
			this->Kill_Obj();
			if (m_bDeadScene)
			{
				Dead_Event();

				CGameObject* pGameObject = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));
				if (dynamic_cast<CFlight*>(pGameObject)->Get_Ending() == false)
					dynamic_cast<CFlight*>(pGameObject)->Set_Ending(true);

				m_bDeadScene = false;
			}
		}
	}

	//애니메이션 관련해서 run animation 이랑 각자 상황에 맞는 애니메이션 넣어주면됨.
	if (!m_bFirst)
	{
		IDLE_Animation_Run();
		Run_Animation(50.f);

		/*if (m_STATE == KRAKEN_APPEAR)
		{

		}
		else if (m_STATE == KRAKEN_IDLE)
		{

		}
		else if (m_STATE == KRAKEN_ATTACK)
		{

		}*/
	}

	CGameObject::LateUpdate_Object();

}

void CKrakenBoss::Hit_SphereCheck(_float _deltaTime)
{
	if (m_bCollisionDmg)
	{
		m_bCollisionDmg = false;
		m_tAbility->fCurrentHp -= 50.f;

	}
	if (!Get_Layer(STAGE_SKILL)->Get_GameList().empty())
	{
		m_pTransCom->Static_Update();

		for (auto& iter : Get_Layer(STAGE_SKILL)->Get_GameList())
		{
			if (iter->GetSphereSkill() == true)
			{
				if (m_pCollision->Sphere_Collision(this->m_pSphereTransCom, dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC)), 1.f, iter->GetSphereScale()*2.f))
				{
					m_tAbility->fCurrentHp -= dynamic_cast<CMeteor*>(iter)->Get_Attack();
				}
			}

		}
	}

	if (m_BeforeHp != m_tAbility->fCurrentHp)
	{
		m_BeforeHp = m_tAbility->fCurrentHp;

		m_pMonsterUI->Set_Name(m_tAbility->strObjTag);
		m_pMonsterUI->Set_Hp(m_tAbility->fCurrentHp);
		m_pMonsterUI->Set_MaxHp(m_tAbility->fMaxHp);
		m_pMonsterUI->On_Switch();

		m_pComboUI->On_Switch();
		m_pComboUI->ComboCntPlus();

		m_pComboUI->On_Switch();
		m_pComboUI->ComboCntPlus();
	}

	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_tAbility->fCurrentHp = 0.f;
	}
}

void CKrakenBoss::Look_Direction(void)
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
			Transform->Set_Angle(&_vec3(yaw, vAngle.y, vAngle.z));
		}
	}
}

_int CKrakenBoss::Update_Pattern(_float fTimeDelta)
{
	if (m_pPlayerTransCom == nullptr)
	{
		m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	}
	if (m_pMonsterUI == nullptr)
		m_pMonsterUI = dynamic_cast<CMonsterUI*>(Engine::Get_GameObject(STAGE_UI, L"MonsterUI"));

	if (m_pComboUI == nullptr)
		m_pComboUI = dynamic_cast<CComboUI*>(Engine::Get_GameObject(STAGE_UI, L"ComboUI"));

	_float Hp = m_tAbility->fCurrentHp / m_tAbility->fMaxHp;


	_vec3 vPlayerPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	_vec3 vPlayerScale;
	m_pPlayerTransCom->Get_Scale(&vPlayerScale);
	_vec3 vScale;
	m_pSphereTransCom->Get_Scale(&vScale);
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	if (Hp <= 0.f)
	{
		CLayer* pLayer = Engine::Get_Layer(STAGE_TENTACLE);

		for (auto& iter : *(pLayer->Get_GameListPtr()))
		{
			dynamic_cast<CKrakenLeg*>(iter)->Set_Dead();
		}
	}
	else if ((Hp <= 0.9f && Hp >= 0.6f && m_bAnihilate_First)						//다리레이어 이터레이터 돌리는부분
		|| (Hp <= 0.6f && Hp >= 0.3f && m_bAnihilate_Second)						//함수가 true면 워닝뜨게// 워닝끄는부분?
		|| (Hp <= 0.3f && Hp >= 0.f && m_bAnihilate_Third))
	{
		CLayer* pLayer = Engine::Get_Layer(STAGE_TENTACLE);

		int iReady = 0;
		for (auto& iter : *(pLayer->Get_GameListPtr()))
		{
			//워닝시작,
			dynamic_cast<CWarning_AnnihilateUI*>(Engine::Get_GameObject(STAGE_UI, WARNING_TEX))->OnSwitch();
			if (dynamic_cast<CKrakenLeg*>(iter)->Get_State() == KRAKEN_IDLE)
				iReady++;

			else
				break;
		}

		if (iReady == 8)
		{
			for (auto& iter : *(pLayer->Get_GameListPtr()))
			{
				dynamic_cast<CKrakenLeg*>(iter)->Start_Annihilate();					//패턴시작부분 워닝끄는부분
				dynamic_cast<CWarning_AnnihilateUI*>(Engine::Get_GameObject(STAGE_UI, WARNING_TEX))->OffSwitch();

			}
			
			if (m_bAnihilate_First)
				m_bAnihilate_First = false;
			else if (m_bAnihilate_First == false && m_bAnihilate_Second == true)
				m_bAnihilate_Second = false;
			else if (m_bAnihilate_Second == false)
				m_bAnihilate_Third = false;
		}
		else
		{
			for (auto& iter : *(pLayer->Get_GameListPtr()))
			{
				dynamic_cast<CKrakenLeg*>(iter)->Ready_Annihilate();
			}
		}
	}
	else if (m_STATE == KRAKEN_IDLE)
	{
		CLayer* pLayer = Engine::Get_Layer(STAGE_TENTACLE);
		for (auto& iter : *(pLayer->Get_GameListPtr()))
		{
			if (dynamic_cast<CKrakenLeg*>(iter)->Get_State() == KRAKEN_APPEAR)
				break;
			else
				m_ReloadTimer += fTimeDelta;
		}

		if (m_ReloadTimer >= 2.f)
		{
			m_STATE = KRAKEN_ATTACK;

			random_shuffle(m_vPattern.begin(), m_vPattern.end());
			m_PATTERN = m_vPattern.front();

			m_ReloadTimer = 0.f;
		}
	}
	else if (m_STATE == KRAKEN_ATTACK)
	{
		_vec3 vDir{ 1.f, 0.f, 0.f };

		if (m_PATTERN == KRAKEN_SKILL_INKSHOT)
		{
			_matrix matRotY, matTrans, matWorld;
			if (m_AnimationTime >= 1.f)
			{
				if (!m_pCollision->Sphere_Collision(this->m_pSphereTransCom, m_pPlayerTransCom, vPlayerScale.x, vScale.x))
				{
					for (m_fFireAngle = 0.f; m_fFireAngle < 360.f; m_fFireAngle += 5.f)
					{
						D3DXMatrixRotationY(&matRotY, D3DXToRadian(-m_fFireAngle));
						D3DXMatrixTranslation(&matTrans, vPos.x, 0.f, vPos.y);

						m_pTransCom->Get_WorldMatrix(&matWorld);
						matWorld = matRotY * matTrans;

						D3DXVec3TransformNormal(&vDir, &vDir, &matWorld);

						vPos.y = 1.4f;
						CPoolMgr::GetInstance()->Reuse_KrakenBullet(m_pGraphicDev, &vPos, &vDir, 10.f, 10.f);
					}
				}
				m_STATE = KRAKEN_IDLE;
			}

		}
		if (m_PATTERN == KRAKEN_SKILL_5)
		{
			if (m_AnimationTime >= 1.f)
			{
		
				if (!m_pCollision->Sphere_Collision(this->m_pSphereTransCom, m_pPlayerTransCom, vPlayerScale.x, vScale.x))
				{
					_vec3 vDirection;
					vDirection = vPlayerPos - vPos;
					vPos.y = 1.4f;
					CPoolMgr::GetInstance()->Reuse_KrakenBullet(m_pGraphicDev, &vPos, &vDirection, 10.f, 10.f);

				}
				m_STATE = KRAKEN_IDLE;
			}

		}
	}
	return 0;
}

HRESULT CKrakenBoss::Add_Component(void)
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

	pComponent = m_pSphereTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Sphere_TransCom", pComponent });

	pComponent = m_pAnimationBox = dynamic_cast<CCubeCol*>(Clone_Proto(CUBECOL_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBECOL_COMP, pComponent });


	return S_OK;
}

HRESULT CKrakenBoss::Create_Monster()
{
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	random_shuffle(m_vMonstertype.begin(), m_vMonstertype.end());
	_int i = m_vMonstertype.front();

	CGameObject* pGameObject = nullptr;
	CLayer* pLayer = Get_Layer(STAGE_MONSTER);

	if (i == 0)
	{
		_tchar* szName = new _tchar[128]{};
		wstring wName = L"ZomBie_boss_%d";
		wsprintfW(szName, wName.c_str(), m_iMonsterCnt);
		m_TcharList.push_back(szName);
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
		m_TcharList.push_back(szName);
		pGameObject = CAlien::Create(m_pGraphicDev, vPos, szName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), E_FAIL);
		m_iMonsterCnt++;
	}
	else if (i == 2)
	{
		_tchar* szName = new _tchar[128]{};
		wstring wName = L"FireMan_boss_%d";
		wsprintfW(szName, wName.c_str(), m_iMonsterCnt);
		m_TcharList.push_back(szName);
		pGameObject = CFireMan::Create(m_pGraphicDev, vPos, szName);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), E_FAIL);
		m_iMonsterCnt++;
	}
	return S_OK;
}

CKrakenBoss * CKrakenBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, _tchar * Name)
{
	CKrakenBoss* pInstance = new CKrakenBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, Name)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CKrakenBoss::Free(void)
{
	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	CGameObject::Free();
	Safe_Delete<KRAKENABILITY*>(m_tAbility);
}

void CKrakenBoss::Hit_Check(_float _deltaTime)
{
	m_pTransCom->Static_Update();

	CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"));
	CWeapon* pWeapon = pPlayer->Get_Weapon();

	if (pWeapon)
	{
		_vec3 vSrcPos;
		pWeapon->Get_Transform()->Get_Info(INFO_POS, &vSrcPos);

		_vec3 vDir;
		if (m_pCollision->HitScan(g_hWnd, &vSrcPos, this->m_pBufferCom, this->m_pTransCom, &vDir))
		{
			if (pWeapon->Get_Shoot() == true)
			{
				m_tAbility->fCurrentHp -= pWeapon->Get_Ability()->fBulletAttack;

				m_pComboUI->On_Switch();
				m_pComboUI->ComboCntPlus();

				pWeapon->Set_Shoot(false);
			}
		}
		if (m_BeforeHp != m_tAbility->fCurrentHp)
		{
			m_BeforeHp = m_tAbility->fCurrentHp;

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

	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_tAbility->fCurrentHp = 0.f;
	}

}

void CKrakenBoss::Dead_Event(void)
{
	CLayer* pLayer = Get_Layer(STAGE_CREATURE);

	CGameObject* pGameObject = nullptr;

	{
		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(10, 20, 0), _vec3(0, 0, 1), L"BATTLE1");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(30, 20, 10), _vec3(0, 0, 1), L"BATTLE2");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(50, 20, 20), _vec3(0, 0, 1), L"BATTLE3");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(70, 20, 30), _vec3(0, 0, 1), L"BATTLE4");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(90, 20, 20), _vec3(0, 0, 1), L"BATTLE5");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(110, 20, 10), _vec3(0, 0, 1), L"BATTLE6");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(130, 20, 0), _vec3(0, 0, 1), L"BATTLE7");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(20, 30, 0), _vec3(0, 0, 1), L"BATTLE8");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(40, 30, 10), _vec3(0, 0, 1), L"BATTLE9");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(60, 30, 20), _vec3(0, 0, 1), L"BATTLE10");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(80, 30, 20), _vec3(0, 0, 1), L"BATTLE11");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(100, 30, 10), _vec3(0, 0, 1), L"BATTLE12");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CBattleCursier::Create(m_pGraphicDev, _vec3(120, 30, 0), _vec3(0, 0, 1), L"BATTLE13");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
	}


	{
		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(0, 15, 0), _vec3(0, 0, 1), L"FLIGHT1");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(10, 15, 5), _vec3(0, 0, 1), L"FLIGHT2");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(20, 15, 0), _vec3(0, 0, 1), L"FLIGHT3");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(30, 15, 5), _vec3(0, 0, 1), L"FLIGHT4");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(40, 15, 0), _vec3(0, 0, 1), L"FLIGHT5");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(50, 15, 5), _vec3(0, 0, 1), L"FLIGHT6");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(60, 15, 0), _vec3(0, 0, 1), L"FLIGHT7");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(70, 15, 5), _vec3(0, 0, 1), L"FLIGHT8");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(80, 15, 0), _vec3(0, 0, 1), L"FLIGHT9");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(90, 15, 5), _vec3(0, 0, 1), L"FLIGHT10");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(100, 15, 0), _vec3(0, 0, 1), L"FLIGHT11");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(110, 15, 5), _vec3(0, 0, 1), L"FLIGHT12");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(120, 15, 0), _vec3(0, 0, 1), L"FLIGHT13");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );

		pGameObject = CFlight::Create(m_pGraphicDev, _vec3(130, 15, 5), _vec3(0, 0, 1), L"FLIGHT14");
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), );
	}
	CGameObject* pShuttle = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));
	dynamic_cast<CFlight*>(pShuttle)->Replace(_vec3(65, 15, -49), _vec3(0, 0, 0), _vec3(0, 0, 1));
	if (!m_bCameraShaking)
	{
		dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"))->CameraShaking();
		m_bCameraShaking = true;
	}
}

HRESULT CKrakenBoss::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Kraken/KRAKEN_TEST.dat",      // 파일의 경로와 이름	
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

		FAILED_CHECK_RETURN(pMyLayer->Add_GamePair(LoadOrder.front(), pGameObject), E_FAIL);	//	Map을 쓰지 않는다!!!!! Layer/노트 참고

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

	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))		//	List 개꿀
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

void CKrakenBoss::Load_Animation(wstring FileName, _uint AnimationID)
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

void CKrakenBoss::Run_Animation(const _float & AnimationSpeed)
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

void CKrakenBoss::APPEAR(void)
{
}

void CKrakenBoss::IDLE_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_IDLE == KRAKENIDLE_1)
			m_IDLE = KRAKENIDLE_2;
		else if (m_IDLE == KRAKENIDLE_2)
			m_IDLE = KRAKENIDLE_1;

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(0 + m_IDLE);
	}
}

void CKrakenBoss::Set_OnTerrain(void)
{
	_vec3 vAnimationPos;

	m_pTransCom->Get_Info(INFO_POS, &vAnimationPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", TERRAINTEX_COMP, ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vAnimationPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ) + 10.f;;

	m_pTransCom->Set_Pos(vAnimationPos.x, fHeight, vAnimationPos.z);
}
