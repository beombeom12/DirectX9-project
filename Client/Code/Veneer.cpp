#include "stdafx.h"
#include "..\Header\Veneer.h"

CVeneer::CVeneer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CVeneer::~CVeneer()
{
}

HRESULT CVeneer::Ready_Object(const _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Set_Pos(vPos.x, 0.1f, vPos.z);
	m_pTransform->Set_Scale(8.f, 8.f, 1.f);
	m_pTransform->Rotation(ROT_X, D3DXToRadian(90.f));
	return S_OK;
}

_int CVeneer::Update_Object(const _float & fTimeDelta)
{
	m_fFrame += fTimeDelta;

	if (m_fFrame > 4.f)
	{
		return -1;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CVeneer::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CVeneer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pTexture->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CVeneer::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Veneer_Tex"));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Veneer_Tex", pInstance });

	pInstance = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pInstance });

	pInstance = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pInstance });

	return S_OK;

}

CVeneer * CVeneer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CVeneer* pInstance = new CVeneer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVeneer::Free(void)
{
	CGameObject::Free();
}
