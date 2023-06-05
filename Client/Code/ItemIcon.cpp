#include "stdafx.h"
#include "Inventory.h"
#include "Shop.h"
#include "CubePlayer.h"
#include "..\Header\ItemIcon.h"
#include "LetterBox.h"
static _int iIconCnt = 0;

CItemIcon::CItemIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItemIcon::~CItemIcon()
{

}

HRESULT CItemIcon::Ready_Object(_int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iNumber = iIndex;
	wsprintf(szCntName, L"Contents_%d", iIconCnt);
	iIconCnt++;

	return S_OK;
}

_int CItemIcon::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		DeadIndex();
		return -1;
	}
	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);

	Index();
	FixOnCursor();
	IconCollision();
	IconTag();
	return 0;
}

void CItemIcon::LateUpdate_Object(void)
{
	if (!m_bInit)
	{
		m_bInit = true;
	}
}

void CItemIcon::Render_Object(void)
{
	if (dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch() && m_iNumber != 0 && !m_bWeaponPart)
	{
		Begin_OrthoProj();
		m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", m_iTexIndex);
		m_pShaderCom->Begin_Shader(0);
		m_pRcTexCom->Render_Buffer();
		End_OrthoProj();
	}
	if (m_iNumber == 0)
	{
		if (dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch() || dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch())
		{
			Begin_OrthoProj();
			if (m_iNumber == 0)
				m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
			m_pShaderCom->Begin_Shader(0);
			m_pRcTexCom->Render_Buffer();
			End_OrthoProj();
		}
	}
}


HRESULT CItemIcon::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pRcTexCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pRcTexCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_ItemIconTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ItemIconTexture", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

void CItemIcon::Begin_OrthoProj()
{
	m_pTransformCom->Get_Info(INFO_POS, &m_vIconPos);

	_matrix matWorld, matView, matOrtho;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = m_fImgX * WINCX / WINCY; // 이미지 가로
	matView.m[1][1] = m_fImgY * WINCY / WINCY;   // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = m_fImgX / 8 + m_vBlockPos.x * (WINCX / WINCY);
	matView.m[3][1] = m_vBlockPos.y * (WINCX / WINCY);
	matView.m[3][2] = m_fImgZ;

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
}

void CItemIcon::End_OrthoProj()
{
	m_pShaderCom->End_Shader();
}

void CItemIcon::Index()
{
	if (m_iNumber == 0)            //커서
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 30.f;
		m_fImgY = 50.f;
		m_fImgZ = 0.f;
	}
	if (m_iNumber == 1)             //아이템
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;
	}

	if (m_iNumber == 2)            //uzi
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 30.f;
		m_fImgY = 70.f;
	}
	if (m_iNumber == 3)            //Shotgun
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;
	}

	if (m_iNumber == 4)            //Sniper
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;
	}
	if (m_iNumber == 5)         // 스피드
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;

	}
	if (m_iNumber == 6)         // 데미지
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;
	}
	if (m_iNumber == 7)         // 장착스킬
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;
	}
	if (m_iNumber == 8)         // 먹는스킬
	{
		m_iTexIndex = m_iNumber;
		m_fImgX = 20.f;
		m_fImgY = 60.f;
	}
}

void CItemIcon::DeadIndex()
{
	if (m_iNumber == 8)
	{
	}

	if (m_iNumber == 8)
	{
		CGameObject* pPlayer = nullptr;
		if (!pPlayer)
			pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");
		dynamic_cast<CCubePlayer*>(pPlayer)->On_StaticField();

		_float fSound = 1.f;
		Engine::PlaySoundGun(L"Upgrade.wav", SOUND_EFFECT, fSound);
	}
	if (m_iNumber == 1)
	{
		CGameObject* pPlayer = nullptr;
		if (!pPlayer)
			pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");
		dynamic_cast<CCubePlayer*>(pPlayer)->On_Shield();

		_float fSound = 1.f;
		Engine::PlaySoundGun(L"Upgrade.wav", SOUND_EFFECT, fSound);
	}
}

void CItemIcon::FixOnCursor()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (m_bFix)
	{
		m_vBlockPos.x = -WINCX / 2.f + pt.x;//(double)( 1.75*(-450 +(pt.x*WINCY/WINCX)));
		m_vBlockPos.y = +WINCY / 2.f - pt.y;// (double)(1.75 * (250 - (pt.y*WINCY / WINCX)));
	}
	if (m_iNumber == 0)
	{
		m_vBlockPos.x = 15.f - WINCX / 2 + pt.x;//(double)( 1.75*(-450 +(pt.x*WINCY/WINCX)));
		m_vBlockPos.y = -25.f + WINCY / 2 - pt.y;// (double)(1.75 * (250 - (pt.y*WINCY / WINCX)));
	}
}

void CItemIcon::IconCollision()
{
	if (m_iNumber != 0)            //커서
	{
		POINT pt;
		POINT IconPointer;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		IconPointer.x = _long(m_vBlockPos.x + WINCX / 2 - m_fImgX / 2);
		IconPointer.y = _long(WINCY - (m_vBlockPos.y + WINCY / 2 + m_fImgY / 2));
		m_bIconCollision = false;

		if (IconPointer.x - m_fImgX < pt.x && pt.x < IconPointer.x + m_fImgX)
		{
			if (IconPointer.y < pt.y && pt.y < IconPointer.y + m_fImgY)
			{
				dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Tag_On();
				dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Set_TagIndex(m_iNumber);

				m_bIconCollision = true;
				cout << m_iNumber << endl;
			}
		}
	}


}

void CItemIcon::IconTag()
{

}

CItemIcon * CItemIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CItemIcon*   pInstance = new CItemIcon(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Maker(pInstance);
	return pInstance;
}

void CItemIcon::Free(void)
{
	CGameObject::Free();
}