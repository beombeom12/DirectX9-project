#include "stdafx.h"
#include "..\Header\Flight.h"

#include "TransAxisBox.h"
#include "FlightBulletParticle.h"
#include "PoolMgr.h"
#include "FlightCamera.h"
#include "FlightSpot.h"
#include "Supporter_Uzi.h"
#include "Supporter_Shotgun.h"
#include "Supporter_Sniper.h"

#include "VerticalLine.h"
#include "CubeParticle.h"
CFlight::CFlight(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CFlight::~CFlight()
{
}

HRESULT CFlight::Ready_Object(const _vec3 & vPos, const _vec3 & vDir, _tchar * Name)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_RocketName = Name;

	m_vDirection = vDir;

	m_bControl = false;
	m_bShuttle = false;
	m_bEnding = false;

	m_pTransform->Set_Scale(1.f, 1.f, 1.f);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransform->Static_Update();

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
	for (_uint i = 20; i < 30; ++i)
	{
		m_ShuffleSpeed.push_back(i);
	}

	return S_OK;
}

_int CFlight::Update_Object(const _float & fTimeDelta)
{
	m_fBulletTime += fTimeDelta;

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

	if (!m_pBulletParicle)
		m_pBulletParicle = dynamic_cast<CFlightBulletParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlightBulletParticle"));

	if (m_bShuttle == true)
	{
		_vec3 vPos;
		m_pTransform->Get_Info(INFO_POS, &vPos);

		_vec3 vDesination;
		vDesination.x = m_vDestination.x;
		vDesination.y = 30.f;
		vDesination.z = m_vDestination.z;

		m_pTransform->Chase_Target(&vDesination, 5.f, fTimeDelta);

		Look_Direction_Only_Y();

		_matrix matWorld;
		m_pTransform->Get_WorldMatrix(&matWorld);
		vAfterDir = { matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2] };

		if (fabs(vPos.x - vDesination.x) < 1.f && fabs(vPos.z - vDesination.z) < 1.f)
		{
			CLayer* pLayer = Get_Layer(STAGE_SUPPORTER);
			if (m_eSupporterID == SUPPORTER_UZI)
			{
				CGameObject* pGameObject = CSupporter_Uzi::Create(m_pGraphicDev, vDesination, L"SUPPORT_UZI");
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SUPPORT_UZI", pGameObject), E_FAIL);
				dynamic_cast<CSupporter_Uzi*>(pGameObject)->Set_setcam(true);
				m_bShuttle = false;			
			}
			else if (m_eSupporterID == SUPPORTER_SHOTGUN)
			{
				CGameObject* pGameObject = CSupporter_Shotgun::Create(m_pGraphicDev, vDesination, L"SUPPORT_SHOTGUN");
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SUPPORT_SHOTGUN", pGameObject), E_FAIL);
				dynamic_cast<CSupporter_Shotgun*>(pGameObject)->Set_setcam(true);
				m_bShuttle = false;
			}
			else if (m_eSupporterID == SUPPORTER_SNIPER)
			{
				CGameObject* pGameObject = CSupporter_Sniper::Create(m_pGraphicDev, vDesination, L"SUPPORT_SNIPER");
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SUPPORT_SNIPER", pGameObject), E_FAIL);
				dynamic_cast<CSupporter_Sniper*>(pGameObject)->Set_setcam(true);
				m_bShuttle = false;
			}
		}
	}

	if (m_bEnding == true)
	{
		CTransform* m_pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));

		_vec3 vPlayerPos;
		m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vFlightPos;
		m_pTransform->Get_Info(INFO_POS, &vFlightPos);

		_vec3 vDistance = vPlayerPos - vFlightPos;

		if (m_bLeaveMap)
		{
			if (Get_Scene()->Get_SceneId() == STAGE_SCENE)
			{
				m_pTransform->Chase_Target_By_Direction(&m_vLeave, 5.f, fTimeDelta);
				Look_Direction_Only_Y();
			}
			else if (Get_Scene()->Get_SceneId() == FINAL_SCENE)
			{
				m_pTransform->Chase_Target_By_Direction(&_vec3(0.f,0.f,1.f), 5.f, fTimeDelta);
				Look_Direction_Only_Y();
			}
		}
		else
		{
			if (!m_bDest)
			{
				m_vEndingDest.x = vPlayerPos.x;
				m_vEndingDest.y = 30.f;
				m_vEndingDest.z = vPlayerPos.z;

				m_vEndingDest2 = vPlayerPos;
				m_bDest = true;
			}
			
			if (fabs(vFlightPos.x - m_vEndingDest.x) < 1.f && fabs(vFlightPos.z - m_vEndingDest.z) < 1.f && !m_bRide)
			{
				m_fEndingFrame += fTimeDelta * 0.2f;
				
				_vec3 vTransLerp;
				D3DXVec3Lerp(&vTransLerp,&m_vEndingDest,&m_vEndingDest2, m_fEndingFrame);
				m_pTransform->Set_Pos(vTransLerp.x,vTransLerp.y,vTransLerp.z);
				m_pTransform->Static_Update();

				if(vFlightPos.y <= 1.f)
					m_bRide = true;
			}
			else if(fabs(vFlightPos.x - m_vEndingDest.x) < 1.f && fabs(vFlightPos.z - m_vEndingDest.z) < 1.f && m_bRide)
			{
				m_fEndingFrame -= fTimeDelta * 0.2f;
				_vec3 vTransLerp;
				D3DXVec3Lerp(&vTransLerp, &m_vEndingDest, &m_vEndingDest2, m_fEndingFrame);
				m_pTransform->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
				m_pTransform->Static_Update();

				if(vFlightPos.y >= 29.f)
					m_bLeaveMap = true;
			}
			else 
			{
				m_fEndingFrame = 0.f;
				m_pTransform->Chase_Target(&m_vEndingDest, 5.f, fTimeDelta);

				_matrix matWorld;
				m_pTransform->Get_WorldMatrix(&matWorld);
				m_vLeave = { matWorld.m[2][0], matWorld.m[2][1],matWorld.m[2][2] };
				Look_Direction_Only_Y();
			}
		}
	}

	if (m_bControl == true && static_cast<CFlightCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlightCamera"))->Get_Maincam())
	{
		Key_Input(fTimeDelta);
		Move(fTimeDelta);
		Look_Direction();
		m_pTransform->Quaternion_Transform();
		_float fSound = 1.f;
		PlaySoundW(L"flight.wav", SOUND_EFFECT, fSound);
	}
	else 
	{
		if (D3DXVec3Length(&vAfterDir) != 0.f)
		{
			D3DXVec3Normalize(&vAfterDir, &vAfterDir);
			m_pTransform->Move_Pos(&(vAfterDir * 5.f * fTimeDelta));
			m_pTransform->Static_Update();
		}
		else 
		{
			if (m_bEnding == false)
			{
				_vec3 vLook;
				m_pTransform->Get_Info(INFO_LOOK, &vLook);
				D3DXVec3Normalize(&vLook, &vLook);
				m_pTransform->Move_Pos(&(vLook * 5.f * fTimeDelta));
				m_pTransform->Static_Update();
			}
		}
	}
	CGameObject::Update_Object(fTimeDelta);

	m_pTransform->Get_Info(INFO_POS, &vPos);

	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		CTransform*	BoxTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
			BoxTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}

	Fly_Effect();
	return 0;
}

