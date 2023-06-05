#include "stdafx.h"
#include "..\Header\MonsterUI.h"
#include "CubePlayer.h"
#include "Weapon.h"

CMonsterUI::CMonsterUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMonsterUI::~CMonsterUI()
{
}

HRESULT CMonsterUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"MonsterName", L"Roboto-Bold", 20, 15, FW_NORMAL), E_FAIL);
	
	m_fX = 0.f;
	m_fY = -300.f;
	m_fSizeX = 400.f;
	m_fSizeY = 20.f;

	m_fSizeHUDX = 410.f;
	m_fSizeHUDY = 31.f;
	return S_OK;
}

_int CMonsterUI::Update_Object(const _float & fTimeDelta)
{
	_int iResult = CGameObject::Update_Object(fTimeDelta);

	if (m_bSwitch)
	{
		m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
		m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);

		m_pHUDTransCom->Set_Scale(m_fSizeHUDX, m_fSizeHUDY, 1.f);
		m_pHUDTransCom->Set_Pos(m_fX, -m_fY, 0.f);
	
		m_strHp = to_wstring(_uint(m_fHp));
		m_strMaxHp = to_wstring(_uint(m_fMaxHp));

		Add_RenderGroup(RENDER_UI, this);
	}
	
	return iResult;
}

void CMonsterUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
	m_strMonsterUI = m_strObjName + L" ( " + m_strHp + L"/" + m_strMaxHp + L" ) ";
}

void CMonsterUI::Render_Object(void)
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
	m_pBufferCom->Resize_Buffer(m_fHp / m_fMaxHp);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();
	Render_Font(L"MonsterName", m_strMonsterUI.c_str(), &_vec2(630.f, 120.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CMonsterUI * CMonsterUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterUI* pInstance = new CMonsterUI(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonsterUI::Free(void)
{
	CGameObject::Free();
}

HRESULT CMonsterUI::Add_Component()
{
	CComponent* pComponent = nullptr;
	
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_GENERAL_MONSTER_HP_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_GENERAL_MONSTER_HP_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(MONSTER_HP_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Monsetr_HP" , pComponent });

	pComponent = m_pHUDBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_GENERAL_MONSTER_HUD_COMP));
	NULL_CHECK_RETURN(m_pHUDBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_GENERAL_MONSTER_HUD_COMP, pComponent });

	pComponent = m_pHUDTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pHUDTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Monster_HUD_Trans", pComponent });

	pComponent = m_pHUDTextureCom = dynamic_cast<CTexture*>(Clone_Proto(MONSTER_HP_HUD_TEX));
	NULL_CHECK_RETURN(m_pHUDTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Monsetr_HP_HUD" , pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}
