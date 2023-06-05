#include "stdafx.h"
#include "..\Header\Zombie.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "HealthPotion.h"
#include "ObtainDefense.h"
#include "ObtainBullet.h"
#include "PoolMgr.h"
#include "ComboUI.h"
#include "IceEffect.h"
#include "TransAxisBox.h"
#include "DeadParticle.h"
CZombie::CZombie(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CZombie::~CZombie()
{
}

HRESULT CZombie::Ready_Object(const _vec3& vPos, _tchar* Name)
{
	// 수치 값 넣어주기
	m_tAbility = new MONSTERABILITY;
	m_tAbility->iLevel = 0;
	m_tAbility->fMaxHp = 100.f;
	m_tAbility->fCurrentHp = m_tAbility->fMaxHp;
	m_tAbility->fDamage = 10.f;
	m_tAbility->strObjTag = L"Zombie";

	m_BeforeHp = m_tAbility->fMaxHp;

	m_STATE = ZOMBIE_IDLE;
	m_WALK = ZOMBIEWALK_START;
	m_IDLE = ZOMBIEIDLE_1;
	m_ATTACK = ZOMBIEATTACK_1;
	m_DEAD = ZOMBIEDEAD_1;

	m_MonsterName = Name;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 리얼 움직이는거고
	m_pTransCom->Set_Scale(&_vec3(0.3f, 0.3f, 0.3f));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	// UI
	m_pTransUICom->Set_Scale(1.f, 0.1f, 0.f);

	_vec3 vAnimationPos;
	m_pTransCom->Get_Info(INFO_POS, &vAnimationPos);

	// 플레이어 총알 맞으려고있는 히트박스고
	m_pHitBoxTransCom->Set_Scale(&_vec3(0.3f, 0.3f, 0.3f));
	m_pHitBoxTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pHitBoxTransCom->Static_Update();

	// ChaseTarget 범위 지정
	m_pSphereTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pSphereTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pSphereTransCom->Static_Update();

	m_pSearchRange_TransCom->Set_Scale(&_vec3(15.f, 15.f, 15.f));
	m_pSearchRange_TransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pSearchRange_TransCom->Static_Update();

	m_pAttackRange_TransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pAttackRange_TransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pAttackRange_TransCom->Static_Update();

	return S_OK;
}

_int CZombie::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Effect();
		Create_Item();
		m_pComboUI->KillCntPlus();
		Monster_DeleteMapping();

		return -1;
	}

	if (m_bFirst)
	{
		m_bFirst = false;

		Engine::Get_Scene()->New_Layer(m_MonsterName);
		pMyLayer = Engine::Get_Layer(m_MonsterName);

		FAILED_CHECK_RETURN(Build(), -1);

		Load_Animation(L"../../Data/Zombie/Zombie_Idle_1.dat", 0);
		Load_Animation(L"../../Data/Zombie/Zombie_Idle_2.dat", 1);

		Load_Animation(L"../../Data/Zombie/Zombie_Walk_1.dat", 2);
		Load_Animation(L"../../Data/Zombie/Zombie_Walk_2.dat", 3);
		Load_Animation(L"../../Data/Zombie/Zombie_Walk_3.dat", 4);

		Load_Animation(L"../../Data/Zombie/Zombie_Attack.dat", 5);

		Load_Animation(L"../../Data/Zombie/Zombie_Dead.dat", 6);
	}
	if (m_iSphereSkillTag != SKILL_STATICFIELD)
		m_fTimeDelta = fTimeDelta;
	if (m_iSphereSkillTag == SKILL_STATICFIELD)
		m_fTimeDelta = 0.f;

	CMonster::Update_Object(fTimeDelta);

	_vec3 vPlayerPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	_vec3 vPlayerScale;
	m_pPlayerTransCom->Get_Scale(&vPlayerScale);
	_vec3 vScale;
	m_pSphereTransCom->Get_Scale(&vScale);
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_vec3 vSearchScale, vAttackScale, vRunScale;
	m_pSearchRange_TransCom->Get_Scale(&vSearchScale);
	m_pAttackRange_TransCom->Get_Scale(&vAttackScale);

	if (!Collision_Wall(fTimeDelta))
	{
		if (m_pCollision->Sphere_Collision(this->m_pAttackRange_TransCom, m_pPlayerTransCom, vPlayerScale.x, vAttackScale.x))
		{
			// 공격충돌
			if (m_iSphereSkillTag != SKILL_STATICFIELD)
				m_pTransCom->Chase_Target(&vPlayerPos, 0.f, fTimeDelta);
			m_STATE = ZOMBIE_ATTACK;
			
			if (m_AnimationTime >= 1.f)
			{
				CLayer* pLayer = Engine::Get_Layer(STAGE_CHARACTER);
				CCubePlayer* pPlayer = dynamic_cast<CCubePlayer*>(pLayer->Get_GameObject(L"PLAYER"));
				pPlayer->KnuckDown(10.f, 10.f);
			}

		}
		else if (m_pCollision->Sphere_Collision(this->m_pSearchRange_TransCom, m_pPlayerTransCom, vPlayerScale.x, vSearchScale.x)/* && (m_STATE != FIREMAN_ATTACK)*/)
		{
			// 탐지충돌
			if (m_iSphereSkillTag != SKILL_STATICFIELD)
				m_pTransCom->Chase_Target(&vPlayerPos, 3.f, fTimeDelta);
			m_STATE = ZOMBIE_WALK;
		}
		else if (m_STATE != FIREMAN_ATTACK)
		{
			m_STATE = ZOMBIE_IDLE;
		}

		Look_Direction();

		_vec3 vMonsterPos;
		m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);
		m_pHitBoxTransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
		m_pSphereTransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);

		m_pSearchRange_TransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
		m_pAttackRange_TransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
	}
	return 0;
}

