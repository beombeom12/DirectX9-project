#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr)
{
}


CManagement::~CManagement()
{
	Free();
}

CLayer * CManagement::Get_Layer(const _tchar * pLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Layer(pLayerTag);
}


CGameObject * CManagement::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag,pObjTag);
}

CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

HRESULT CManagement::Add_GameObject(const _tchar * pLayerTag, CGameObject * pGameObject, const _tchar* pObjTag)
{
	FAILED_CHECK_RETURN(m_pScene->Get_MapLayer()[pLayerTag]->Add_GameObject(pObjTag, pGameObject), E_FAIL);
	
	return S_OK;
}

HRESULT CManagement::Delete_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	FAILED_CHECK_RETURN(m_pScene->Get_MapLayer()[pLayerTag]->Delete_GameObject(pObjTag), E_FAIL);

	return S_OK;
}

CScene * CManagement::Get_Scene(void)
{
	return m_pScene;
}

HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);	 // 기존 scene을 삭제

	Engine::Clear_RenderGroup(); // 기존 scene에 그려지고 있던 모든 렌더 요소들을 삭제

	m_pScene = pScene;
	
	return S_OK;
}

_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(void)
{
	if (nullptr == m_pScene)
		return;

	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

