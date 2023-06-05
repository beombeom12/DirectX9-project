#include "stdafx.h"
#include "..\Header\Key.h"
#include "ItemCubeEffect.h"
CKey::CKey(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
}

CKey::~CKey()
{
}

HRESULT CKey::Ready_Object(const _vec3 & vPos, COLOR_ID eID)
{
	m_eID = eID;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();

	m_pHitBoxTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pHitBoxTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pHitBoxTransCom->Static_Update();

	return S_OK;
}

_int CKey::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		return -1;
	}

	CItem::Move_Item(fTimeDelta);
	CItem::Update_Object(fTimeDelta);
	Effect();
	return 0;
}

void CKey::LateUpdate_Object()
{
	CItem::LateUpdate_Object();
}

void CKey::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pDBufferCom->Render_Buffer();
}

HRESULT CKey::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	switch (m_eID)
	{
	case COLOR_BLUE:
		pComponent = m_pDBufferCom = dynamic_cast<CDynamicBuffer*>(Clone_Proto(BLUEKEY_ITEM_BUFFER));
		NULL_CHECK_RETURN(m_pDBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ BLUEKEY_ITEM_BUFFER, pComponent });

		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(BLUEKEY_ITEM_TEX));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ BLUEKEY_ITEM_TEX, pComponent });
		break;
	case COLOR_RED:
		pComponent = m_pDBufferCom = dynamic_cast<CDynamicBuffer*>(Clone_Proto(REDKEY_ITEM_BUFFER));
		NULL_CHECK_RETURN(m_pDBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ REDKEY_ITEM_BUFFER, pComponent });

		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(REDKEY_ITEM_TEX));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ REDKEY_ITEM_TEX, pComponent });
		break;
	case COLOR_YELLOW:
		pComponent = m_pDBufferCom = dynamic_cast<CDynamicBuffer*>(Clone_Proto(YELLOWKEY_ITEM_BUFFER));
		NULL_CHECK_RETURN(m_pDBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ YELLOWKEY_ITEM_BUFFER, pComponent });

		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(YELLOWKEY_ITEM_TEX));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ YELLOWKEY_ITEM_TEX, pComponent });
		break;
	}

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(m_pHitBox, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	pComponent = m_pHitBoxTransCom = dynamic_cast<CTransform*>(Clone_Proto(ITEM_TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pHitBoxTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ ITEM_TRANSFORM_COMP, pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	return S_OK;
}

void CKey::Effect()
{
	CItemCubeEffect* pItemCubeEffect = nullptr;

	if (!pItemCubeEffect)
		pItemCubeEffect = dynamic_cast<CItemCubeEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ItemCubeEffect"));
	_vec3 vPos;														//대쉬이펙트하려던것
	_vec3 vDir;
	_vec3 min = { -1.0f + rand() % 3 ,0.5 ,-1.0f + rand() % 3 };
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	for (_float i = 1.f; i < 4.f; i++)
	{
		for (_float j = -3.f; j < 3.f; j++)
		{
			for (_float k = 0.f; k < 1.f; k++)
			{
				dynamic_cast<CItemCubeEffect*>(pItemCubeEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.05f);
				dynamic_cast<CItemCubeEffect*>(pItemCubeEffect)->Set_PcleDir(min);
				//dynamic_cast<CItemCubeEffect*>(pItemCubeEffect)->Set_PcleMoveDir(_vec3(0,1,0));

				pItemCubeEffect->addParticle();
			}
		}
	}
}

CKey * CKey::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, COLOR_ID eID)
{
	CKey* pInstance = new CKey(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos,eID)))
	{
		MSG_BOX("CKey Item Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CKey::Free()
{
	CItem::Free();
}
