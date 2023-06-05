#include "stdafx.h"
#include "..\Header\Warning_AnnihilateUI.h"


CWarning_AnnihilateUI::CWarning_AnnihilateUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CWarning_AnnihilateUI::~CWarning_AnnihilateUI()
{
}

HRESULT CWarning_AnnihilateUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);
	m_bSwitch = false;
	m_fX = 0;
	m_fY = -250;
	m_fSizeX = 600;
	m_fSizeY = 150;
	m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);
	return S_OK;
}

_int CWarning_AnnihilateUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bSwitch)
	{
		m_fTimer += fTimeDelta;
		CGameObject::Update_Object(fTimeDelta);
		Add_RenderGroup(RENDER_UI, this);
		Animate();
	}
	return 0;
}

void CWarning_AnnihilateUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CWarning_AnnihilateUI::Render_Object(void)
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

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iTexture);
	m_pShaderCom->Begin_Shader(0);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();
}

HRESULT CWarning_AnnihilateUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(WARNING_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ WARNING_TEX, pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

void CWarning_AnnihilateUI::Animate()
{	
	if (m_iTexture == 0 && m_fTimer > .125f)
	{
		m_iTexture += 1;
		m_fTimer = 0.f;
	}
	if (m_iTexture == 1 && m_fTimer > .125f)
	{
		m_iTexture -= 1;
		m_fTimer = 0.f;
	}
}

CWarning_AnnihilateUI * CWarning_AnnihilateUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWarning_AnnihilateUI *   pInstance = new CWarning_AnnihilateUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWarning_AnnihilateUI::Free(void)
{
	CGameObject::Free();
}
