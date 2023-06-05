#include "stdafx.h"
#include "..\Header\PlayerMapping.h"
#include "Meteor.h"
#include "StaticField.h"

CPlayerMapping::CPlayerMapping(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayerMapping::~CPlayerMapping()
{
}

HRESULT CPlayerMapping::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Scale(0.5f, 0.5f, 0.5f);

	m_pBombTransform->Set_Scale(8.f, 8.f, 1.f);
	m_pBombTransform->Rotation(ROT_X,D3DXToRadian(90.f));

	return S_OK;
}

_int CPlayerMapping::Update_Object(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);
	
	Add_RenderGroup(RENDER_MAPSETTING, this);

	if (m_bBombard)
	{
		m_fFrame += fTimeDelta;
		m_fSkillFrame += fTimeDelta;

		if (m_fFrame >= 0.2f)
		{
			Bombard(fTimeDelta);
			m_fFrame = 0.f;
		}

		if (m_fSkillFrame >= 5.f)
		{
			m_fFrame = 0.f;
			m_fSkillFrame = 0.f;
			m_bBombard = false;
		}
	}

	return 0;
}

void CPlayerMapping::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
	
	if (nullptr == m_pTransformPlayer)
	{
		m_pTransformPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK(m_pTransformPlayer);
	}
	
	_vec3 vPos;
	m_pTransformPlayer->Get_Info(INFO_POS, &vPos);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

	if (!m_bWorldMap && !m_bBombard)
	{
		m_pBombTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
}

