#include "stdafx.h"
#include "..\Header\GunUI.h"

#include "Export_Function.h"
#include "CubePlayer.h"

CGunUI::CGunUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CGunUI::~CGunUI()
{
}

HRESULT CGunUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = 590.f;
	m_fY = 280.f;
	m_fSizeX = 188.8f;
	m_fSizeY = 100.f;
	m_iGunIndex = 0;

	return S_OK;
}

_int CGunUI::Update_Object(const _float & fTimeDelta)
{
	m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);
	_int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	if (m_pPlayer == nullptr)
		m_pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");

	if (m_pPlayer != nullptr)
	{
		m_iGunIndex = dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->iGunTexture;
	}
	return iResult;
}

void CGunUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CGunUI::Render_Object(void)
{
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

	if (dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Weapon() != nullptr)
	{
		m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iGunIndex);
		m_pShaderCom->Begin_Shader(0);
		m_pBufferCom->Render_Buffer();
		m_pShaderCom->End_Shader();
	}
}

HRESULT CGunUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Gun_UI"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Gun_UI", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CGunUI * CGunUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGunUI*   pInstance = new CGunUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGunUI::Free(void)
{
	CGameObject::Free();
}