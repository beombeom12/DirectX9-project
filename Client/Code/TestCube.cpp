#include "stdafx.h"
#include "TestCube.h"
#include "WallMapping.h"
#include "LetterBox.h"
#include "Export_Function.h"
#include "CubePlayer.h"
#include "StaticCamera.h"
#include "Shop.h"
static _int m_iMappingCnt = 0;
static _int m_iLetterCnt = 0;

CTestCube::CTestCube(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)

{
}

CTestCube::CTestCube(const CGameObject& rhs) : CGameObject(rhs)
{
}

CTestCube::~CTestCube()
{
}

HRESULT CTestCube::Ready_Object(int PosX, int PosY)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	if (PosX == 0 && PosY == 0) {}
	else
	{ Set_TransformPositon(); }
	
	_vec3 vScale = { 0.5f, 0.5f, 0.5f };
	m_pTransCom->Set_Scale(&vScale);
	
	return S_OK;
}

_int CTestCube::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
	{	
		m_pStaticCam->CameraShaking();
		_float fGunSound = 1.f;
		Engine::PlaySoundW(L"EXPLOLRG.WAV", SOUND_DOOR, fGunSound);
		return -1;
	}
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	if (Engine::Get_Scene()->Get_SceneId() == STAGE_SCENE)
	{
		Update_NullCheck();
		CGameObject::Update_Object(fTimeDelta);
		if (!(dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Worldmap()))
		{
			Add_RenderGroup(RENDER_NONALPHA, this);
		}
		Wall_Mapping();
		Interact();
		m_fTimer += fTimeDelta;
	}
	else if (Engine::Get_Scene()->Get_SceneId() == TOOL_SCENE)
	{
		CGameObject::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return 0;
}

// 툴쓸때는 이거꺼주고 위에꺼 써야함
//_int CTestCube::Update_Object(const _float& fTimeDelta)
//{
//	if (m_bDead)
//	{
//		return -1;
//	}
//	Update_NullCheck();
//	CGameObject::Update_Object(fTimeDelta);
//	if (!(dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Worldmap()))
//	{
//		Add_RenderGroup(RENDER_NONALPHA, this);
//	}
//	Wall_Mapping();
//	Interact();
//	m_fTimer += fTimeDelta;
//	return 0;
//}

void CTestCube::Render_Object()
{
#pragma region 쉐이더
	//CCamera* pCam = dynamic_cast<CCamera*>(Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"));
	//_vec4 vCamPos = { pCam->GetEye().x,pCam->GetEye().y,pCam->GetEye().z, 1.f };
	//_vec4 vLightPos = { 50.f, 50.f, -50.f, 1.f };
	//D3DXVECTOR4      gLightColor(0.7f, 0.7f, 1.f, 1.f);

	//_matrix         matWorld, matView, matProj;
	//matWorld = *m_pTransCom->Get_WorldMatrixPointer();
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//D3DXMatrixTranspose(&matWorld, &matWorld);
	//D3DXMatrixTranspose(&matView, &matView);
	//D3DXMatrixTranspose(&matProj, &matProj);

	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", matWorld, sizeof(_matrix))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_matrix))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("gWorldLightPosition", &vLightPos, sizeof(_vec4))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("gWorldCameraPosition", &vCamPos, sizeof(_vec4))))
	//	return;
	//if (FAILED(m_pShaderCom->Set_RawValue("gLightColor", &gLightColor, sizeof(_vec4))))
	//	return;

	//m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iTexIndex);

	//m_pShaderCom->Begin_Shader(0);

	//m_pBufferCom->Render_Buffer();

	//m_pShaderCom->End_Shader();

	//if (m_bWireFrame)
	//	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#pragma endregion
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	FAILED_CHECK_RETURN(Set_Material(), );
	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();
}

