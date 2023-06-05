#include "stdafx.h"
#include "..\Header\KrakenLeg.h"
#include "MonsterUI.h"
#include "ComboUI.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "TransAxisBox.h"

CKrakenLeg::CKrakenLeg(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CKrakenLeg::~CKrakenLeg()
{
}

HRESULT CKrakenLeg::Ready_Object(const _vec3 & vPos, _tchar * Name)
{
	m_MonsterName = Name;

	m_tAbility = new KRAKENABILITY;
	m_tAbility->fMaxHp = 500.f;
	m_tAbility->fCurrentHp = m_tAbility->fMaxHp;
	m_tAbility->fDamage = 20.f;
	m_tAbility->strObjTag = m_MonsterName;

	m_STATE = KRAKEN_APPEAR;
	m_IDLE = KRAKENIDLE_1;
	m_APPEAR = KRAKENAPPEAR_1;
	m_SMASH = KRAKENSMASH_1;
	m_INKSHOT = KRAKENINKSHOT_1;
	m_ROLLING = KRAKENROLLING_1;
	m_LURKER = KRAKENLURKER_1;
	m_PATTERN = KRAKEN_SKILL_END;

	m_LEGIDLE = LEGIDLE_6;
	m_LEGSHAKE = LEGSHAKE_1;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(&_vec3(1.f, 20.f, 1.f));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_vOriginPos = vPos;

	m_pTransUICom->Set_Scale(1.f, 1.f, 1.5f);

	_vec3 vAnimationPos;
	m_pTransCom->Get_Info(INFO_POS, &vAnimationPos);

	m_pHitBoxTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pHitBoxTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pHitBoxTransCom->Static_Update();

	m_pSphereTransCom->Set_Scale(&_vec3(20.f, 20.f, 20.f));
	m_pSphereTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pSphereTransCom->Static_Update();

	return S_OK;
}

_int CKrakenLeg::Update_Object(const _float & fTimeDelta)
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

		for (float i = -10.f; i < 10.f; ++i)
		{
			m_ShufflePos.push_back(i);
		}

		m_vPattern.push_back(KRAKEN_SKILL_SMASH);
		//m_vPattern.push_back(KRAKEN_SKILL_INKSHOT);
		//m_vPattern.push_back(KRAKEN_SKILL_ROLLING);
		m_vPattern.push_back(KRAKEN_SKILL_LURKER);
		//m_vPattern.push_back(KRAKEN_SKILL_5);

		Engine::Get_Scene()->New_Layer(m_MonsterName);
		pMyLayer = Engine::Get_Layer(m_MonsterName);
		FAILED_CHECK_RETURN(Build(), -1);

		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE1.dat", 0);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE2.dat", 1);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE3.dat", 2);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE4.dat", 3);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE5.dat", 4);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE6.dat", 5);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_IDLE7.dat", 6);

		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH1.dat", 7);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH2.dat", 8);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH3.dat", 9);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH4.dat", 10);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH5.dat", 11);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH6.dat", 12);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH7.dat", 13);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH8.dat", 14);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH9.dat", 15);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH10.dat", 16);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SMASH11.dat", 17);

		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SHAKE1.dat", 18);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SHAKE2.dat", 19);
		Load_Animation(L"../../Data/KrakenLeg/KRAKEN_SHAKE3.dat", 20);
	}

	Update_Pattern(fTimeDelta);

	_vec3 vMainBodyPos, vPos, vPlayerPos;
	vMainBodyPos = { 65.f, 0.f, 65.f };
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vDistance = vPlayerPos - vPos;
	_float fDistance = D3DXVec3Length(&vDistance);

	_vec3 vNewDir;

	if (m_PATTERN == KRAKEN_SKILL_SMASH)
	{
		if ((m_SMASH == KRAKENSMASH_1 || m_SMASH == KRAKENSMASH_2)
			&& fabs(fDistance) <= 50.f)
		{
			vDir = vPlayerPos - vPos;
		}

		vNewDir = vPos - vMainBodyPos;

		if (/*m_SMASH == KRAKENSMASH_8 || */m_SMASH == KRAKENSMASH_9 ||
			m_SMASH == KRAKENSMASH_10 || m_SMASH == KRAKENSMASH_11 ||
			m_SMASH == KRAKENSMASH_12)
		{
			m_fLerpTime += (fTimeDelta / 500.f);
			if (m_fLerpTime >= 1.f)
				m_fLerpTime = 1.f;
			D3DXVec3Lerp(&vDir, &vDir, &vNewDir, m_fLerpTime);
		}
	}
	else
	{
		vNewDir = vPos - vMainBodyPos;
		vDir = vNewDir;
		m_fLerpTime = 0.f;
	}

	m_pTransCom->Chase_Target_By_Direction(&vDir, 0.f, fTimeDelta);
	Look_Direction();

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	m_pTransUICom->Billboard_Transform(fTimeDelta);
	Hit_Check(fTimeDelta);

	_vec3 vMonsterPos;
	m_pTransCom->Get_Info(INFO_POS, &vMonsterPos);
	m_pHitBoxTransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
	m_pSphereTransCom->Set_Pos(vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
	m_pTransCom->Get_Info(INFO_POS, &vUIPos);
	m_pTransUICom->Set_Pos(vUIPos.x, vUIPos.y + 2.4f, vUIPos.z);

	return 0;
}

