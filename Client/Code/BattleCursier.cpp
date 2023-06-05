#include "stdafx.h"
#include "..\Header\BattleCursier.h"
#include "TransAxisBox.h"

CBattleCursier::CBattleCursier(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_vDirection(0, 0, 0), m_vAngle(0, 0, 0)
{
}

CBattleCursier::~CBattleCursier()
{
}

HRESULT CBattleCursier::Ready_Object(const _vec3 & vPos, const _vec3 & vDir, _tchar * Name)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_RocketName = Name;

	m_vDirection = vDir;

	m_pTransform->Set_Scale(1.f, 1.f, 1.f);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransform->Static_Update();

	m_fSpeed = 1.f;

	_vec3 vShuffle;
	m_ShufflePos.reserve(24);
	for (float i = 10.f; i <= 110.f; i += 20.f)
	{
		vShuffle = { i, 20.f, -50.f };
		m_ShufflePos.push_back(vShuffle);

		vShuffle = { -50.f, 25.f, i };
		m_ShufflePos.push_back(vShuffle);

		vShuffle = { i, 30.f, 170.f };
		m_ShufflePos.push_back(vShuffle);

		vShuffle = { 170.f, 35.f, i };
		m_ShufflePos.push_back(vShuffle);
	}

	m_ShuffleSpeed.reserve(10);
	for (_uint i = 5; i < 15; ++i)
	{
		m_ShuffleSpeed.push_back(i);
	}

	return S_OK;
}

_int CBattleCursier::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if (m_bFirst)
	{
		m_bFirst = false;
		Engine::Get_Scene()->New_Layer(m_RocketName);
		pMyLayer = Engine::Get_Layer(m_RocketName);

		FAILED_CHECK_RETURN(Build(), -1);
	}

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	if (vPos.x > 170.f || vPos.x < -50.f || vPos.z > 170.f || vPos.z < -50.f)
	{
		Random();
	}

	m_pTransform->Set_Angle(&m_vAngle);

	Look_Direction();

	m_pTransform->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));

	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		CTransform*	BoxTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
			BoxTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}

	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CBattleCursier::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CBattleCursier::Render_Object(void)
{
}

HRESULT CBattleCursier::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	return S_OK;
}

void CBattleCursier::Random(void)
{
	random_shuffle(m_ShufflePos.begin(), m_ShufflePos.end());
	random_shuffle(m_ShuffleSpeed.begin(), m_ShuffleSpeed.end());

	_vec3 vAngle, vDir;

	/*
	좌>우	1	0	0	90
	우>좌	-1	0	0	270
	뒤>앞	0	0	1	0
	앞>뒤	0	0	-1	180
	*/

	if (m_ShufflePos.front().z == -50.f)	//	뒤>앞
	{
		vAngle = { 0.f, D3DXToRadian(0.f) ,0.f };
		vDir = { 0.f, 0.f, 1.f };
	}
	else if (m_ShufflePos.front().x == -50.f)	//	좌>우
	{
		vAngle = { 0.f, D3DXToRadian(90.f) ,0.f };
		vDir = { 1.f, 0.f, 0.f };
	}
	else if (m_ShufflePos.front().z == 170.f)	//	앞>뒤
	{
		vAngle = { 0.f, D3DXToRadian(180.f) ,0.f };
		vDir = { 0.f, 0.f, -1.f };
	}
	else if (m_ShufflePos.front().x == 170.f)	//	우>좌
	{
		vAngle = { 0.f, D3DXToRadian(270.f) ,0.f };
		vDir = { -1.f, 0.f, 0.f };
	}

	Replace(m_ShufflePos.front(), vAngle, vDir);

	Set_Speed(_float(m_ShuffleSpeed.front()));
	
}

HRESULT CBattleCursier::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Rocket/BATTLECRUISER.dat",      // 파일의 경로와 이름	
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

void CBattleCursier::Look_Direction(void)
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

CBattleCursier * CBattleCursier::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, const _vec3 & vDir, _tchar * Name)
{
	CBattleCursier* pInstance = new CBattleCursier(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, Name)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBattleCursier::Free(void)
{
	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		iter.second->Kill_Obj();
	}

	for (auto iter : m_TcharList)
	{
		Safe_Delete_Array(iter);
	}

	CGameObject::Free();
}
