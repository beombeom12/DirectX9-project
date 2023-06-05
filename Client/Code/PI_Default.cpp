#include "stdafx.h"
#include "..\Header\PI_Default.h"

CPI_Default::CPI_Default(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPI_Default::~CPI_Default()
{
}

HRESULT CPI_Default::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPI_Default::Update_Object(const _float & fTimeDelta)
{
	//m_TransformCom->Static_Update();
	//m_TransformCom->Billboard_Transform(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);


	Engine::CGameObject::Update_Object(fTimeDelta);
	return 0;
}

void CPI_Default::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
	
}

void CPI_Default::Render_Object(void)
{	
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);// 깊이버퍼를 없애는 함수
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_TransformCom->Get_WorldMatrixPointer());
	//m_TextureCom->Set_Texture(0);
	//m_RcTexCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	

	
	// 직교투영 행렬
	_matrix		matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 0.f, 1.f);

	matView(0, 0) *= 111.1f;
	matView(1, 1) *= 111.1f;
	matView(2, 2) *= 111.1f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	m_TextureCom->Set_Texture(0);
	m_RcTexCom->Render_Buffer();
	
}

HRESULT CPI_Default::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_RcTexCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_RcTexCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_TextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(m_TextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ButtonPlayTexture", pComponent });

	pComponent = m_TransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_TransformCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TransformCom", pComponent });

	return S_OK;
}

CPI_Default * CPI_Default::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPI_Default* pInsatnce = new CPI_Default(pGraphicDev);

	if (FAILED(pInsatnce->Ready_Object()))
	{
		Safe_Release(pInsatnce);
		return nullptr;
	}
	return pInsatnce;
}

void CPI_Default::Free(void)
{
	CGameObject::Free();
}
