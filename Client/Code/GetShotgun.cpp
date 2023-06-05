#include "stdafx.h"
#include "..\Header\GetShotgun.h"
#include "CubePlayer.h"
#include "Inventory.h"
#include "ItemCubeEffect.h"
CGetShotgun::CGetShotgun(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CGetShotgun::~CGetShotgun()
{
}

HRESULT CGetShotgun::Ready_Object(const _vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransCom->Set_Rotation(ROT_Z, D3DXToRadian(270.f));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Static_Update();
	m_eSupporterID = SUPPORTER_SHOTGUN;
	m_pHitBoxTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pHitBoxTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pHitBoxTransCom->Static_Update();

	return S_OK;
}

_int CGetShotgun::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		dynamic_cast<CCubePlayer*>(Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER"))->Capture_Shotgun();
		dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->WeaponCreate(3);
		_float fSound = 1.f;
		PlaySoundW(L"SupporterDrop0.wav", SOUND_GET, fSound);
		return -1;
	}
	
	CItem::Move_Item(fTimeDelta);
	CItem::Update_Object(fTimeDelta);
	Effect();
	return 0;
}

void CGetShotgun::LateUpdate_Object()
{
	CItem::LateUpdate_Object();
}

void CGetShotgun::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pDBufferCom->Render_Buffer();
}

HRESULT CGetShotgun::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pDBufferCom = dynamic_cast<CDynamicBuffer*>(Clone_Proto(SHOTGUN_ITEM_BUFFER));
	NULL_CHECK_RETURN(m_pDBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SHOTGUN_ITEM_BUFFER, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(SHOTGUN_ITEM_TEX));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SHOTGUN_ITEM_TEX, pComponent });
	
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

void CGetShotgun::Effect()
{
	CItemCubeEffect* pItemCubeEffect = nullptr;

	if (!pItemCubeEffect)
		pItemCubeEffect = dynamic_cast<CItemCubeEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ItemCubeEffect"));
	_vec3 vPos;														//대쉬이펙트하려던것
	_vec3 vDir;
	_vec3 min = { -1.0f + rand() % 3 ,-1.0f + rand() % 3 ,-1.0f + rand() % 3 };
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	for (_float i = -1.f; i < 1.f; i++)
	{
		for (_float j = -1.f; j < 1.f; j++)
		{
			for (_float k = -1.f; k < 1.f; k++)
			{
				dynamic_cast<CItemCubeEffect*>(pItemCubeEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.0125f);
				dynamic_cast<CItemCubeEffect*>(pItemCubeEffect)->Set_PcleDir(min);
				pItemCubeEffect->addParticle();
			}
		}
	}
}

CGetShotgun * CGetShotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos)
{
	CGetShotgun* pInstance = new CGetShotgun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		MSG_BOX("GetShotgun Item Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGetShotgun::Free()
{
	CItem::Free();
}
