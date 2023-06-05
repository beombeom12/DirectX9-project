#include "..\..\Header\Renderer.h"
#include "FrustumCullMgr.h"
#include "..\..\Client\Default\Define.h"
#include "Transform.h"
#include "..\..\Client\Header\TransAxisBox.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject * pGameObject)
{
	if (eID >= RENDER_END || nullptr == pGameObject)
		return;

	if (eID == RENDER_UI || eID == RENDER_PRIORITY || eID == RENDER_MAPSETTING || eID == RENDER_EFFECT_UI || !pGameObject->GetCheckFrustum())
	{
		m_RenderGroup[eID].push_back(pGameObject);
		pGameObject->AddRef();
	}
	else if (eID == RENDER_ANIOBJ && pGameObject->GetCheckFrustum())
	{
		_matrix matFinal;
		dynamic_cast<CTransAxisBox*>(pGameObject)->Get_Final(&matFinal);
		_vec3 vPos;
		memcpy(&vPos, &matFinal.m[3][0], sizeof(_vec3));
		_float fRadius = dynamic_cast<CTransAxisBox*>(pGameObject)->GetBoundingSphereRadius(matFinal);
		if (CFrustumCullMgr::GetInstance()->ContainsSphere
		(vPos, fRadius))
		{
			m_RenderGroup[eID].push_back(pGameObject);
			pGameObject->AddRef();
		}
	}
	else if (pGameObject->Get_Component(TRANSFORM_COMP, ID_DYNAMIC) != nullptr && pGameObject->GetCheckFrustum())
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		_vec3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);
		if (CFrustumCullMgr::GetInstance()->ContainsSphere
		(vPos,pTransform->GetBoundingSphereRadius() + 1.f))
		{
			m_RenderGroup[eID].push_back(pGameObject);
			pGameObject->AddRef();
		}
	}
	else if (pGameObject->Get_Component(TRANSFORM_COMP, ID_STATIC) != nullptr && pGameObject->GetCheckFrustum())
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(TRANSFORM_COMP, ID_STATIC));
		_vec3 vPos;
		pTransform->Get_Info(INFO_POS, &vPos);
		if (CFrustumCullMgr::GetInstance()->ContainsSphere
		(vPos, pTransform->GetBoundingSphereRadius() + 1.f))
		{
			m_RenderGroup[eID].push_back(pGameObject);
			pGameObject->AddRef();
		}
	}
	else
	{
		m_RenderGroup[eID].push_back(pGameObject);
		pGameObject->AddRef();
	}
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
	{
		iter->Render_Object();
		Safe_Release(iter);			
	}
	m_RenderGroup[RENDER_PRIORITY].clear();
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_NONALPHA].clear();

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_ALPHA].clear();

	for (auto& iter : m_RenderGroup[RENDER_ANIOBJ])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_ANIOBJ].clear();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	for (auto& iter : m_RenderGroup[RENDER_MAPSETTING])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_MAPSETTING].clear();
	
	for (auto& iter : m_RenderGroup[RENDER_EFFECT_UI])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	m_RenderGroup[RENDER_EFFECT_UI].clear();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		iter->Render_Object();
		Safe_Release(iter);
	}
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_RenderGroup[RENDER_UI].clear();
}

void CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}
