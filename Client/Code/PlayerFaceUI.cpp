#include "stdafx.h"
#include "PlayerFaceUI.h"

#include "Export_Function.h"
#include "CubePlayer.h"

CPlayerFaceUI::CPlayerFaceUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CPlayerFaceUI::~CPlayerFaceUI()
{
}

HRESULT CPlayerFaceUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = -722.f;
	m_fY = 375.f;
	m_fSizeX = 146.f;
	m_fSizeY = 155.f;

	if (m_pPlayer == nullptr)
		m_pPlayer = Engine::Get_GameObject(L"Layer_Character", L"PLAYER");

	return S_OK;
}

_int CPlayerFaceUI::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayer == nullptr)
		m_pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");

	m_pTransCom->Set_Scale(m_fSizeX , m_fSizeY, 1.f);
	m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);

	m_fFrame += 5.f * fTimeDelta * 0.2f;

	if (m_fFrame >= 5.f)
		m_fFrame = 0.f;

	_int iResult = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	return iResult;
}

void CPlayerFaceUI::LateUpdate_Object(void)
{

	CGameObject::LateUpdate_Object();
}

void CPlayerFaceUI::Render_Object(void)
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

	if (dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp > 75.f)
	{
		m_pTexture_100->Set_Texture(m_pShaderCom, "g_DefaultTexture", (_ulong)m_fFrame);
	}
	else if (dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp <= 75.f &&
		dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp > 50.f)
	{
		m_pTexture_75->Set_Texture(m_pShaderCom, "g_DefaultTexture", (_ulong)m_fFrame);
	}
	else if (dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp <= 50.f &&
		dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp > 25.f)
	{
		m_pTexture_50->Set_Texture(m_pShaderCom, "g_DefaultTexture", (_ulong)m_fFrame);
	}
	else if (dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp <= 25.f &&
		dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp > 1.f)
	{
		m_pTexture_25->Set_Texture(m_pShaderCom, "g_DefaultTexture", (_ulong)m_fFrame);
	}
	else if (dynamic_cast<CCubePlayer*>(m_pPlayer)->Get_Ability()->fHp <= 0.f)
	{
		m_pTexture_0->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	}

	m_pShaderCom->Begin_Shader(0);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();
}


HRESULT CPlayerFaceUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pTexture_100 = dynamic_cast<CTexture*>(Clone_Proto(L"HP_100_Face"));
	NULL_CHECK_RETURN(m_pTexture_100, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"HP_100_Face", pComponent });

	pComponent = m_pTexture_75 = dynamic_cast<CTexture*>(Clone_Proto(L"HP_75_Face"));
	NULL_CHECK_RETURN(m_pTexture_75, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"HP_75_Face", pComponent });

	pComponent = m_pTexture_50 = dynamic_cast<CTexture*>(Clone_Proto(L"HP_50_Face"));
	NULL_CHECK_RETURN(m_pTexture_50, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"HP_50_Face", pComponent });

	pComponent = m_pTexture_25 = dynamic_cast<CTexture*>(Clone_Proto(L"HP_25_Face"));
	NULL_CHECK_RETURN(m_pTexture_25, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"HP_25_Face", pComponent });

	pComponent = m_pTexture_0 = dynamic_cast<CTexture*>(Clone_Proto(L"HP_0_Face"));
	NULL_CHECK_RETURN(m_pTexture_0, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"HP_0_Face", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });
	
	return S_OK;
}

CPlayerFaceUI * CPlayerFaceUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerFaceUI *   pInstance = new CPlayerFaceUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayerFaceUI::Free(void)
{
	CGameObject::Free();
}