void CKrakenLeg::Render_Object(void)
{
}

void CKrakenLeg::LateUpdate_Object(void)
{
	if (m_tAbility->fCurrentHp <= 0.f)
	{
		m_pMonsterUI->Off_Switch();
		this->Kill_Obj();
	}

	//애니메이션 관련해서 run animation 이랑 각자 상황에 맞는 애니메이션 넣어주면됨.
	if (!m_bFirst)
	{
		//IDLE_Animation_Run();
		////SMASH_Animation_Run();
		//Run_Animation(20.f);

		if (m_STATE == KRAKEN_APPEAR)
		{
			_vec3 vPos;
			m_pTransCom->Get_Info(INFO_POS, &vPos);

			if (vPos.y < 0.f)
			{
				SHAKE_Animation_Run();
				Run_Animation(15.f);
				m_pTransCom->Set_Pos(vPos.x, vPos.y + 0.1f, vPos.z);
				m_pTransCom->Static_Update();
			}
			else
			{
				m_STATE = KRAKEN_IDLE;
			}
		}
		else if (m_STATE == KRAKEN_IDLE)
		{
			IDLE_Animation_Run();
			Run_Animation(15.f);
		}
		else if (m_STATE == KRAKEN_ATTACK)
		{
			if (m_PATTERN == KRAKEN_SKILL_SMASH)
			{
				SMASH_Animation_Run();
				Run_Animation(3.f);
			}
			if (m_PATTERN == KRAKEN_SKILL_INKSHOT)
			{
		
			}
			if (m_PATTERN == KRAKEN_SKILL_ROLLING)
			{
			
			}
			if (m_PATTERN == KRAKEN_SKILL_LURKER)
			{
				if (m_LURKER == KRAKENLURKER_1)
				{
					IDLE_Animation_Run();
					Run_Animation(15.f);
				}
				else
				{
					SHAKE_Animation_Run();
					Run_Animation(15.f);
				}
			}
			if (m_PATTERN == KRAKEN_SKILL_5)
			{
			
			}
		}
	}

	CGameObject::LateUpdate_Object();
}

void CKrakenLeg::Look_Direction(void)
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

_int CKrakenLeg::Update_Pattern(_float fTimeDelta)
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

	if (m_STATE == KRAKEN_IDLE)
	{
		m_ReloadTimer += fTimeDelta;

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
		if (m_PATTERN == KRAKEN_SKILL_SMASH)
		{
			// 플레이어랑 가까운 촉수 내려찍기
		}
		if (m_PATTERN == KRAKEN_SKILL_INKSHOT)
		{
			// 전방향 먹물탄
		}
		if (m_PATTERN == KRAKEN_SKILL_ROLLING)
		{
			// 촉수 뻗고 한바퀴 돌기
		}
		if (m_PATTERN == KRAKEN_SKILL_LURKER)
		{
			// 럴커
			Lurker_Pattern();
		}
		if (m_PATTERN == KRAKEN_SKILL_5)
		{

		}
	}

	return 0;
}

void CKrakenLeg::Hit_Check(_float _deltaTime)
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
			m_pMonsterUI->Set_Name(m_tAbility->strObjTag);
			m_pMonsterUI->Set_Hp(m_tAbility->fCurrentHp);
			m_pMonsterUI->Set_MaxHp(m_tAbility->fMaxHp);
			m_pMonsterUI->On_Switch();
			if (pWeapon->Get_Shoot() == true)
			{
				m_tAbility->fCurrentHp -= pWeapon->Get_Ability()->fBulletAttack;
				m_pComboUI->On_Switch();
				m_pComboUI->ComboCntPlus();

				pWeapon->Set_Shoot(false);
			}

			if (m_tAbility->fCurrentHp <= 0.f)
			{
				m_tAbility->fCurrentHp = 0.f;
			}
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

HRESULT CKrakenLeg::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/KrakenLeg/KRAKEN_LEG.dat",      // 파일의 경로와 이름	
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

void CKrakenLeg::Load_Animation(wstring FileName, _uint AnimationID)
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

void CKrakenLeg::Run_Animation(const _float & AnimationSpeed)
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

