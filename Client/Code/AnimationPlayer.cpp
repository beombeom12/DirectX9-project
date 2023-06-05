#include "stdafx.h"
#include "..\Header\AnimationPlayer.h"

#include "TransAxisBox.h"

USING(Engine)

CAnimationPlayer::CAnimationPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CAnimationPlayer::~CAnimationPlayer()
{
}

HRESULT CAnimationPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pRotationTrans->Set_Pos(10.f, 0.6f, 10.f);
	m_pRotationTrans->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pRotationTrans->Static_Update();

	_vec3 vPos;
	m_pRotationTrans->Get_Info(INFO_POS, &vPos);

	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransform->Set_Scale(1.f, 1.f, 1.f);
	m_pTransform->Static_Update();

	m_WALK = PLAYERWALK_START;
	m_STOP = PLAYERSTOP_END;

	return S_OK;
}

_int CAnimationPlayer::Update_Object(const _float & fTimeDelta)
{
	if (m_bFirst)	//	1회만 모델을 불러오기 위함
	{
		m_bFirst = false;
		FAILED_CHECK_RETURN(Build(), E_FAIL);
	}

	m_fTimeDelta = fTimeDelta;

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CAnimationPlayer::LateUpdate_Object(void)
{
	Key_Input();
	Look_Direction();

	_vec3 vPos;
	m_pRotationTrans->Get_Info(INFO_POS, &vPos);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

	CGameObject::LateUpdate_Object();
}

void CAnimationPlayer::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DPMISCCAPS_CULLNONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pHitbox->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRotationTrans->Get_WorldMatrixPointer());
	m_pCubeCol->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DPMISCCAPS_CULLCCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CAnimationPlayer::Key_Input(void)
{
	if (Get_DIKeyState(DIK_UP) & 0x8000)
	{
		// 이동은 히트박스가 진행
		m_STOP = PLAYERSTOP_1;
		m_pRotationTrans->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pRotationTrans->Move_Pos(&(vDir * 5.f * m_fTimeDelta));
		// 애니메이션 실행 (용도에 따라 함수 추가로 생성)
		Walk_Animation_Run();
		m_STATE = PLAYER_WALK;
	}
	else if (Get_DIKeyState(DIK_DOWN) & 0x8000)
	{
		m_STOP = PLAYERSTOP_1;
		m_pRotationTrans->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pRotationTrans->Move_Pos(&(vDir * -5.f * m_fTimeDelta));
		Walk_Animation_Run();
		m_STATE = PLAYER_WALK;
	}
	else
	{
		m_WALK = PLAYERWALK_START;
		Stop_Animation_Run();
		m_STATE = PLAYER_STOP;
	}
}

HRESULT CAnimationPlayer::Build(void)	//	모델 불러오는 함수
{
	//HANDLE      hFile = CreateFile(L"../../Data/PLAYER_DEFAULT.dat",      // 파일의 경로와 이름
	HANDLE      hFile = CreateFile(L"../../Data/Player/PLAYER_REAL.dat",      // 파일의 경로와 이름	
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
	CLayer* pMyLayer = nullptr;

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
		pMyLayer = Engine::Get_Layer(STAGE_PLAYER);				//	각 애니메이션 오브젝트마다 별개의 레이어 사용을 권장 (플레이어/ 몬스터 등)
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

		_vec3		vHitPos;
		m_pTransform->Get_Info(INFO_POS, &vHitPos);

		_vec3		vPlayerPos;
		m_pRotationTrans->Get_Info(INFO_POS, &vPlayerPos);


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
	//MSG_BOX("Load_Complete");

	return S_OK;
}

void CAnimationPlayer::Look_Direction(void)
{
	_long MoveX = Get_DIMouseMove(DIMS_X);
	_long MoveY = Get_DIMouseMove(DIMS_Y);
	_long MoveZ = Get_DIMouseMove(DIMS_Z);

	m_pRotationTrans->Rotation(ROT_Y, D3DXToRadian(MoveX / 10.f));
	//m_pRotationTrans->Rotation(ROT_X, D3DXToRadian(MoveY / 10.f));

	CLayer* pLayer = Engine::Get_Layer(STAGE_PLAYER);
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pLayer->Get_GamePairPtr());

	for (auto& iter : ListBox)
	{
		if (0 == _tcscmp(iter.first, L"A_ROOT"))
		{
			_vec3 vAngle;
			CTransform* Transform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));
			Transform->Get_Angle(&vAngle);

			_vec3 vCubeAngle;
			m_pRotationTrans->Get_Angle(&vCubeAngle);

			//	yaw pitch roll 순서!!
			_vec3 vCubeFinal(vCubeAngle.y, vCubeAngle.x, vCubeAngle.z);
			Transform->Set_Angle(&vCubeFinal);
		}
	}
}

