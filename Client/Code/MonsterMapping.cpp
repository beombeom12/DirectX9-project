#include "stdafx.h"
#include "..\Header\MonsterMapping.h"


CMonsterMapping::CMonsterMapping(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMonsterMapping::~CMonsterMapping()
{
}

HRESULT CMonsterMapping::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Scale(0.5f, 0.5f, 0.5f);
	return S_OK;
}

_int CMonsterMapping::Update_Object(const _float & fTimeDelta)
{
	Add_RenderGroup(RENDER_MAPSETTING, this);
	
	Key_Input();

	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CMonsterMapping::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CMonsterMapping::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	Begin_OrthoProj();
	m_pTexture->Set_Texture(43);
	if(m_bMinimap)
		m_pRcCom->Render_Buffer();
	End_OrthoProj();
	if(m_bWorldMap)
		m_pCube->Render_Buffer();
}


void CMonsterMapping::Begin_OrthoProj()
{
	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	_matrix matWorld, matView, matProj, matOrtho;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	memcpy(&m_matWorld, &matWorld, sizeof(_matrix));
	memcpy(&m_matView, &matView, sizeof(_matrix));
	memcpy(&m_matProj, &matProj, sizeof(_matrix));

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = PINGSIZE*WINCX / WINCY* MAPCX / MAPCY; // 이미지 가로
	matView.m[1][1] = PINGSIZE*WINCX / WINCY* MAPCX / MAPCY; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = MAPPOSX - (MAPCX) + vPos.x * ( ((float)MAPCX*2) / (float)VTXCNTX); //- PINGSIZE /2;
	matView.m[3][1] = MAPPOSY - (MAPCY) + vPos.z * ( ((float)MAPCY*2) / (float)VTXCNTZ); //- PINGSIZE /2;
	matView.m[3][2] = 0.001f;


	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CMonsterMapping::End_OrthoProj()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CMonsterMapping::Key_Input(void)
{
	m_bWorldMap = dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Worldmap();
	m_bMinimap = dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Minimap();
}



HRESULT CMonsterMapping::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pCube = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pInstance });

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pInstance });

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });

	pInstance = m_pRcCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pRcCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pInstance });


	return S_OK;
}

CMonsterMapping * CMonsterMapping::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterMapping* pInstance = new CMonsterMapping(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CMonsterMapping::Free(void)
{
	CGameObject::Free();
}


