#include "stdafx.h"
#include "..\Header\Supporter_Shotgun.h"
#include "TransAxisBox.h"
#include "PoolMgr.h"
#include "VerticalLine.h"

CSupporter_Shotgun::CSupporter_Shotgun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CSupporter(pGraphicDev)
{
}

CSupporter_Shotgun::~CSupporter_Shotgun()
{
}

HRESULT CSupporter_Shotgun::Ready_Object(const _vec3 & vPos, _tchar * Name)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_SupporterName = Name;

	m_STATE = SHOTGUNSUPPORT_IDLE;
	m_IDLE = SHOTGUNSUPPORT_IDLE_1;
	m_WALK = SHOTGUNSUPPORT_WALK_1;
	m_ATTACK = SHOTGUNSUPPORT_ATTACK_1;
	m_DROP = SPDROP_1;

	m_pTransform->Set_Scale(0.3f, 0.3f, 0.3f);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransform->Static_Update();

	_vec3 vAnimationPos;
	m_pTransform->Get_Info(INFO_POS, &vAnimationPos);

	m_pHitBoxTransform->Set_Scale(&_vec3(0.3f, 0.3f, 0.3f));
	m_pHitBoxTransform->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pHitBoxTransform->Static_Update();

	m_pSphereTransCom->Set_Scale(&_vec3(7.f, 7.f, 7.f));
	m_pSphereTransCom->Set_Pos(vAnimationPos.x, vAnimationPos.y, vAnimationPos.z);
	m_pSphereTransCom->Static_Update();

	return S_OK;
}
_int CSupporter_Shotgun::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if (m_pPlayerTransCom == nullptr)
		m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));

	if (m_bFirst)
	{
		m_bFirst = false;
		Engine::Get_Scene()->New_Layer(m_SupporterName);
		pMyLayer = Engine::Get_Layer(m_SupporterName);

		FAILED_CHECK_RETURN(Build(), -1);

		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_IDLE_1.dat", 0);
		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_IDLE_2.dat", 1);

		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_WALK_1.dat", 2);
		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_WALK_2.dat", 3);

		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_ATTACK_1.dat", 4);
		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_ATTACK_2.dat", 5);

		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_DROP.dat", 6);
		Load_Animation(L"../../Data/Supporter_Shotgun/SHOUGUN_DROP_COMPLETE.dat", 7);
	}

	_vec3 vPlayerScale;
	m_pPlayerTransCom->Get_Scale(&vPlayerScale);
	_vec3 vScale;
	m_pSphereTransCom->Get_Scale(&vScale);
	_vec3 vPlayerPos;
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vPosition;
	m_pTransform->Get_Info(INFO_POS, &vPosition);

	if (vPosition.y > 1.f)
	{
		m_pTransform->Move_Pos(&(_vec3(0.f, -1.f, 0.f) * 10.f * fTimeDelta));

		CVerticalLine* pVerticalLine = nullptr;
		_vec3 vPos;
		_vec3 vDir;
		m_pTransform->Get_Info(INFO_POS, &vPos);
		if (!pVerticalLine)
			pVerticalLine = dynamic_cast<CVerticalLine*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"VerticalLine"));
		pVerticalLine->Set_PcleDir(vDir);
		pVerticalLine->Set_PclePos(vPos);
		for (_int i = 0; i < 250; ++i)
		{
			pVerticalLine->addParticle();
		}
	}
	else if (vPosition.y < 2.f)
	{
		m_bSetCam = false;
	}

	if (vPosition.y > 1.f)
	{
		m_STATE = SHOTGUNSUPPORT_DROP;
		if (vPosition.y < 1.f)
		{
			_float fSound = 1.f;
			Engine::PlaySoundGun(L"SupporterDrop.wav", SOUND_GET, fSound);
		}
	}
	else if (m_bGetOrder)
	{
		if (!m_bOrdering)
		{
			CTerrainTex*   pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
			NULL_CHECK_RETURN(pTerrainBufferCom, 0);

			CTransform*      pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pTerrainTransformCom, 0);

			_vec3 vPos;
			vPos = m_pCalculatorCom->Peek_Target_Vector(g_hWnd, &_vec3(0.f, 0.f, 0.f), pTerrainBufferCom, pTerrainTransformCom);
			_vec3 vSetPos = _vec3(vPos.x, vPos.y + 0.5f, vPos.z);

			m_vOrderPos = vSetPos;
			m_bGetOrder = true;
			m_bOrdering = true;
		}

		_vec3 vPosition;
		m_pTransform->Get_Info(INFO_POS, &vPosition);

		_vec3 vOrderDir = m_vOrderPos - vPosition;

		if (m_STATE != SHOTGUNSUPPORT_ATTACK)
		{
			vOrderDir = m_vOrderPos - vPosition;
			m_pCollision->Wall_Collision_Check(this->m_pTransform, this->m_pHitBox, &vOrderDir);
			m_pTransform->Chase_Target_By_Direction(&vOrderDir, 5.f, fTimeDelta);
		}

		if ((fabs(m_vOrderPos.x - vPosition.x) < 1.f &&
			fabs(m_vOrderPos.y - vPosition.y) < 1.f &&
			fabs(m_vOrderPos.z - vPosition.z) < 1.f) || vOrderDir.x == 0 || vOrderDir.z == 0)
		{
			m_bGetOrder = false;
			m_bOrdering = false;
		}
		m_STATE = SHOTGUNSUPPORT_WALK;
	}
	else if (!m_pCollision->Sphere_Collision(this->m_pSphereTransCom, m_pPlayerTransCom, vPlayerScale.x, vScale.x) && (m_STATE != SHOTGUNSUPPORT_ATTACK))
	{
		_vec3 vDir = vPlayerPos - vPosition;

		m_pCollision->Wall_Collision_Check(this->m_pTransform, this->m_pHitBox, &vDir);
		m_pTransform->Chase_Target_By_Direction(&vDir, 5.f, fTimeDelta);
		m_STATE = SHOTGUNSUPPORT_WALK;
	}
	else // 아무 상태도 아니면 가만히있기
	{
		m_pTransform->Chase_Target(&vPlayerPos, 0.f, fTimeDelta);
		m_STATE = SHOTGUNSUPPORT_IDLE;
	}
	// 여기서 good

	if (vPosition.y < 1.f)
	{
		Find_Target();   //   맨 아래에 둘 것, 주변 적 탐색하여 공격하는 기능임
						 //   Look_Direction 지금 yaw만 적용시킨 상태

		Look_Direction();

		if (m_STATE != SHOTGUNSUPPORT_DROP)
			Set_On_Terrain();
	}

	CSupporter::Update_Object(fTimeDelta);

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	m_pHitBoxTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pSphereTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	if (m_STATE == SHOTGUNSUPPORT_ATTACK)
	{
		m_fShootingTime += fTimeDelta;
	}
	else
	{
		m_fShootingTime -= (fTimeDelta * 2.f);

		if (m_fShootingTime <= 0.f)
			m_fShootingTime = 0.f;
	}

	if (m_bShooting == true && m_fShootingTime >= 5.f)
	{
		m_bShooting = false;
	}
	else if (m_fShootingTime <= 0.f)
	{
		m_bShooting = true;
	}

	return 0;
}