void CFlight::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CFlight::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBomb->Get_WorldMatrixPointer());
	m_pBombTexture->Set_Texture();
	m_pBombBuffer->Render_Buffer();
}

HRESULT CFlight::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pCalculator = dynamic_cast<CCalculator *>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	pComponent = m_pBombTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	return S_OK;
}

HRESULT CFlight::Set_Material()
{
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));

	Material.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Material.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Material.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	Material.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	Material.Power = 0.f;

	m_pGraphicDev->SetMaterial(&Material);

	return S_OK;
}

void CFlight::Fly_Effect()
{
	_vec3 vPos;														// 불
	_vec3 vDir;
	if (!m_pCubeParticle)
		m_pCubeParticle = dynamic_cast<CCubeParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"CubeParticle"));
	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		if (0 == _tcscmp(iter.first, L"FIRE2"))
		{
			CTransAxisBox* m_pFireBox = dynamic_cast<CTransAxisBox*>(iter.second);
			_matrix matFinal;
			m_pFireBox->Get_Final(&matFinal); // 30 31 32 << pos
			vPos.x=matFinal.m[3][0];
			vPos.y=matFinal.m[3][1];
			vPos.z=matFinal.m[3][2];
				dynamic_cast<CCubeParticle*>(m_pCubeParticle)->Set_PclePos(vPos);
			//	dynamic_cast<CCubeParticle*>(m_pCubeParticle)->Set_PcleDir(vDir);
				for (_int i = 0; i < 50; ++i)
				{
					m_pCubeParticle->addParticle();
				}
		}
		if (0 == _tcscmp(iter.first, L"FIRE4"))
		{
			CTransAxisBox* m_pFireBox = dynamic_cast<CTransAxisBox*>(iter.second);
			_matrix matFinal;
			m_pFireBox->Get_Final(&matFinal); // 30 31 32 << pos
			vPos.x = matFinal.m[3][0];
			vPos.y = matFinal.m[3][1];
			vPos.z = matFinal.m[3][2];
			dynamic_cast<CCubeParticle*>(m_pCubeParticle)->Set_PclePos(vPos);
			//	dynamic_cast<CCubeParticle*>(m_pCubeParticle)->Set_PcleDir(vDir);
			for (_int i = 0; i < 50; ++i)
			{
				m_pCubeParticle->addParticle();
			}
		}
	}
}

