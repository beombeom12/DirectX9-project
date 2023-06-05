#include "stdafx.h"
#include "..\Header\Ping.h"

CPing::CPing(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPing::~CPing()
{
}

HRESULT CPing::Ready_Object(const _vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Scale(0.5f, 0.5f, 0.f);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	return S_OK;
}

_int CPing::Update_Object(const _float & fTimeDelta)
{
	m_fFrame += fTimeDelta;
	m_fKillTime += fTimeDelta;

	if (m_fKillTime > 3.f)
	{
		return -1;
	}

	if (m_fFrame > 0.2f)
	{
		m_fReverseDir *= -1.f;
		m_fFrame = 0.f;
	}

	m_pTransCom->Move_Pos(&(_vec3(0.f,-1.f,0.f) * m_fReverseDir * fTimeDelta));

	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pTransCom->Billboard_Transform(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	return 0;
}

void CPing::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CPing::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CPing::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Ping_Tex"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Ping_Tex", pComponent });

	return S_OK;
}

CPing * CPing::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos)
{
	CPing* pInstance = new CPing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPing::Free(void)
{
	CGameObject::Free();
}