void CZombie::LateUpdate_Object(void)
{
	
	if (!m_bFirst)
	{
		if (m_STATE == ZOMBIE_WALK)
		{
			Walk_Animation_Run();
			Run_Animation(5.f);
		}
		else if (m_STATE == ZOMBIE_IDLE)
		{
			Idle_Animation_Run();
			Run_Animation(5.f);
		}
		else if (m_STATE == ZOMBIE_ATTACK)
		{
			Attack_Animation_Run();
			Run_Animation(5.f);
		}
		else if (m_STATE == ZOMBIE_DEAD)
		{
			Dead_Animation_Run();
			Run_Animation(5.f);
		}
	}

	CMonster::LateUpdate_Object();
}

void CZombie::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransUICom->Get_WorldMatrixPointer());
	m_pTextureUICom->Set_Texture(0);
	m_pBufferUICom->Resize_Buffer(m_tAbility->fCurrentHp / m_tAbility->fMaxHp);
	m_pBufferUICom->Render_Buffer();
}

void CZombie::Run_Animation(const _float & AnimationSpeed)
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

void CZombie::Look_Direction(void)
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
			Transform->Set_Angle(&_vec3(yaw, vAngle.y, roll));
		}
	}
}

void CZombie::Walk_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_WALK == ZOMBIEWALK_START)
			m_WALK = ZOMBIEWALK_1;
		else if (m_WALK == ZOMBIEWALK_1)
			m_WALK = ZOMBIEWALK_2;
		else if (m_WALK == ZOMBIEWALK_2)
			m_WALK = ZOMBIEWALK_1;

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(2 + m_WALK);
	}
}

void CZombie::Idle_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_IDLE == ZOMBIEIDLE_1)
			m_IDLE = ZOMBIEIDLE_2;
		else if (m_IDLE == ZOMBIEIDLE_2)
			m_IDLE = ZOMBIEIDLE_1;

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(0 + m_IDLE);
	}
}

void CZombie::Attack_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(5 + m_ATTACK);
	}
}

void CZombie::Dead_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(6 + m_DEAD);
	}
}

