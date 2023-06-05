#include "stdafx.h"
#include "..\Header\WallMapping.h"


CWallMapping::CWallMapping(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CWallMapping::~CWallMapping()
{
}

HRESULT CWallMapping::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Scale(0.5f, 0.5f, 0.5f);
	
	return S_OK;
}

_int CWallMapping::Update_Object(const _float & fTimeDelta)
{
	Add_RenderGroup(RENDER_MAPSETTING, this);
	
	Key_Input();

	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CWallMapping::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CWallMapping::Render_Object(void)
{
	if (m_bWorldMap)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		//빛표현 블랜딩
		//		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		//간단한 그림자
		//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		//곱셈합성 스킬뷰에 쓰면 좋을듯
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);

		//제곱합성 밝은색은 그대로, 어두운색은 더 어둡게 ( 보스전 방 꾸미는 데 쓰면 좋을듯)
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);

		//네거티브, 포지티브 합성 (검<->흰)
		//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	Begin_OrthoProj();
	if (m_bMinimap)
	{
		m_pTexture->Set_Texture(m_iTextureIndex);
		m_pRcCom->Render_Buffer();
	}
	End_OrthoProj();
	if (m_bWorldMap)
	{
		m_pTexture->Set_Texture(46);
		m_pCube->Render_Buffer();
	}
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


void CWallMapping::Begin_OrthoProj()
{
	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	_vec3 vScale;
	m_pTransform->Get_Scale(&vScale);
	_matrix matWorld, matView, matProj, matOrtho;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	memcpy(&m_matWorld, &matWorld, sizeof(_matrix));
	memcpy(&m_matView, &matView, sizeof(_matrix));
	memcpy(&m_matProj, &matProj, sizeof(_matrix));

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = 2 * vScale.x *WINCX / WINCY* MAPCX / MAPCY;// PINGSIZE; // 이미지 가로
	matView.m[1][1] = 2 * vScale.z *WINCX/WINCY* MAPCX / MAPCY;// PINGSIZE; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = MAPPOSX - (MAPCX)+vPos.x * (((float)MAPCX * 2) / (float)VTXCNTX); //- PINGSIZE /2;
	matView.m[3][1] = MAPPOSY - (MAPCY)+vPos.z * (((float)MAPCY * 2) / (float)VTXCNTZ); //- PINGSIZE /2;
																						//matView.m[3][2] = m_pTransCom->m_vInfo[INFO_POS].z+0.1f;


	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CWallMapping::End_OrthoProj()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CWallMapping::Key_Input(void)
{
	m_bWorldMap = dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Worldmap();
	m_bMinimap = dynamic_cast<CBaseMapping*>(Engine::Get_GameObject(STAGE_MAPPING, L"BaseMapping"))->Get_Minimap();

}



HRESULT CWallMapping::Add_Component(void)
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

CWallMapping * CWallMapping::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWallMapping* pInstance = new CWallMapping(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CWallMapping::Free(void)
{
	CGameObject::Free();
}