void CFlight::Random(void)
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
	
	if (this == dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE")))
	{
		m_ShufflePos.front().y = 30.f;

		Replace(m_ShufflePos.front(), vAngle, vDir);

		Set_Speed(0.f);
	}
	else
	{
		Replace(m_ShufflePos.front(), vAngle, vDir);

		Set_Speed(_float(m_ShuffleSpeed.front()));
	}

	vAfterDir = _vec3(0.f, 0.f, 0.f);
}

HRESULT CFlight::Build(void)
{
	HANDLE      hFile = CreateFile(L"../../Data/Rocket/ROCKET_2.dat",      // 파일의 경로와 이름	
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

void CFlight::Key_Input(const _float& fTimeDelta)
{
	//m_pTransform->Move_Pos(&(-vLook * 1.f * fTimeDelta));

	/*long		dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 20.f));
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_pTransform->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 20.f));
	}*/

	if (m_fBulletTime > 0.2f)
	{
		if (Get_DIMouseState(DIM_LB))
		{
			Fire_Bullet();

			
			Set_Shoot(true);
			m_fBulletTime = 0.f;
		}
	}
	if (m_fBulletTime > 0.5f)
	{
		if (Get_DIMouseState(DIM_RB))
		{
			Bombing();
			Set_Shoot(true);
			m_fBulletTime = 0.f;
		}
	}

}

void CFlight::Look_Direction()
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
	yaw	  = atan2f(2.0f * (qRot.x * qRot.z + qRot.w * qRot.y), (-sqx - sqy + sqz + sqw));
	roll  = atan2f(2.0f * (qRot.x * qRot.y + qRot.w * qRot.z), (-sqx + sqy - sqz + sqw));

	list<pair<const _tchar*, CGameObject*>> ListBox = *(pMyLayer->Get_GamePairPtr());

	for (auto& iter : ListBox)
	{
		if (0 == _tcscmp(iter.first, L"A_ROOT"))
		{
			_vec3 vAngle;
			CTransform* Transform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));
			Transform->Get_Angle(&vAngle);
			Transform->Set_Angle(&_vec3(yaw, pitch, roll));
		}
	}
}

void CFlight::Look_Direction_Only_Y()
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

