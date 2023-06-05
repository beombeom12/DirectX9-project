#include "stdafx.h"
#include "..\Header\CrossHeader.h"
#include "CubePlayer.h"
#include "Weapon.h"

CCrossHeader::CCrossHeader(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCrossHeader::~CCrossHeader()
{
}

HRESULT CCrossHeader::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CCrossHeader::Update_Object(const _float & fTimeDelta)
{
	_int iResult = CGameObject::Update_Object(fTimeDelta);
	CGameObject* pPlayer = nullptr;
	pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");
	NULL_CHECK_RETURN(pPlayer, iResult);

	if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == nullptr)
		return iResult;

	if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1")))
	{
		m_eGunID = GUN_RIFLE;
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
	{
		m_eGunID = GUN_SHOTGUN;
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SNIPER")))
	{
		if(dynamic_cast<CCubePlayer*>(pPlayer)->Get_SniperZoom() == true)
			m_eGunID = GUN_SNIPER_ZOOMIN;	
		else
			m_eGunID = GUN_SNIPER_ZOOMOUT;
	}

	Add_RenderGroup(RENDER_UI, this);
	return iResult;
}

void CCrossHeader::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CCrossHeader::Render_Object(void)
{
	Begin_OrthoProj(m_eGunID);
	
	m_pBufferCom->Render_Buffer();

	End_OrthoProj();
}

void CCrossHeader::Begin_OrthoProj(GUN_ID eID)
{
	_matrix matWorld, matView, matOrtho;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	
	if (eID == GUN_RIFLE || eID == GUN_SNIPER_ZOOMOUT)
	{
		matView.m[0][0] = 20.f;	  // 이미지 가로
		matView.m[1][1] = 20.f;   // 이미지 세로
		matView.m[2][2] = 0.f;
		matView.m[3][0] = m_pTransCom->m_vInfo[INFO_POS].x;
		matView.m[3][1] = m_pTransCom->m_vInfo[INFO_POS].y;
	}
	else if(eID == GUN_SHOTGUN)
	{
		matView.m[0][0] = 30.f;	  // 이미지 가로
		matView.m[1][1] = 30.f;   // 이미지 세로
		matView.m[2][2] = 0.f;
		matView.m[3][0] = m_pTransCom->m_vInfo[INFO_POS].x;
		matView.m[3][1] = m_pTransCom->m_vInfo[INFO_POS].y;
	}
	else if (eID == GUN_SNIPER_ZOOMIN)
	{
		matView.m[0][0] = 1600.f;	  // 이미지 가로
		matView.m[1][1] = 900.f;	  // 이미지 세로
		matView.m[2][2] = 0.f;
		matView.m[3][0] = m_pTransCom->m_vInfo[INFO_POS].x;
		matView.m[3][1] = m_pTransCom->m_vInfo[INFO_POS].y;
	}
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

	switch (m_eGunID)
	{
	case CCrossHeader::GUN_RIFLE:
		m_pRifleTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
		break;
	case CCrossHeader::GUN_SHOTGUN:
		m_pShotGunTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
		break;
	case CCrossHeader::GUN_SNIPER_ZOOMIN:
		m_pSniperTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
		break;
	case CCrossHeader::GUN_SNIPER_ZOOMOUT:
		m_pRifleTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
		break;
	default:
		break;
	}
	m_pShaderCom->Begin_Shader(0);
}

void CCrossHeader::End_OrthoProj()
{
	m_pShaderCom->End_Shader();
}

HRESULT CCrossHeader::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({L"CCrossHeader_RcTexCom", pComponent});

	pComponent = m_pRifleTextureCom = dynamic_cast<CTexture*>(Clone_Proto(CROSSHAIR_RIFLE_TEX));
	NULL_CHECK_RETURN(m_pRifleTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"CCrossHeader_Rifle", pComponent });

	pComponent = m_pShotGunTextureCom = dynamic_cast<CTexture*>(Clone_Proto(CROSSHAIR_SHOTGUN_TEX));
	NULL_CHECK_RETURN(m_pShotGunTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"CCrossHeader_ShotGun", pComponent });

	pComponent = m_pSniperTextureCom = dynamic_cast<CTexture*>(Clone_Proto(CROSSHAIR_SNIPER_TEX));
	NULL_CHECK_RETURN(m_pSniperTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"CCrossHeader_Sniper", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

CCrossHeader * CCrossHeader::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossHeader*	pInstance = new CCrossHeader(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrossHeader::Free(void)
{
	CGameObject::Free();
}