// 큐브를 선택 후 터레인 자리위에 올려 놓는 함수
void CTestCube::Set_TransformPositon()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_MapTool", L"TerrainByTool", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_MapTool", L"TerrainByTool", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );

	_vec3 Temp = m_pCalculatorCom->Peek_Target_Vector(g_hWnd, &(_vec3(0.f,0.f,0.f)), pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);
}

bool CTestCube::Set_SelectGizmo()
{
	if (m_pCalculatorCom->Peek_Cube_Target(g_hWnd, &(_vec3(0.f, 0.f, 0.f)), m_pBufferCom, m_pTransCom))
		return true;

	return false;
}

void CTestCube::Update_NullCheck()
{
	if (!m_pMonsterParticle)
		m_pMonsterParticle = dynamic_cast<CMonsterParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"MonsterParticle"));
}

HRESULT CTestCube::Interact(void)
{
	CGameObject*		pGameObject = nullptr;
	CProjectileParticle* m_pProjectileParicle = nullptr;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	if (!m_pProjectileParicle)
		m_pProjectileParicle = dynamic_cast<CProjectileParticle*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ProjectileParticle"));
	
	_vec3		vPlayerPos;
	_vec3		vPos;
	_vec3		vDir;
	_float		fDistance;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	pPlayerTransformCom->Get_Info(Engine::INFO_POS, &vPlayerPos);
	vDir = vPos - vPlayerPos;
	fDistance = D3DXVec3Length(&vDir);

	_vec3		vGrenadePos;
	_vec3		vGPos;
	_vec3		vGDir;
	_float		fGDistance;
	vGPos = m_pProjectileParicle->Get_DeadPos();
	pPlayerTransformCom->Get_Info(Engine::INFO_POS, &vGrenadePos);
	vGDir = vPos- vGPos ;
	fGDistance = D3DXVec3Length(&vGDir);


	if (fDistance <= 12.f)
	{
		m_bSwitch = true;
	}
	if (fDistance > 12.f)
	{
		m_bSwitch = false;
	}


	if (!m_bLetterboxInit&& m_bSwitch)
	{
		if (m_iTexIndex == 37 || m_iTexIndex == 99)
		{
			
			m_bLetterboxInit = true;
			m_pLetterBox = CLetterBox::Create(m_pGraphicDev, L"Press [E] to Interact", sizeof(L"Press [E] to Interact"), 0);

			//TCHAR* szCntName = new TCHAR[64];
			//wsprintf(szCntName, L"");
			//const _tchar*	szNumbering = L"WallLetter_%d";
			//wsprintf(szCntName, szNumbering, m_iLetterCnt);
			//Engine::Add_GameObject(STAGE_MAPPING, m_pLetterBox, szCntName);
			//m_listWallCnt.push_back(szCntName);

			//++m_iLetterCnt;
		}
	}

	if (m_bSwitch && Get_DIKeyState(DIK_E))
	{
		if (m_iTexIndex == 37 || m_iTexIndex == 99) //초록색문
			if (vPos.y < 15)
			{
				_float fGunSound = .5f;
				Engine::PlaySoundW(L"portal_2.wav", SOUND_DOOR, fGunSound);
				m_bDoorOpen = true;
				CGameObject* pGameObject = Get_GameObject(STAGE_CHARACTER, L"PLAYER");
				dynamic_cast<CCubePlayer*>(pGameObject)->Set_DoorOpen(true);
			}
	}	

	if (fGDistance < 5)
	{
		if (m_iTexIndex == 45) // 갈색문
		{
			if (vPos.y <= 5)
			{
			
				m_bDead = true;
				m_bDoorUp = true;
				m_bCameraShaking = true;
				for (_int i = 0; i < 15; ++i)
				{
					m_pBrownCloudEffect->addParticle();
				};
			}
		}
	}

	if (m_bSwitch && m_pLetterBox != nullptr)
	{
		if(!(dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch()))
			dynamic_cast<CLetterBox*>(m_pLetterBox)->On_Switch();
	}
	if (m_pLetterBox != nullptr)
	{
		if (!m_bSwitch || (dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch()))
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox)->Off_Switch();
		}
	}

	if (m_bSwitch &&m_iTexIndex == 45)
	{
		m_bDoorFall = true;
	}
	if (!m_bSwitch &&m_iTexIndex == 45)
	{
		m_bDoorUp = true;
	}
	if(m_bDoorOpen)
	{ 
		if (vPos.y > -15)
		{
			vPos.y -= 0.1f;			
			m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}

	if (m_bDoorFall&&!m_bDoorUp)
	{
		if (m_pStaticCam == nullptr)
			m_pStaticCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"));
		if (!m_pBrownCloudEffect)
			m_pBrownCloudEffect = dynamic_cast<CBrownCloudEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"BrownCloudEffect"));
		m_pBrownCloudEffect->Set_PclePos(vPos);
		if (vPos.y > 5)
		{
			if (vPos.y <= 6.)
			{
				_float fGunSound = 1.f;
				Engine::PlaySoundW(L"EXPLOLRG.WAV", SOUND_DOOR, fGunSound);
			}
			vPos.y -= 1.f;
			m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
			m_fTimer = 0.f;
		}
		if (vPos.y <= 5)
		{
			
			m_bDoorFall = false;	
			
			if (m_bCameraShaking)
			{
				m_pStaticCam->CameraShaking();
				for (_int i = 0; i < 15; ++i)
					{
						m_pBrownCloudEffect->addParticle();
					}
				if (m_fTimer > 0.5f)
					m_bCameraShaking = false;
			}
		}
	}
	if (m_bDoorUp&&!m_bDoorFall)
	{
		if (vPos.y < 10)
		{
			vPos.y += 0.5f;
			m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
		if (vPos.y >= 10)
			m_bDoorUp = false;
		m_bCameraShaking = true;
	}

	return S_OK;
}

