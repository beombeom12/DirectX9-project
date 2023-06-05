#include "stdafx.h"
#include "..\Header\ComboUI.h"

#include "Export_Function.h"
#include "CubePlayer.h"

CComboUI::CComboUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CComboUI::~CComboUI()
{
}

HRESULT CComboUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fX = 420.f;
	m_fY = -225.f;
	m_fSizeX = 240.f;
	m_fSizeY = 18.f;

	m_fSizeHUDX = 248.f;
	m_fSizeHUDY = 26.f;
	//FONT
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Combo", L"Electronic Highway Sign", 20, 30, FW_BOLD), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Kill", L"Electronic Highway Sign", 10, 15, FW_NORMAL), E_FAIL);
	return S_OK;
}

_int CComboUI::Update_Object(const _float & fTimeDelta)
{
	_int iResult = CGameObject::Update_Object(fTimeDelta);

	if (m_bSwitch && m_fComboCnt >= 2)
	{
		m_fFrame -= fTimeDelta;
		if (m_fFrame < 0.f)
		{
			m_bSwitch = false;
			m_fComboCnt = 0.f;
			m_fKillCnt = 0.f;
			m_fFrame = 2.f;
		}

		m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
		m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);
		m_pHUDTransCom->Set_Scale(m_fSizeHUDX, m_fSizeHUDY, 1.f);
		m_pHUDTransCom->Set_Pos(m_fX, -m_fY, 0.f);

		if(m_bSwitch)
			Add_RenderGroup(RENDER_UI, this);

		m_strComboCnt = to_wstring(_uint(m_fComboCnt));
		m_strKillCnt = to_wstring(_uint(m_fKillCnt));
	}

	return iResult;
}

void CComboUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
	m_strCombo = m_strComboCnt + L" X COMBO!";
	m_strKill = m_strKillCnt + L" X KILLS";
}

void CComboUI::Render_Object(void)
{
	_matrix      matWorld, matView;

	matWorld = *m_pHUDTransCom->Get_WorldMatrixPointer();

	matView = *D3DXMatrixIdentity(&matView);

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_matProj, sizeof(_matrix))))
		return;

	m_pHUDTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
	m_pHUDBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();

	matWorld = *m_pTransCom->Get_WorldMatrixPointer();
	D3DXMatrixTranspose(&matWorld, &matWorld);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_matProj, sizeof(_matrix))))
		return;

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
	m_pBufferCom->Resize_Buffer(m_fFrame * 0.5f);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();

	Render_Font(L"Combo", m_strCombo.c_str(), &_vec2(1095.f, 170.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Render_Font(L"Kill", m_strKill.c_str(), &_vec2(1120.f,250.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CComboUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMBO_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMBO_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(COMBOBAR_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COMBOBAR_TEX , pComponent });

	pComponent = m_pHUDBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMBOBAR_HUD_COMP));
	NULL_CHECK_RETURN(m_pHUDBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMBOBAR_HUD_COMP, pComponent });

	pComponent = m_pHUDTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pHUDTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Combo_HUD_Trans", pComponent });

	pComponent = m_pHUDTextureCom = dynamic_cast<CTexture*>(Clone_Proto(COMBOBAR_HUD_TEX));
	NULL_CHECK_RETURN(m_pHUDTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COMBOBAR_HUD_TEX , pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CComboUI * CComboUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CComboUI* pInstance = new CComboUI(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CComboUI::Free(void)
{
	CGameObject::Free();
}

