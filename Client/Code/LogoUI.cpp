#include "stdafx.h"
#include "..\Header\LogoUI.h"
#include "PlayButton.h"
#include "Logo.h"

CLogoUI::CLogoUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLogoUI::~CLogoUI()
{
}

HRESULT CLogoUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_component(), E_FAIL);

	return S_OK;
}

_int CLogoUI::Update_Object(const _float & fTimeDelta)
{
	if (dynamic_cast<CLogo*>(Get_Scene())->Get_bVideo() == true)
	{
		CGameObject::Update_Object(fTimeDelta);

		Add_RenderGroup(RENDER_UI, this);
	}

	return 0;
}

void CLogoUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CLogoUI::Render_Object(void)
{
	Begin_OrthoProj();
	m_pRcTexCom->Render_Buffer();
	End_OrthoProj();
}

void CLogoUI::Begin_OrthoProj()
{
	_matrix matWorld, matView, matProj, matOrtho;
	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixIdentity(&matView);
	matView.m[0][0] = 600.f; // 이미지 가로
	matView.m[1][1] = 900.f; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = m_pTransCom->m_vInfo[INFO_POS].x + 600.f;
	matView.m[3][1] = m_pTransCom->m_vInfo[INFO_POS].y;

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

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
}

void CLogoUI::End_OrthoProj()
{
	m_pShaderCom->End_Shader();
}

HRESULT CLogoUI::Add_component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pRcTexCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pRcTexCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_LogoUITexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_LogoUITexture", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CLogoUI * CLogoUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoUI *   pInstance = new CLogoUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogoUI::Free(void)
{
	CGameObject::Free();
}