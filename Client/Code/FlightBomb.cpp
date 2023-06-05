#include "stdafx.h"
#include "..\Header\FlightBomb.h"
#include "TransAxisBox.h"
#include "Explosion.h"
#include "TargetPointEffect.h"
CFlightBomb::CFlightBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CFlightBomb::~CFlightBomb()
{
}

HRESULT CFlightBomb::Ready_Object(const _vec3 & vPos, _tchar* Name)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	_float fSound = .4f;
	Engine::PlaySoundGun(L"FlightBomb.wav", SOUND_EFFECT, fSound);
	m_vPosition = vPos;

	m_MissileName = Name;

	m_fSpeed = 10.f;

	m_pTransCom->Set_Pos(vPos.x, vPos.y -0.2f , vPos.z);

	m_pTransCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));

	m_pTransCom->Static_Update();

	m_vDirection = { 0.f, -1.f, 0.f };

	return S_OK;
}

_int CFlightBomb::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		_tchar* szName = new _tchar[256]{};
		wstring wName = L"Explosion_%d";

		wsprintfW(szName, wName.c_str(), g_iExplosion);
		m_TcharList.push_back(szName);

		g_iExplosion++;

		// 위치 바꿔줘야함
		CGameObject* pGameObject = CExplosion::Create(m_pGraphicDev, _vec3(vPos.x, 0.f, vPos.z), szName);
		NULL_CHECK_RETURN(pGameObject, -1);
		CLayer* pLayer = Get_Layer(STAGE_SKILL);
		FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), -1);

		return -1;
	}

	if (m_bFirst)
	{
		m_bFirst = false;
		Engine::Get_Scene()->New_Layer(m_MissileName);
		pMyLayer = Engine::Get_Layer(m_MissileName);

		FAILED_CHECK_RETURN(Build(), -1);
	}

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	Bombing(fTimeDelta);
	Effect();
	return _int();
}

void CFlightBomb::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	if (vPos.y <= 0.f)
	{
		this->Kill_Obj();
	}

	CGameObject::LateUpdate_Object();
}

void CFlightBomb::Render_Object(void)
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(1);
	m_pSphereTex->Render_Buffer();*/
}

HRESULT CFlightBomb::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	return S_OK;
}


void CFlightBomb::Bombing(const _float& fTimeDelta)
{
	m_pTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));

	list<pair<const _tchar*, CGameObject*>> Parts = *(pMyLayer->Get_GamePairPtr());

	for (auto& iter : Parts)
	{
		CTransform*	BoxTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));

		_vec3		vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
			BoxTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
}

void CFlightBomb::Effect()
{
	//타겟떨어지는모양

	CTargetPointEffect* m_pTargetPointEffect = nullptr;

	if (!m_pTargetPointEffect)
		m_pTargetPointEffect = dynamic_cast<CTargetPointEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TargetPointEffect"));

	_vec3 vPos;													//대쉬이펙트하려던것
	_vec3 vDir;
	_vec3 min = { -1.0f ,-1.0f ,-1.0f };
	_vec3 max = { 0.f ,1.0f , 0.f};

	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vPos.y = 0.5f;
	for (_float i = -5.f; i < 5.f; i++)
	{
		for (_float j = -5.f; j < 5.f; j++)
		{
			for (_float k = -5.f; k < 5.f; k++)
			{
				D3DXVec3Normalize(&min, &_vec3(i, j, k));

				dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PclePos(vPos + min*4.f);
				dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleDir(min);
				dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleMoveDir(max);

				m_pTargetPointEffect->addParticle();
			}
		}
	}	
}

HRESULT CFlightBomb::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Rocket/MISSILE.dat",      // 파일의 경로와 이름	
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

CFlightBomb * CFlightBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, _tchar* Name)
{
	CFlightBomb* pInstance = new CFlightBomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, Name)))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CFlightBomb::Free(void)
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
