#include "stdafx.h"
#include "..\Header\PlayerHpUI.h"

#include "Export_Function.h"
#include "CubePlayer.h"

CPlayerHpUI::CPlayerHpUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayerHpUI::~CPlayerHpUI()
{

}

HRESULT CPlayerHpUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = -442.f;
	m_fY = 428.f;
	m_fSizeX = 204.5f;
	m_fSizeY = 21.f;

	//FONT
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"HP", L"Electronic Highway Sign", 10, 15, FW_NORMAL), E_FAIL);
	return S_OK;
}

_int CPlayerHpUI::Update_Object(const _float & fTimeDelta)
{
	m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);

	_int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	if (m_pPlayer == nullptr)
		m_pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");

	if (m_pPlayer != nullptr)
	{
		m_fHp = dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp;
		m_fMaxHp = dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fMaxHp;
		m_strHp = to_wstring(_uint(m_fHp));
	}

	return iResult;
}

void CPlayerHpUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CPlayerHpUI::Render_Object(void)
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

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
	m_pBufferCom->Resize_Buffer(m_fHp / m_fMaxHp);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();

	Render_Font(L"HP", m_strHp.c_str(), &(_vec2(176.5f, 870.f)), D3DXCOLOR(0.5f, 0.5f, 0.3f, 1.f));
}

HRESULT CPlayerHpUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_HP_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_HP_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"HP_Gage"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"HP_Gage", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CPlayerHpUI * CPlayerHpUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerHpUI*   pInstance = new CPlayerHpUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayerHpUI::Free(void)
{
	CGameObject::Free();
}