HRESULT CTestCube::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(CUBETEX_SHADER));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBETEX_SHADER, pComponent });

	return S_OK;
}

HRESULT CTestCube::Set_Material()
{
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));

	Material.Diffuse = D3DXCOLOR(1.f, 0.5f, 0.5f, 1.f);
	Material.Specular = D3DXCOLOR(1.f, 0.4f, 0.4f, 1.f);
	Material.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.4f, 1.f);
	Material.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	Material.Power = 0.f;

	m_pGraphicDev->SetMaterial(&Material);

	return S_OK;
}

HRESULT CTestCube::Wall_Mapping(void)
{
	_vec3		vPos;
	_vec3		vSize;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Get_Scale(&vSize);
	if (!m_bMappingInit)
	{
		m_bMappingInit = true;

		CGameObject*	m_pMapWall = CWallMapping::Create(m_pGraphicDev);
		if (m_pMapWall != nullptr && m_iTexIndex == 37 || m_iTexIndex == 45 || m_iTexIndex == 99)
		{
			
			dynamic_cast<CWallMapping*>(m_pMapWall)->Set_Texture(37);
		}
		TCHAR* szCntName = new TCHAR[64];
		wsprintf(szCntName, L"");
		const _tchar*	szNumbering = L"MapWall_%d";
		wsprintf(szCntName, szNumbering, m_iMappingCnt);
		Engine::Add_GameObject(STAGE_MAPPING, m_pMapWall, szCntName);
		m_listWallCnt.push_back(szCntName);

		
		m_pWallMapping = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_MAPPING, szCntName, TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pWallMapping, E_FAIL);
		++m_iMappingCnt;
		
	}

	m_pWallMapping->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pWallMapping->Set_Scale(vSize.x, vSize.y, vSize.z);
	
	return S_OK;
}
CTestCube* CTestCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, int Posx, int Posy)
{
	CTestCube*	pInstance = new CTestCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CTestCube::Free()
{
	for (auto& iter : m_listWallCnt)
	{
		if (iter != nullptr)
			delete iter;
	}

	m_listWallCnt.clear();	
	m_listLetterCnt.clear();
	CGameObject::Free();
}