HRESULT CZombie::Add_Component(void)
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

	// FOR HITBOX
	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Engine::Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pComponent });

	pComponent = m_pHitBoxTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"HitBox_Transform", pComponent });

	// FOR UI
	pComponent = m_pTransUICom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransUICom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformUICom", pComponent });

	pComponent = m_pBufferUICom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_MONTER_HP_COMP));
	NULL_CHECK_RETURN(m_pBufferUICom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_MONTER_HP_COMP, pComponent });

	pComponent = m_pTextureUICom = dynamic_cast<CTexture*>(Clone_Proto(L"Monster_General_HP"));
	NULL_CHECK_RETURN(m_pTextureUICom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Monster_General_HP", pComponent });

	// For Sphere
	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pComponent });

	pComponent = m_pSphereTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Sphere_TransCom", pComponent });

	///////////////////////////탐지///////////////////////////////////////////////////////
	pComponent = m_pSearchRange_TransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"SearchRange", pComponent });

	pComponent = m_pAttackRange_TransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"AttackRange", pComponent });
	////////////////////////////////////////////////////////////////////////////////////////

	// 추가
	/*pComponent = m_pAnimationBox = dynamic_cast<CCubeCol*>(Clone_Proto(CUBECOL_COMP));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBECOL_COMP, pComponent });*/

	return S_OK;
}

HRESULT CZombie::Create_Item()
{
	CGameObject*		pGameObject = nullptr;
	_vec3 vItemPos;
	m_pTransCom->Get_Info(INFO_POS, &vItemPos);

	srand((unsigned int)time(NULL));
	_int iRand = rand() % 3;

	switch (iRand)
	{
	case 0:
		pGameObject = CHealthPotion::Create(m_pGraphicDev, vItemPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Get_Layer(STAGE_ITEM)->Add_GameList(pGameObject);
		break;

	case 1:
		pGameObject = CObtainBullet::Create(m_pGraphicDev, vItemPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Get_Layer(STAGE_ITEM)->Add_GameList(pGameObject);
		break;

	case 2:
		pGameObject = CObtainDefense::Create(m_pGraphicDev, vItemPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Get_Layer(STAGE_ITEM)->Add_GameList(pGameObject);
		break;
	}

	return S_OK;
}

void CZombie::Dead_Effect(void)
{
	//if (m_pDeadParticle == nullptr)
	//{
	//	m_pDeadParticle = dynamic_cast<CDeadParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"DeadParticle"));
	//}



	//_vec3 vPos;														//대쉬이펙트하려던것
	//_vec3 vDir;
	//m_pTransCom->Get_Info(INFO_POS, &vPos);
	//_vec3 min = { .0f ,.0f ,.0f };

	//for (_int i = -5; i < 5; i++)
	//{
	//	for (_int j = -5; j < 5; j++)
	//	{
	//		for (_int k = -5; k < 5; k++)
	//		{
	//			D3DXVec3Normalize(&min, &_vec3(i, j, k));

	//			dynamic_cast<CDeadParticle*>(m_pDeadParticle)->Set_PclePos(vPos);

	//			dynamic_cast<CDeadParticle*>(m_pDeadParticle)->Set_PcleDir(min);

	//			m_pDeadParticle->addParticle();
	//		}
	//	}
	//}

	if (m_pDeadParticle == nullptr)
		m_pDeadParticle = dynamic_cast<CDeadParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"DeadParticle"));
	
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	
	if (m_pDeadParticle != nullptr)
	{
		m_pDeadParticle->Set_PclePos(vPos);
		for (_int i = 0; i < 30; ++i)
		{
			m_pDeadParticle->addParticle();
		}
	}
}

HRESULT CZombie::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Zombie/ZOMBIE.dat",      // 파일의 경로와 이름	
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

void CZombie::Load_Animation(wstring FileName, _uint AnimationID)
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

CZombie * CZombie::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, _tchar* Name)
{
	CZombie *	pInstance = new CZombie(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, Name)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CZombie::Free(void)
{
	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		iter.second->Kill_Obj();
	}

	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	CMonster::Free();
	Safe_Delete<MONSTERABILITY*>(m_tAbility);
}
