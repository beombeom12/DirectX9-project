#include "stdafx.h"
#include "..\Header\LetterBox.h"
#include "Inventory.h"
static _int   iLetterBoxCnt = 0;

CLetterBox::CLetterBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CLetterBox::~CLetterBox()
{
}

HRESULT CLetterBox::Ready_Object(_tchar* tDialogue, _int iSize, _int iIndex)
{
	m_iIndex = iIndex;

	m_fFontAlpha = 1.f;
	m_strLetterContents = tDialogue;
	m_iTextAmount = iSize - 3;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	iLetterBoxCnt++;

	return S_OK;
}

_int CLetterBox::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);

	if (m_bDead)
	{
		return -1;
	}

	return 0;
}

void CLetterBox::LateUpdate_Object(void)
{
	if (!m_bInit)
	{
		m_bInit = true;
		if (m_iIndex != 2)
		{
			m_fFontSizeX = 25.f;
			m_fFontSizeY = 40.f;
			wsprintf(szCntName, L"LetterBox%d", iLetterBoxCnt);
			FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, szCntName, L"Roboto-Bold", _uint(m_fFontSizeX), _uint(m_fFontSizeY), FW_HEAVY), );
		}
		if (m_iIndex == 2)
		{
			m_fFontSizeX = 7.5f;
			m_fFontSizeY = 15.f;
			wsprintf(szCntName, L"LetterBox%d", iLetterBoxCnt);
			FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, szCntName, L"Roboto-Bold", _uint(m_fFontSizeX), _uint(m_fFontSizeY), FW_HEAVY), );
		}

	}
	CGameObject::LateUpdate_Object();
}

void CLetterBox::Render_Object(void)
{
	if (m_iIndex == 0)
	{
		PlayerNotice();
		Alpha_Effect();
	}

	if (m_iIndex == 1)
	{
		PlayerNotice();
		Index1();
	}

	if (m_iIndex == 2)               //Inventory : Item : TagInfo
	{
		if (m_bPowerSwitch)
		{
			if (dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch())
			{
				BoxText();
				Begin_OrthoProj();
				m_pRcCom->Render_Buffer();
				End_OrthoProj();
			}
		}
	}

	if (m_iIndex == 3)
	{
		BoxText();
	}
}

HRESULT CLetterBox::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pRcCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pRcCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"LetterBox_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ STAGE_MAPPING, pComponent });

	pComponent = m_TranformCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_TranformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

void CLetterBox::Alpha_Effect()
{
	if (m_bPowerSwitch)
	{
		if (!m_bAlphaSwitch)
		{
			m_fFontAlpha -= 0.015f;
			if (m_fFontAlpha <= 0)
				m_bAlphaSwitch = true;
		}
		if (m_bAlphaSwitch)
		{
			m_fFontAlpha += 0.015f;
			if (m_fFontAlpha >= 1)
				m_bAlphaSwitch = false;
		}
	}
	if (!m_bPowerSwitch)
	{
		m_fFontAlpha = 1.f;
	}
}

void CLetterBox::Begin_OrthoProj()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	_matrix matWorld, matView, matOrtho;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_fBoxSizeX = WINCX / 6.f + 30.f;
	m_fBoxSizeY = WINCY / 4.f;


	matView.m[0][0] = m_fBoxSizeX;// 이미지 가로
	matView.m[1][1] = m_fBoxSizeY; // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = (-WINCX / 2.f + pt.x) * (WINCX / WINCY) + m_fBoxSizeX / 2 + 10.f;
	matView.m[3][1] = (WINCY / 2.f - pt.y) * (WINCX / WINCY) - m_fBoxSizeY / 2 - 50.f;
	matView.m[3][2] = 0.001f;

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

	m_pTexture->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
}

void CLetterBox::End_OrthoProj()
{
	m_pShaderCom->End_Shader();
}

void CLetterBox::PlayerNotice()
{
	if (m_bPowerSwitch)
	{
		Engine::Render_Font(szCntName, m_strLetterContents.c_str(), &(_vec2((WINCX / 2.f) - (m_iTextAmount)*(m_fFontSizeX / 4.f), (WINCY / 1.5f)  * (WINCX / WINCY))), D3DXCOLOR(1.f, 1.f, 1.f, m_fFontAlpha));
	}
}

void CLetterBox::BoxText()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	Engine::Render_Font(szCntName, m_strLetterContents.c_str(), &(_vec2(pt.x + 60.f, pt.y + 150.f)), D3DXCOLOR(1.f, 1.f, 1.f, m_fFontAlpha));
	//cout << m_fPosX << endl;

}

void CLetterBox::Index1()
{
	m_fFontAlpha -= 0.03f;
	if (m_fFontAlpha <= 0)
		m_bDead = true;
}

void CLetterBox::HitCombo()
{
}

void CLetterBox::DialogueBox()
{
}

CLetterBox * CLetterBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* tDialogue, _int iSize, _int iIndex)
{
	CLetterBox* pInstance = new CLetterBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tDialogue, iSize, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Maker(pInstance);

	return pInstance;
}

void CLetterBox::Free(void)
{
	CGameObject::Free();
}
