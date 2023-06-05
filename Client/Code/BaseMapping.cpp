#include "stdafx.h"
#include "..\Header\BaseMapping.h"


CBaseMapping::CBaseMapping(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBaseMapping::~CBaseMapping()
{
}

HRESULT CBaseMapping::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Scale(65.f, .5f, 65.f);
	m_pTransform->Set_Pos(65.f, -0.24f, 65.f);
	return S_OK;
}

_int CBaseMapping::Update_Object(const _float & fTimeDelta)
{
	Add_RenderGroup(RENDER_MAPSETTING, this);
	WorldMap();
	Key_Input();
	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CBaseMapping::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CBaseMapping::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		if (m_bWorldmap)
			m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	Begin_OrthoProj();
	m_pTexture->Set_Texture(0);
	if (m_bMinimap)
	{
		m_pRcCom->Render_Buffer();
	}
	End_OrthoProj();

	if (m_bWorldmap)
		m_pCube->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


void CBaseMapping::Key_Input(void)
{
	if(Key_Down(DIK_M))
	{		
		m_bMinimap = !m_bMinimap;
	}

	if (Key_Down(DIK_F))
	{
		m_bWorldmap = false;
	}
}

void CBaseMapping::Begin_OrthoProj()
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

	matView.m[0][0] = (float)MAPCX*2.7f; // 이미지 가로
	matView.m[1][1] = (float)MAPCY*2.85f; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = MAPPOSX - 4.f;
	matView.m[3][1] = MAPPOSY;
	matView.m[3][2] = 0.002f;//m_pTransform->m_vInfo[INFO_POS].z;; //+0.1f;

	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CBaseMapping::End_OrthoProj()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CBaseMapping::WorldMap(void)
{

}

HRESULT CBaseMapping::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pCube = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pInstance });

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Minimap"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Minimap", pInstance });

	/*pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pInstance });*/

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });

	pInstance = m_pRcCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pRcCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pInstance });


	return S_OK;
}

CBaseMapping * CBaseMapping::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBaseMapping* pInstance = new CBaseMapping(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CBaseMapping::Free(void)
{
	CGameObject::Free();
}