void CFlight::Fire_Bullet()
{
	m_iPosSet *= -1;
	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	vPos.x += 0.5f * m_iPosSet;

	_vec3 vMouse;
	vMouse = m_pCalculator->Get_Mouse_World(g_hWnd);

	_vec3 vLook = vMouse - vPos;
	D3DXVec3Normalize(&vLook, &vLook);

	CPoolMgr::GetInstance()->Reuse_ExBullet(m_pGraphicDev, &vPos, &vLook, 100.f);
}

void CFlight::Bombing()
{
	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	CGameObject* pGameObject = CFlightBomb::Create(m_pGraphicDev, vPos, L"Bomb");
	NULL_CHECK_RETURN(pGameObject, );
	CLayer* pLayer = Get_Layer(STAGE_SKILL);
	FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject),);


	//cout << vDirection.x << " " << vDirection.y << " " << vDirection.z << " " << endl;
}

void CFlight::Move(const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	_vec3 vSpotPos;
	dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"FlightSpot", TRANSFORM_COMP, ID_DYNAMIC))
		->Get_Info(INFO_POS, &vSpotPos);

	_vec3 vSpotRight;
	dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"FlightSpot", TRANSFORM_COMP, ID_DYNAMIC))
		->Get_Info(INFO_RIGHT, &vSpotRight);
	D3DXVec3Normalize(&vSpotRight, &vSpotRight);

	_vec3 vDir = vSpotPos - vPos;
	_float fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Move_Pos(&(vDir * fDistance * 2.f * fTimeDelta));

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	_vec3 vEye = { matView.m[3][0],matView.m[3][1],matView.m[3][2] };
	_vec3 vCamLook = { matView.m[2][0],matView.m[2][1],matView.m[2][2] };
	D3DXVec3Normalize(&vCamLook, &vCamLook);

	m_fRotY = vSpotPos.y - vEye.y;
	m_fTurn = vSpotPos.x - vEye.x;

	{
		m_fRotX += Get_DIMouseMove(DIMS_X);

		if (m_fRotX > 1000.f)
			m_fRotX = 1000.f;
		else if (m_fRotX < -1000.f)
			m_fRotX = -1000.f;

		if (m_fRotX > 0)
			m_fRotX -= 10.f;
		else if (m_fRotX < 0)
			m_fRotX += 10.f;

		if (m_fRotX > -10.f && m_fRotX < 10.f)
			m_fRotX = 0.f;
	}

	/*{
		m_fRotY += Get_DIMouseMove(DIMS_Y);
		if (m_fRotY > 600.f)
			m_fRotY = 600.f;
		else if (m_fRotY < -600.f)
			m_fRotY = -600.f;

		if (m_fRotY > 0)
			m_fRotY -= 10.f;
		else if (m_fRotY < 0)
			m_fRotY += 10.f;

		if (m_fRotY > -10.f && m_fRotY < 10.f)
			m_fRotY = 0.f;
	}*/

	{
		_float fDot = D3DXVec3Dot(&vSpotRight, &vCamLook);
		_float fAngle = acosf(fDot);
		m_fTurn = D3DXToDegree(fAngle) - 90.f;

		if (vSpotRight.x < vCamLook.x || vSpotRight.z < vCamLook.z)
		{
			m_fTurn = D3DX_PI * 2.f - m_fTurn;
		}
		if (vCamLook.z < 0.f)
		{
			m_fTurn += 180.f;
		}
	}

	m_pTransform->Set_Rotation(ROT_Z, D3DXToRadian(-m_fRotX / 20.f));
	m_pTransform->Set_Rotation(ROT_X, -m_fRotY / 10.f);
	m_pTransform->Set_Rotation(ROT_Y, D3DXToRadian(m_fTurn));

	for (auto& iter : *(pMyLayer->Get_GamePairPtr()))
	{
		CTransform*	BoxTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(L"Proto_TransformCom", ID_STATIC));

		if (0 == _tcscmp(iter.first, L"A_ROOT"))
		{
			BoxTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}
}

CFlight * CFlight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, const _vec3 & vDir, _tchar * Name)
{
	CFlight* pInstance = new CFlight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, Name)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFlight::Free(void)
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
