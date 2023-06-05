#include "stdafx.h"
#include "..\Header\Quest.h"
#include "LetterBox.h"
#include "Npc.h"

CQuest::CQuest(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CQuest::~CQuest()
{
}

HRESULT CQuest::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_fX = -660.f;
	m_fY = -300.f;
	m_fSizeX = 184.f * 1.5f;
	m_fSizeY = 206.f * 1.5f;

	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Quest", L"Roboto-Bold", 10, 20, FW_HEAVY), E_FAIL);
	return S_OK;
}

_int CQuest::Update_Object(const _float & fTimeDelta)
{
	Key_Input();

	if (!m_bSwitch)
		return 0;

	CNpc* pGameObject = dynamic_cast<CNpc*>(Get_GameObject(STAGE_ENVIRONMENT, L"NPC"));

	if (pGameObject != nullptr)
	{
		if (pGameObject->Get_QuestText1() && !m_bQuest1)
		{
			m_bQuest1 = true;
		}
		else if(pGameObject->Get_QuestText2() && !m_bQuest2)
		{
			m_bQuest2 = true;
		}
		else if (pGameObject->Get_QuestText3() && !m_bQuest3)
		{
			m_bQuest3 = true;
		}
	}

	m_pTransCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransCom->Set_Pos(m_fX, -m_fY, 0.f);
	_int iResult = CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_EFFECT_UI, this);

	return 0;
}

void CQuest::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CQuest::Render_Object(void)
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
	m_pShaderCom->Begin_Shader(1);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();
	
	if (m_bQuest1)
	{
		m_iWeapon = 3 - Get_Layer(STAGE_GUNITEM)->Get_GameObjectMap().size();
		m_strWeapon = L"¹«±â È¹µæ ( " + to_wstring(m_iWeapon) + L"/" + to_wstring(Get_Layer(STAGE_GUNITEM)->Get_GameObjectMap().size() + m_iWeapon) + L" )";
		Engine::Render_Font(L"Quest", m_strWeapon.c_str(), &_vec2(40.f, 35.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	if (m_bQuest2)
	{
		m_iKey = 3 - Get_Layer(L"STAGE_KEY")->Get_GameList().size();
		m_strKey = L"Å° È¹µæ ( " + to_wstring(m_iKey) + L"/" + to_wstring(Get_Layer(L"STAGE_KEY")->Get_GameList().size() + m_iKey) + L" )";
		Engine::Render_Font(L"Quest", m_strKey.c_str(), &_vec2(40.f, 85.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	if(m_bQuest3)
	{
		m_iMiddle = 1 - Get_Layer(L"STAGE_BOSS")->Get_GameObjectMap().size();
		m_strQuest3 = L"¹Ìµé º¸½º Ã³Ä¡( " + to_wstring(m_iMiddle) + L"/" + to_wstring(Get_Layer(L"STAGE_BOSS")->Get_GameObjectMap().size() + m_iMiddle) + L" )";
		Engine::Render_Font(L"Quest", m_strQuest3.c_str(), &_vec2(40.f, 135.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}

void CQuest::Key_Input()
{
	if (Key_Down(DIK_Q))
	{
		m_bSwitch = !m_bSwitch;
	}
}

HRESULT CQuest::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Quest_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Quest_Tex", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CQuest * CQuest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuest*   pInstance = new CQuest(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CQuest::Free(void)
{
	CGameObject::Free();
}
