#include "stdafx.h"
#include "..\Header\CubeShop.h"
#include "LetterBox.h"
#include "Shop.h"

static _int m_iDialogCnt = 0;

CCubeShop::CCubeShop(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CCubeShop::~CCubeShop()
{
}

HRESULT CCubeShop::Ready_Object(const _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_component(), E_FAIL);

	m_pTransform->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	return S_OK;
}

_int CCubeShop::Update_Object(const _float & fTimeDelta)
{
	if (m_pShop == nullptr)
		m_pShop = dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"));

	Interact();
	Add_RenderGroup(RENDER_NONALPHA, this);
	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CCubeShop::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CCubeShop::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture(99);
	m_pCubeTexture->Render_Buffer();
}

HRESULT CCubeShop::Add_component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	pComponent = m_pCubeTexture = dynamic_cast<CCubeTex*>(Clone_Proto(CUBETEX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ CUBETEX_COMP, pComponent });

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pHitBox = dynamic_cast<CHitBox*>(Clone_Proto(HITBOX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ HITBOX_COMP, pComponent });

	return S_OK;
}

HRESULT CCubeShop::Interact()
{
	CGameObject*		pGameObject = nullptr;

	CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, E_FAIL);
	_vec3		vPlayerPos;
	_vec3		vPos;
	_vec3		vDir;
	_float		fDistance;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	pPlayerTransformCom->Get_Info(Engine::INFO_POS, &vPlayerPos);
	vDir = vPos - vPlayerPos;
	fDistance = D3DXVec3Length(&vDir);

	if (fDistance <= 5.f)
	{
		m_bSwitch = true;
	}
	if (fDistance >= 5.f)
	{
		m_bSwitch = false;
	}
	
	//if (!m_bDialogInit && m_bSwitch)
	//{
	//	m_bDialogInit = true;
	//
	//	m_pDialogBox = CLetterBox::Create(m_pGraphicDev, L"Press [E] to Interact", sizeof(L"Press [E] to Interact"), 0);
	//
	//	TCHAR* szCntName = new TCHAR[64];
	//	wsprintf(szCntName, L"");
	//	const _tchar*	szNumbering = L"ShopLetter_%d";
	//	wsprintf(szCntName, szNumbering, m_iDialogCnt);
	//	Engine::Add_GameObject(STAGE_MAPPING, m_pDialogBox, szCntName);
	//	m_listDialogCnt.push_back(szCntName);
	//
	//	++m_iDialogCnt;
	//}


	if (m_bSwitch && Key_Down(DIK_E))
	{
		dynamic_cast<CShop*>(m_pShop)->Set_Switch();
	}

	//if (m_bSwitch && m_pDialogBox != nullptr && !(dynamic_cast<CShop*>(m_pShop)->Get_Switch()))
	//{
	//	dynamic_cast<CLetterBox*>(m_pDialogBox)->On_Switch();
	//}
	//if (!m_bSwitch && m_pDialogBox != nullptr && dynamic_cast<CShop*>(m_pShop)->Get_Switch())
	//{
	//	dynamic_cast<CLetterBox*>(m_pDialogBox)->Off_Switch();
	//}

	return S_OK;
}

CCubeShop * CCubeShop::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CCubeShop* pInstance = new CCubeShop(pGraphicDev);

	if(FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCubeShop::Free(void)
{
	for (auto& iter : m_listDialogCnt)
	{
		if (iter != nullptr)
			delete iter;
	}

	m_listDialogCnt.clear();
	CGameObject::Free();
}
