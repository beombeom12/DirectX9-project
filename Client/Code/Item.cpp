#include "stdafx.h"
#include "..\Header\Item.h"

#include "Export_Function.h"
#include "ItemCubeEffect.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CItem::CItem(const CGameObject & rhs)
	:CGameObject(rhs)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Object(const _vec3& vPos)
{
	return S_OK;
}

_int CItem::Update_Object(const _float & fTimeDelta)
{
	
	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_PRIORITY, this);
	return 0;
}

void CItem::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CItem::Render_Object()
{
}

void CItem::Move_Item(const _float& fTimeDelta)
{
	m_fRotSpeed++;

	if (m_fRotSpeed > 180.f)
		m_fRotSpeed = -180.f;

	m_pTransCom->Set_Rotation(ROT_X, D3DXToRadian(m_fRotSpeed));
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	m_fFrame += fTimeDelta;

	if (m_fFrame > 1.f)
	{
		m_iReverseDir *= -1;
		m_fFrame = 0.f;
	}

	m_fYSpeed = fTimeDelta * m_iReverseDir * 0.5f;

	m_pTransCom->Set_Pos(vPos.x, vPos.y + m_fYSpeed, vPos.z);
}

void CItem::Free()
{
	CGameObject::Free();
}