HRESULT CAnimationPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pHitbox = dynamic_cast<CHitBox*>(Engine::Clone_Proto(L"Proto_HitboxCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HitboxCom", pComponent });

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculator = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pCollision = dynamic_cast<CCollision*>(Engine::Clone_Proto(L"Proto_CollisionCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CollisionCom", pComponent });

	pComponent = m_pCubeCol = dynamic_cast<CCubeCol*>(Engine::Clone_Proto(CUBECOL_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBECOL_COMP, pComponent });

	pComponent = m_pRotationTrans = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"m_pRotationTrans", pComponent });

	return S_OK;
}

void CAnimationPlayer::Sync_Animation(PLAYERSTATEID eID)	//	이거 지금 안씁니다
{
	switch (eID)
	{
	case PLAYER_WALK:
		Load_Animation(L"../../Data/SMALL_WALK_1.dat");
		break;
	case PLAYER_STOP:
		Load_Animation(L"../../Data/ANIMATION_RUN_2.dat");
		break;
	}
}

void CAnimationPlayer::Load_Animation(wstring FileName)		//	실제 애니메이션을 불러오는 부분
{
	CLayer* MyLayer = Engine::Get_Layer(STAGE_PLAYER);

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
		auto	MapFindIter = find_if(MyLayer->Get_GamePairPtr()->begin(), MyLayer->Get_GamePairPtr()->end(), CTag_Finder(iter));

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
			Quaternion->Add_World(&matAnimation);
		}

		iSize--;

		if (0 == dwByte)
			break;

		if (0 == iSize)
			break;
	}

	CloseHandle(hFile);
}

void CAnimationPlayer::Run_Animation(const _float& AnimationSpeed)	//	불러와진 애니메이션을 동작시키는 부분
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_PLAYER);
	list<pair<const _tchar*, CGameObject*>> ListBox = *(pLayer->Get_GamePairPtr());
	//map<const _tchar*, CGameObject*> mapBox = pLayer->Get_GameObjectMap();

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

		/*D3DXVec3Lerp(&vScaleLerp, &vScale1, &vScale2, fTimeDelta - static_cast<_float>(iCount));
		D3DXQuaternionSlerp(&qSLerp, &q1, &q2, fTimeDelta - static_cast<_float>(iCount));
		D3DXVec3Lerp(&vTransLerp, &vTrans1, &vTrans2, fTimeDelta - static_cast<_float>(iCount));*/

		_float pitch, yaw, roll;

		FLOAT sqw = qSLerp.w * qSLerp.w;
		FLOAT sqx = qSLerp.x * qSLerp.x;
		FLOAT sqy = qSLerp.y * qSLerp.y;
		FLOAT sqz = qSLerp.z * qSLerp.z;

		pitch = asinf(2.f * (qSLerp.w * qSLerp.x - qSLerp.y * qSLerp.z));
		yaw =  atan2f(2.0f * (qSLerp.x * qSLerp.z + qSLerp.w * qSLerp.y), (-sqx - sqy + sqz + sqw));
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
		m_pRotationTrans->Get_Info(INFO_POS, &vPlayerPos);

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
			BoxTransform->Set_Pos(vTransLerp.x + vPlayerPos.x, vTransLerp.y + vPlayerPos.y, vTransLerp.z + vPlayerPos.z);
		else
			BoxTransform->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
	}
}

