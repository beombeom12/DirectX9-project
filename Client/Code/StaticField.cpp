#include "stdafx.h"
#include "..\Header\StaticField.h"


CStaticField::CStaticField(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CStaticField::~CStaticField()
{
}

HRESULT CStaticField::Ready_Object(const _vec3 & Position)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSphereScale = 5.f;
	m_fDuration = 3.f;
	m_pTransCom->Set_Pos(Position.x , Position.y , Position.z );

	m_pTransCom->Set_Scale(&_vec3(m_fSphereScale, m_fSphereScale, m_fSphereScale));

	m_iSphereSkillTag = SKILL_STATICFIELD;
	//m_bSphereSkill = true;

	return S_OK;
}

_int CStaticField::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	m_fAge += fTimeDelta;
	if (m_fAge > m_fDuration)
	{
		m_bDead = true;
		return -1;
	}
	return 0;
}

void CStaticField::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();

}

void CStaticField::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTexture->Set_Texture();
	m_pSphereTex->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pSphereTex2->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT CStaticField::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pSphereTex = dynamic_cast<CSphereTex*>(Engine::Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ SPHERETEX_COMP, pComponent });

	pComponent = m_pSphereTex2 = dynamic_cast<CSphereTex*>(Engine::Clone_Proto(SPHERETEX_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_SphereTexCom2", pComponent });

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Black_Tex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Black_Tex", pComponent });

	return S_OK;
}

CStaticField * CStaticField::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & Position)
{
	CStaticField* pInstance = new CStaticField(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Position)))
	{
		MSG_BOX("CStaticField Object Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStaticField::Free(void)
{
	CGameObject::Free();
}
