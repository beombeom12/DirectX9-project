#include "stdafx.h"
#include "..\Header\HitBarUI.h"


CHitBarUI::CHitBarUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CHitBarUI::~CHitBarUI()
{
}

HRESULT CHitBarUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = 0.f;
	m_fY = -300.f;
	m_fSizeX = 600.f;
	m_fSizeY = 270.f;
	m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);
	return S_OK;
}

_int CHitBarUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bSwitch)
	{
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Rotation_Revolution_Z(m_fAngle);
		Add_RenderGroup(RENDER_UI, this);
	}
	return 0;
}

void CHitBarUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CHitBarUI::Render_Object(void)
{
	_matrix      ViewMatrix;
	_matrix      matWorld, matView;

	matWorld = *m_pTransCom->Get_WorldMatrixPointer();

	matView = *D3DXMatrixIdentity(&matView);

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&m_ProjMatrix, &m_ProjMatrix);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix))))
		return;

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();
}

HRESULT CHitBarUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(HITBAR_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBAR_TEX, pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CHitBarUI * CHitBarUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHitBarUI *   pInstance = new CHitBarUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHitBarUI::Free(void)
{
	CGameObject::Free();
}