void CAnimationPlayer::Walk_Animation_Run(void)	//	걷기 애니메이션 실행
{
	if (m_STATE == PLAYER_STOP)
	{
		CLayer* MyLayer = Engine::Get_Layer(STAGE_PLAYER);
		list<pair<const _tchar*, CGameObject*>> ListBox = *(MyLayer->Get_GamePairPtr());

		for (auto& iter : ListBox)	//	모든 쿼터니언(애니메이션) 객체들에 대해 기존의 애니메이션 값을 전부 지워줌
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}
	}

	if (m_AnimationTime >= 1.f)	//	시간값이 1보다 크거나 같으면 애니메이션을 변경함, 각 캡처마다 1초를 원칙으로 함
	{
		CLayer* MyLayer = Engine::Get_Layer(STAGE_PLAYER);
		list<pair<const _tchar*, CGameObject*>> ListBox = *(MyLayer->Get_GamePairPtr());
		//map<const _tchar*, CGameObject*> mapBox = *(MyLayer->Get_GameObjectMapPtr());

		for (auto& iter : ListBox)	//	모든 쿼터니언(애니메이션) 객체들에 대해 기존의 애니메이션 값을 전부 지워줌
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		// 현재 걷기는 두개의 캡쳐밖에 없으므로 bool타입을 활용했지만, 3개 이상의 캡쳐가 필요한 애니메이션의 경우 열거체 만들어서 활용
		if (m_WALK == PLAYERWALK_START)
			m_WALK = PLAYERWALK_1;
		else if (m_WALK == PLAYERWALK_1)
			m_WALK = PLAYERWALK_2;
		else if (m_WALK == PLAYERWALK_2)
			m_WALK = PLAYERWALK_1;

		m_AnimationTime = 0.f;
	}

	//	bool/열거체 값에 따라 어떤 애니메이션을 불러올 것인지 결정
	if (m_WALK == PLAYERWALK_START)
	{
		Load_Animation(L"../../Data/Player/Animation_Walk_Start.dat");
		Run_Animation(2.f);	//	애니메이션의 동작 시간, 캡처는 1초를 원칙으로 하나 해당 부분에서 입력되는 숫자의 값만큼 애니메이션이 느려짐
	}
	else if (m_WALK == PLAYERWALK_1)
	{
		Load_Animation(L"../../Data/Player/Animation_Walk_1.dat");
		Run_Animation(4.f);
	}
	else if (m_WALK == PLAYERWALK_2)
	{
		Load_Animation(L"../../Data/Player/Animation_Walk_2.dat");
		Run_Animation(4.f);
	}
}

void CAnimationPlayer::Stop_Animation_Run(void)
{
	if (m_STATE == PLAYER_WALK)
	{
		CLayer* MyLayer = Engine::Get_Layer(STAGE_PLAYER);
		list<pair<const _tchar*, CGameObject*>> ListBox = *(MyLayer->Get_GamePairPtr());

		for (auto& iter : ListBox)	//	모든 쿼터니언(애니메이션) 객체들에 대해 기존의 애니메이션 값을 전부 지워줌
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}
	}

	if (m_AnimationTime >= 1.f)	//	시간값이 1보다 크거나 같으면 애니메이션을 변경함, 각 캡처마다 1초를 원칙으로 함
	{
		CLayer* MyLayer = Engine::Get_Layer(STAGE_PLAYER);
		list<pair<const _tchar*, CGameObject*>> ListBox = *(MyLayer->Get_GamePairPtr());
		//map<const _tchar*, CGameObject*> mapBox = *(MyLayer->Get_GameObjectMapPtr());

		for (auto& iter : ListBox)	//	모든 쿼터니언(애니메이션) 객체들에 대해 기존의 애니메이션 값을 전부 지워줌
		{
			CQuarternion* Qtan = dynamic_cast<CQuarternion*>(iter.second->Get_Component(L"Proto_QuaternionCom", ID_STATIC));
			Qtan->Delete_WorldVector();
		}

		// 현재 걷기는 두개의 캡쳐밖에 없으므로 bool타입을 활용했지만, 3개 이상의 캡쳐가 필요한 애니메이션의 경우 열거체 만들어서 활용
		/*if (m_STOP == PLAYERSTOP_1)
			m_STOP = PLAYERSTOP_2;
		else if (m_STOP == PLAYERSTOP_2)
			m_STOP = PLAYERSTOP_END;*/

		if (m_WALK == PLAYERWALK_1)
			m_STOP = PLAYERSTOP_1;
		else if (m_WALK == PLAYERWALK_2)
			m_STOP = PLAYERSTOP_2;
		else
			m_STOP = PLAYERSTOP_END;

		m_AnimationTime = 0.f;
	}

	//	bool/열거체 값에 따라 어떤 애니메이션을 불러올 것인지 결정
	if (m_STOP == PLAYERSTOP_1)
	{
		Load_Animation(L"../../Data/Player/Animation_Walk_1_Stop.dat");
		Run_Animation(2.f);	//	애니메이션의 동작 시간, 캡처는 1초를 원칙으로 하나 해당 부분에서 입력되는 숫자의 값만큼 애니메이션이 느려짐
	}
	else if (m_STOP == PLAYERSTOP_2)
	{
		Load_Animation(L"../../Data/Player/Animation_Walk_2_Stop.dat");
		Run_Animation(2.f);
	}
}

CAnimationPlayer * CAnimationPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimationPlayer* pInstance = new CAnimationPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("AnimationPalyer Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAnimationPlayer::Free(void)
{
	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	for (auto iter : m_CharList)
	{
		Safe_Delete_Array(iter);
	}

	CGameObject::Free();
}
