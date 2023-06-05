#include "stdafx.h"
#include "..\Header\LoadingBar.h"
#include "Logo.h"

CLoadingBar::CLoadingBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLoadingBar::~CLoadingBar(void)
{
}

HRESULT CLoadingBar::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CLoadingBar::Update_Object(const _float & fTimeDelta)
{
	if (dynamic_cast<CLogo*>(Get_Scene())->Get_bVideo() == true)
	{
		CGameObject::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_UI, this);
	}
	return 0;
}

void CLoadingBar::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CLoadingBar::Render_Object(void)
{
	Begin_OrthoProj();
	if (m_fMin >= m_fMax)
		m_fMin = m_fMax;

	m_pBufferUICom->Resize_Buffer(m_fMin / m_fMax);
	m_pBufferUICom->Render_Buffer();
	End_OrthoProj();
	
	Begin_HudOrthoProj();
	m_pHUDBufferCom->Render_Buffer();
	End_HudOrthoProj();
}

HRESULT CLoadingBar::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	//loading
	pComponent = m_pBufferUICom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_LoadingBar"));
	NULL_CHECK_RETURN(m_pBufferUICom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LoadingBar", pComponent });
	
	pComponent = m_pUITransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pUITransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pUITextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_ReadyLoading"));
	NULL_CHECK_RETURN(m_pUITextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ReadyLoading", pComponent });

	//Hud
	pComponent = m_pHUDBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_HudLoading"));
	NULL_CHECK_RETURN(m_pHUDBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HudLoading", pComponent });

	pComponent = m_pHUDTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pHUDTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"TRANSFORM_COMP2", pComponent });

	pComponent = m_pHUDTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_HudLoadinga"));
	NULL_CHECK_RETURN(m_pHUDTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HudLoadinga", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });
	
	return S_OK;
}

void CLoadingBar::Begin_OrthoProj()
{
	_matrix matWorld, matView, matProj, matOrtho;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = 390.f; // 이미지 가로
	matView.m[1][1] = 150.f; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = m_pUITransCom->m_vInfo[INFO_POS].x + 540.f;
	matView.m[3][1] = m_pUITransCom->m_vInfo[INFO_POS].y - 250.f;

	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&matOrtho, &matOrtho);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matOrtho, sizeof(_matrix))))
		return;

	m_pUITextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
}

void CLoadingBar::End_OrthoProj()
{
	m_pShaderCom->End_Shader();
}

void CLoadingBar::Begin_HudOrthoProj()
{
	_matrix matWorld, matView, matProj, matOrtho;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = 400; // 이미지 가로
	matView.m[1][1] = 150.f; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = m_pHUDTransCom->m_vInfo[INFO_POS].x + 540.f;
	matView.m[3][1] = m_pHUDTransCom->m_vInfo[INFO_POS].y - 250.f;

	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&matOrtho, &matOrtho);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matOrtho, sizeof(_matrix))))
		return;

	m_pHUDTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
}

void CLoadingBar::End_HudOrthoProj()
{
	m_pShaderCom->End_Shader();
}

CLoadingBar * CLoadingBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingBar* pInstance = new CLoadingBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoadingBar::Free(void)
{
	CGameObject::Free();
}