void CPlayerMapping::Render_Object(void)
{
	if (m_bWorldMap)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		//빛표현 블랜딩
		//		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		//간단한 그림자
		//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		//곱셈합성 스킬뷰에 쓰면 좋을듯
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);

		//제곱합성 밝은색은 그대로, 어두운색은 더 어둡게 ( 보스전 방 꾸미는 데 쓰면 좋을듯)
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);

		//네거티브, 포지티브 합성 (검<->흰)
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);

		//불투명
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	Begin_OrthoProj();
	m_pTexture->Set_Texture(60);
	if(m_bMinimap)
		m_pBufferCom->Render_Buffer();
	End_OrthoProj();
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBombTransform->Get_WorldMatrixPointer());
	
	if (m_bWorldMap)
	{
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pBombTexure->Set_Texture();
		m_pBombBuffer->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPlayerMapping::Begin_OrthoProj()
{
	_vec3 vPos;
	m_pTransformPlayer->Get_Info(INFO_POS, &vPos);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

	_matrix matWorld, matView, matProj, matOrtho;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	memcpy(&m_matWorld, &matWorld, sizeof(_matrix));
	memcpy(&m_matView, &matView, sizeof(_matrix));
	memcpy(&m_matProj, &matProj, sizeof(_matrix));

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = PINGSIZE*WINCX / WINCY * MAPCX/MAPCY; // 이미지 가로
	matView.m[1][1] = PINGSIZE*WINCX / WINCY* MAPCX / MAPCY; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = MAPPOSX - (MAPCX)+vPos.x * (((float)MAPCX * 2) / (float)VTXCNTX); //- PINGSIZE /2;
	matView.m[3][1] = MAPPOSY - (MAPCY)+vPos.z * (((float)MAPCY * 2) / (float)VTXCNTZ); //- PINGSIZE /2;
	matView.m[3][2] = 0.001f;

	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CPlayerMapping::End_OrthoProj()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CPlayerMapping::Key_Input(const _float& fTimeDelta)
{
	m_bWorldMap = dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Worldmap();
	m_bMinimap = dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Minimap();

	_vec3	vDir(0, 0, 0);
	_vec3	vNormal(0, 0, 0);
	_vec3 vPos;
	m_pBombTransform->Get_Info(INFO_POS, &vPos);

	if (Get_DIKeyState(DIK_W) && Get_DIKeyState(DIK_A))
	{
		_vec3 vUp, vRight;
		m_pBombTransform->Get_Info(INFO_UP, &vUp);
		m_pBombTransform->Get_Info(INFO_RIGHT, &vRight);
		vRight *= -1.f;

		vDir = vUp + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_W) && Get_DIKeyState(DIK_D))
	{
		_vec3 vUp, vRight;
		m_pBombTransform->Get_Info(INFO_UP, &vUp);
		m_pBombTransform->Get_Info(INFO_RIGHT, &vRight);
		
		vDir = vUp + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_S) && Get_DIKeyState(DIK_D))
	{
		_vec3 vUp, vRight;
		m_pBombTransform->Get_Info(INFO_UP, &vUp);
		m_pBombTransform->Get_Info(INFO_RIGHT, &vRight);
		vUp *= -1.f;

		vDir = vUp + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_S) && Get_DIKeyState(DIK_A))
	{
		_vec3 vUp, vRight;
		m_pBombTransform->Get_Info(INFO_UP, &vUp);
		m_pBombTransform->Get_Info(INFO_RIGHT, &vRight);
		vUp *= -1.f;
		vRight *= -1.f;

		vDir = vUp + vRight;

		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_W))
	{
		m_pBombTransform->Get_Info(INFO_UP, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
	}
	else if (Get_DIKeyState(DIK_S))
	{
		m_pBombTransform->Get_Info(INFO_UP, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		vDir = -vDir;
	}
	else if (Get_DIKeyState(DIK_A))
	{
		m_pBombTransform->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		vDir = -vDir;
	}
	else if (Get_DIKeyState(DIK_D))
	{
		m_pBombTransform->Get_Info(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
	}

	m_pBombTransform->Move_Pos(&(vDir * m_fSpeed * fTimeDelta));

	_long MoveX = Get_DIMouseMove(DIMS_X);
	_long MoveY = Get_DIMouseMove(DIMS_Y);
	_long MoveZ = Get_DIMouseMove(DIMS_Z);

	m_pBombTransform->Rotation(ROT_Y, D3DXToRadian(MoveX / 10.f));

	if (Mouse_Down(DIM_LB) && m_bWorldMap)
	{
		m_bBombard = true;
		_float fGunSound = 1.f;
		Engine::PlaySoundGun(L"MeteorCreate.wav", SOUND_EFFECT, 0.3f);
		Engine::PlaySoundGun(L"MeteorCreate2.wav", SOUND_EFFECT, fGunSound);
		dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Switch_Worldmap();
	}
}

HRESULT CPlayerMapping::Bombard(const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pBombTransform->Get_Info(INFO_POS, &vPos);
	
	srand((unsigned int)time(NULL)+ m_iRand);
	m_iRand = rand() % 1000;
	int iDir = rand() % 4;
	int iRand = rand() % 4;

	_vec3 vRand;

	if(iDir == 0)
		vRand = _vec3(vPos.x + iRand, vPos.y, vPos.z + iRand);
	else if(iDir == 1)
		vRand = _vec3(vPos.x + iRand, vPos.y, vPos.z - iRand);
	else if(iDir == 2)
		vRand = _vec3(vPos.x - iRand, vPos.y, vPos.z + iRand);
	else if(iDir == 3)
		vRand = _vec3(vPos.x - iRand, vPos.y, vPos.z - iRand);

	CGameObject* pGameObject = CMeteor::Create(m_pGraphicDev, vRand);

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	CLayer* pLayer = Get_Layer(STAGE_SKILL);
	FAILED_CHECK_RETURN(pLayer->Add_GameList(pGameObject), E_FAIL);
	
	return S_OK;
}

HRESULT CPlayerMapping::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pBombBuffer = dynamic_cast<CRcTex*>(Engine::Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Bomb_RcTexCom", pInstance });

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pInstance });

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });
		
	pInstance = m_pBombTexure = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bomb_TEX"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Bomb_TEX", pInstance });

	pInstance = m_pBombTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Bomb_TransCom", pInstance });

	pInstance = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pInstance });

	return S_OK;
}

CPlayerMapping * CPlayerMapping::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerMapping* pInstance = new CPlayerMapping(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayerMapping::Free(void)
{
	CGameObject::Free();
}