void CSupporter_Shotgun::LateUpdate_Object(void)
{
	if (!m_bFirst)
	{
		if (m_STATE == UZISUPPORTER_DROP)
		{
			DROP_Animation_Run();
			Run_Animation(10.f);
		}
		else if (m_STATE == SHOTGUNSUPPORT_IDLE)
		{
			IDLE_Animation_Run();
			Run_Animation(10.f);
		}
		else if (m_STATE == SHOTGUNSUPPORT_WALK)
		{
			WALK_Animation_Run();
			Run_Animation(5.f);
		}
		else if (m_STATE == SHOTGUNSUPPORT_ATTACK)
		{
			ATTACK_Animation_Run();
			if (m_ATTACK == SHOTGUNSUPPORT_ATTACK_1)
				Run_Animation(0.5f);
			else
				Run_Animation(10.f);
		}
	}

	CSupporter::LateUpdate_Object();
}

void CSupporter_Shotgun::Render_Object(void)
{
	/*m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pHitBoxTransform->Get_WorldMatrixPointer());
	m_pHitBox->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pSphereTransCom->Get_WorldMatrixPointer());
	m_pSphereBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
}

HRESULT CSupporter_Shotgun::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pHitBoxTransform = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"m_pHitBoxTransform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Clone_Proto(COLLISION_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COLLISION_COMP, pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pComponent });

	pComponent = m_pSphereTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"m_pSphereTransCom", pComponent });

	return S_OK;
}

void CSupporter_Shotgun::Look_Direction(void)
{
	_matrix matWorld;
	m_pTransform->Get_WorldMatrix(&matWorld);

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
			Transform->Set_Angle(&_vec3(yaw, 0.f, 0.f));
		}
	}
}

void CSupporter_Shotgun::Find_Target(void)
{
	CLayer*	pMonsterLayer = Engine::Get_Layer(STAGE_MONSTER);

	_vec3 vGunPos_Left, vGunPos_Right;
	for (auto& iter : pMyLayer->Get_GamePair())
	{
		if (0 == _tcscmp(iter.first, L"SHOTGUN_3"))
		{
			_matrix matFinal;
			dynamic_cast<CTransAxisBox*>(iter.second)->Get_Final(&matFinal);
			vGunPos_Left = { matFinal.m[3][0], matFinal.m[3][1], matFinal.m[3][2] };
		}
	}

	_vec3 vScale;
	m_pSphereTransCom->Get_Scale(&vScale);

	for (auto& iter : pMonsterLayer->Get_GameList())
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));

		_vec3 vMonsterScale;
		pTransform->Get_Scale(&vMonsterScale);
		_vec3 vMonsterPos;
		pTransform->Get_Info(INFO_POS, &vMonsterPos);

		if (m_pCollision->Sphere_Collision(this->m_pSphereTransCom, pTransform, vMonsterScale.x, vScale.x) && m_bShooting)
		{
			_vec3 vDir_Left = vMonsterPos - vGunPos_Left;
			D3DXVec3Normalize(&vDir_Left, &vDir_Left);

			m_pTransform->Chase_Target(&vMonsterPos, 0.f, m_fTimeDelta);
			m_STATE = SHOTGUNSUPPORT_ATTACK;

			if (m_AnimationTime >= 1.f)
			{
				CPoolMgr::GetInstance()->Reuse_PlayerBullet(m_pGraphicDev, &vGunPos_Left, &vDir_Left, 5, 10.f);
			}
		}
	}
}

HRESULT CSupporter_Shotgun::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Supporter_Shotgun/PLAYER_SHOTGUN.dat",      // 파일의 경로와 이름	
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
		m_pTransform->Get_Info(INFO_POS, &vPlayerPos);

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

void CSupporter_Shotgun::Load_Animation(wstring FileName, _uint AnimationID)
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

void CSupporter_Shotgun::Run_Animation(const _float & AnimationSpeed)
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
		m_pTransform->Get_Info(INFO_POS, &vPlayerPos);

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
			BoxTransform->Set_Pos(vTransLerp.x + vPlayerPos.x, vTransLerp.y + vPlayerPos.y, vTransLerp.z + vPlayerPos.z);
		else
			BoxTransform->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
	}
}

void CSupporter_Shotgun::IDLE_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_IDLE == SHOTGUNSUPPORT_IDLE_1)
			m_IDLE = SHOTGUNSUPPORT_IDLE_2;
		else if (m_IDLE == SHOTGUNSUPPORT_IDLE_2)
			m_IDLE = SHOTGUNSUPPORT_IDLE_1;

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		if (m_IDLE == SHOTGUNSUPPORT_IDLE_1)
			Qtan->Change_Animation(0);
		else if (m_IDLE == SHOTGUNSUPPORT_IDLE_2)
			Qtan->Change_Animation(1);
	}
}

void CSupporter_Shotgun::WALK_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_WALK == SHOTGUNSUPPORT_WALK_1)
			m_WALK = SHOTGUNSUPPORT_WALK_2;
		else if (m_WALK == SHOTGUNSUPPORT_WALK_2)
			m_WALK = SHOTGUNSUPPORT_WALK_1;

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		if (m_WALK == SHOTGUNSUPPORT_WALK_1)
			Qtan->Change_Animation(2);
		else if (m_WALK == SHOTGUNSUPPORT_WALK_2)
			Qtan->Change_Animation(3);
	}
}

void CSupporter_Shotgun::ATTACK_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (m_ATTACK == SHOTGUNSUPPORT_ATTACK_1)
			m_ATTACK = SHOTGUNSUPPORT_ATTACK_2;
		else if (m_ATTACK == SHOTGUNSUPPORT_ATTACK_2)
			m_ATTACK = SHOTGUNSUPPORT_ATTACK_1;

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		if (m_ATTACK == SHOTGUNSUPPORT_ATTACK_1)
			Qtan->Change_Animation(4);
		else if (m_ATTACK == SHOTGUNSUPPORT_ATTACK_2)
			Qtan->Change_Animation(5);
	}
}

void CSupporter_Shotgun::DROP_Animation_Run(void)
{
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	_vec3 vPosition;
	m_pTransform->Get_Info(INFO_POS, &vPosition);

	if (m_AnimationTime >= 1.f)
	{
		for (auto& iter : ListBox)
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		if (vPosition.y >= 2.f)
			m_DROP = SPDROP_1;
		else
			m_DROP = SPDROP_2;

		m_AnimationTime = 0.f;
	}

	for (auto& iter : ListBox)	// 애니메이션 변경
	{
		CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
		Qtan->Change_Animation(6 + m_DROP);

	}
}

CSupporter_Shotgun * CSupporter_Shotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, _tchar * Name)
{
	CSupporter_Shotgun* pInstance = new CSupporter_Shotgun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, Name)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSupporter_Shotgun::Free(void)
{
	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		iter.second->Kill_Obj();
	}

	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	CSupporter::Free();
}
