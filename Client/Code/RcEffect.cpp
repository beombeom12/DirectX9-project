#include "stdafx.h"
#include "RcEffect.h"
#include "Shop.h"
#include "..\Header\RcEffect.h"

//»ç¿ë¹ý
//CGameObject*			m_pEffect = nullptr;
//m_pEffect = CRcEffect::Create(m_pGraphicDev, REDEFFECT_EFT); //enum EFFECTID
//dynamic_cast<CRcEffect*>(m_pEffect)->Set_EffectPos(vPos.x, vPos.y, vPos.z);//EFFECT POS
//dynamic_cast<CRcEffect*>(m_pEffect)->Set_Scale(10.f);
//dynamic_cast<CRcEffect*>(m_pEffect)->Set_SingleUse();
//dynamic_cast<CRcEffect*>(m_pEffect)->Dead_Timer(10.f);

//
static _int iEffectCnt = 0;

CRcEffect::CRcEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CRcEffect::~CRcEffect()
{

}

HRESULT CRcEffect::Ready_Object(_int iIndex)
{
	m_iNumber = iIndex;
	Index();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Static_Update();
	wsprintf(szCntName, L"RcEffect_%d", iEffectCnt);
	iEffectCnt++;

	
	return S_OK;
}

_int CRcEffect::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{		
		return -1;
	}
	CGameObject::Update_Object(fTimeDelta);
	m_pTransformCom->Billboard_Transform(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	m_fTimer += fTimeDelta;
	Dead_Condition();
	Effect_Change();
	return 0;
}

void CRcEffect::LateUpdate_Object(void)
{
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
}

void CRcEffect::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pTextureCom->Set_Texture(m_iTexIndex);
	//m_pRcTexCom->Render_Buffer();
}

HRESULT CRcEffect::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pRcTexCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pRcTexCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TRANSFORM_COMP, pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(szTextureName));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ szTextureName, pComponent });
	
	return S_OK;
}


void CRcEffect::Index()
{

	switch (m_iNumber)
	{
	case BLUEBOMB_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 2;
		m_iTexRecall = 0;
		wsprintf(szTextureName, BLUEBOMB_TEX);
		break;
	case REDEFFECT_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 9;
		m_iTexRecall = 0;
		wsprintf(szTextureName, REDEFFECT_TEX);
		break;
	case BLUEEFFECT_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 9;
		m_iTexRecall = 0;
		wsprintf(szTextureName, BLUEEFFECT_TEX);
		break;
	case CASTINGEFFECT_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 9;
		m_iTexRecall = 0;
		wsprintf(szTextureName, CASTINGEFFECT_TEX);
		break;
	case STAREFFECT_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 8;
		m_iTexRecall = 0;
		wsprintf(szTextureName, STAREFFECT_TEX);
		break;
	case COIN_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 7;
		m_iTexRecall = 0;
		wsprintf(szTextureName, COIN_TEX);
		break;
	case CREATEEFFECT_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 10;
		m_iTexRecall = 0;
		wsprintf(szTextureName, CREATEEFFECT_TEX);
		break;
	case EXPLOSION_EFT:
		m_fTexTimer = 0.1f;
		m_iTexBegin = 0;
		m_iTexEnd = 11;
		m_iTexRecall = 0;
		wsprintf(szTextureName, EXPLOSION_TEX);
		break;
	case LIGHTNING_EFT:
		m_fTexTimer = 0.05f;
		m_iTexBegin = 0;
		m_iTexEnd = 3;
		m_iTexRecall = 0;
		wsprintf(szTextureName, LIGHTNING_TEX);
		break;
	case REDBOMB_EFT:
		m_fTexTimer = 0.05f;
		m_iTexBegin = 0;
		m_iTexEnd = 7;
		m_iTexRecall = 0;
		wsprintf(szTextureName, REDBOMB_TEX);
		break;
	case STUN_EFT:
		m_fTexTimer = 0.15f;
		m_iTexBegin = 0;
		m_iTexEnd = 3;
		m_iTexRecall = 0;
		wsprintf(szTextureName, STUN_TEX);
		break;
	case EFT_END:
		break;
	}
	
}

void CRcEffect::Effect_Change()
{
	m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);

	if (m_bSingleUse)
	{
		if (m_fTimer > m_fTexTimer)
		{			
			if (m_iTexIndex < m_iTexEnd)
			{
				m_iTexIndex += 1;				
			}
			m_fTimer = 0.f;
		}
	}
	if (!m_bSingleUse)
	{
		if (m_fTimer > m_fTexTimer)
		{
			if (m_iTexIndex < m_iTexEnd)
			{
				m_iTexIndex+=1;				
			}
			if (m_iTexIndex >= m_iTexEnd)
			{
				m_iTexIndex = m_iTexRecall;
			}
			m_fTimer = 0.f;
		}
	}
}

void CRcEffect::Dead_Condition()
{
	if (m_bSingleUse)
	{
		if (m_iTexIndex >= m_iTexEnd)
		{
			m_bDead = true;
		}
	}
	if (m_fTimer > m_fDeadTimer)
	{
		if (m_iTexIndex >= m_iTexEnd)
		{
			m_bDead = true;
		}
	}
}


CRcEffect * CRcEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CRcEffect*   pInstance = new CRcEffect(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Maker(pInstance);

	return pInstance;
}

void CRcEffect::Free(void)
{
	CGameObject::Free();
}