void CKrakenLeg::Lurker_Pattern(void)
{
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	_vec3 vPlayerPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

	if ((m_LURKER == KRAKENLURKER_1))
	{
		m_pTransCom->Set_Pos(vPos.x, vPos.y - 0.5f, vPos.z);
		m_pTransCom->Static_Update();
	}
	else if (m_LURKER == KRAKENLURKER_2)
	{
		if (vPos.y < -90.f)
		{
			random_shuffle(m_ShufflePos.begin(), m_ShufflePos.end());
			_float fRandFront = m_ShufflePos.front();
			_float fRandBack = m_ShufflePos.back();
			m_pTransCom->Set_Pos(vPlayerPos.x + fRandFront, vPos.y + 2.f, vPlayerPos.z + fRandBack);
		}
		else
			m_pTransCom->Set_Pos(vPos.x, vPos.y + 2.f, vPos.z);

		m_pTransCom->Static_Update();
	}
	else if (m_LURKER == KRAKENLURKER_3)
	{
		m_fLurkerTime += m_fTimeDelta;

		if (m_fLurkerTime >= 3.f)
		{
			m_LURKER = KRAKENLURKER_4;
			m_fLurkerTime = 0.f;
		}
	}
	else if (m_LURKER == KRAKENLURKER_4)
	{
		m_pTransCom->Set_Pos(vPos.x, vPos.y - 1.f, vPos.z);
		m_pTransCom->Static_Update();
	}
	else if (m_LURKER == KRAKENLURKER_5)
	{
		m_pTransCom->Set_Pos(m_vOriginPos.x, vPos.y + 1.f, m_vOriginPos.z);
		m_pTransCom->Static_Update();
	}

	if (vPos.y < -200.f)
	{
		if (m_LURKER == KRAKENLURKER_1)
			m_LURKER = KRAKENLURKER_2;
		if (m_LURKER == KRAKENLURKER_4)
			m_LURKER = KRAKENLURKER_5;
	}
	if (vPos.y > 0.f && m_LURKER == KRAKENLURKER_2)
	{
		m_LURKER = KRAKENLURKER_3;
	}
	if (vPos.y > 0.f && m_LURKER == KRAKENLURKER_5)
	{
		m_STATE = KRAKEN_IDLE;
		m_LEGIDLE = LEGIDLE_6;
		m_LURKER = KRAKENLURKER_1;
	}
}

void CKrakenLeg::APPEAR(void)
{
}

void CKrakenLeg::IDLE_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_LEGIDLE == LEGIDLE_1)
			m_LEGIDLE = LEGIDLE_2;
		else if (m_LEGIDLE == LEGIDLE_2)
			m_LEGIDLE = LEGIDLE_3;
		else if (m_LEGIDLE == LEGIDLE_3)
			m_LEGIDLE = LEGIDLE_4;
		else if (m_LEGIDLE == LEGIDLE_4)
			m_LEGIDLE = LEGIDLE_5;
		else if (m_LEGIDLE == LEGIDLE_5)
			m_LEGIDLE = LEGIDLE_6;
		else if (m_LEGIDLE == LEGIDLE_6)
			m_LEGIDLE = LEGIDLE_7;
		else if (m_LEGIDLE == LEGIDLE_7)
			m_LEGIDLE = LEGIDLE_1;

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(0 + m_LEGIDLE);
	}
}

void CKrakenLeg::SMASH_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_SMASH == KRAKENSMASH_1)
			m_SMASH = KRAKENSMASH_2;
		else if(m_SMASH == KRAKENSMASH_2)
			m_SMASH = KRAKENSMASH_3;
		else if (m_SMASH == KRAKENSMASH_3)
			m_SMASH = KRAKENSMASH_4;
		else if (m_SMASH == KRAKENSMASH_4)
			m_SMASH = KRAKENSMASH_5;
		else if (m_SMASH == KRAKENSMASH_5)
			m_SMASH = KRAKENSMASH_6;
		else if (m_SMASH == KRAKENSMASH_6)
			m_SMASH = KRAKENSMASH_7;
		else if (m_SMASH == KRAKENSMASH_7)
			m_SMASH = KRAKENSMASH_8;
		else if (m_SMASH == KRAKENSMASH_8)
			m_SMASH = KRAKENSMASH_9;
		else if (m_SMASH == KRAKENSMASH_9)
			m_SMASH = KRAKENSMASH_10;
		else if (m_SMASH == KRAKENSMASH_10)
			m_SMASH = KRAKENSMASH_11;
		else if (m_SMASH == KRAKENSMASH_11)
		{
			m_STATE = KRAKEN_IDLE;
			m_SMASH = KRAKENSMASH_1;
			m_LEGIDLE = LEGIDLE_1;
			m_PATTERN = KRAKEN_SKILL_END;
		}

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(7 + m_SMASH);
	}
}

void CKrakenLeg::SHAKE_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_LEGSHAKE == LEGSHAKE_1)
			m_LEGSHAKE = LEGSHAKE_2;
		else if (m_LEGSHAKE == LEGSHAKE_2)
			m_LEGSHAKE = LEGSHAKE_3;
		else if (m_LEGSHAKE == LEGSHAKE_3)
			m_LEGSHAKE = LEGSHAKE_1;

		m_AnimationTime = 0.f;
	}
	for (auto& iter : ListBox)
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(18 + m_LEGSHAKE);
	}
}

HRESULT CKrakenLeg::Add_Component(void)
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

CKrakenLeg * CKrakenLeg::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, _tchar * Name)
{
	CKrakenLeg* pInstance = new CKrakenLeg(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, Name)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CKrakenLeg::Free(void)
{
	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	CGameObject::Free();
	Safe_Delete<KRAKENABILITY*>(m_tAbility);
}
