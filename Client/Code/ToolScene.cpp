#include "stdafx.h"
#include "..\Header\ToolScene.h"

#include "Export_Function.h"
#include "Wall.h"
#include "ImguiMgr.h"

CToolScene::CToolScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CToolScene::~CToolScene()
{
}

HRESULT CToolScene::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Wall(L"Layer_Wall"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Tool(L"Layer_MapTool"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(L"Layer_Cam"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Gun(L"Layer_Gun"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Shop(L"Layer_Shop"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Lava(L"Layer_Lava"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_ItemBox(L"Layer_ItemBox"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Throne(L"Layer_Throne"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Slime(L"Layer_Slime"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Fireman(L"Layer_FireMan"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Zombie(L"Layer_Zombie"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Illusioner(L"Layer_Illusioner"), E_FAIL);

	return S_OK;
}

_int CToolScene::Update_Scene(const _float & fTimeDelta)
{
	CImGuiMgr::GetInstance()->WindowLayOut();
	CImGuiMgr::GetInstance()->CreateWall(m_pGraphicDev, this, m_pCam);
	CImGuiMgr::GetInstance()->CreateObject(m_pGraphicDev, this, m_pCam);
	CImGuiMgr::GetInstance()->SwitchCamera(m_pGraphicDev, this, m_pCamLayer, m_pCam);
	
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CToolScene::LateUpdate_Scene(void)
{
	Engine::CScene::LateUpdate_Scene();
}

void CToolScene::Render_Scene(void)
{
}

HRESULT CToolScene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CToolScene::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, m_pCamLayer });

	return S_OK;
}

HRESULT CToolScene::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	m_pCamLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pCamLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = m_pCam = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pCamLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, m_pCamLayer });
	
	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Tool(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TerrainByTool", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Gun(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Shop(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Lava(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_ItemBox(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Throne(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Slime(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Fireman(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Zombie(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Layer_Illusioner(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });


	return S_OK;
}

HRESULT CToolScene::Ready_Proto(void)
{
	return S_OK;
}

HRESULT CToolScene::Ready_Light(void)
{
	D3DLIGHT9	Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));

	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Direction = _vec3(0.f, -1.f, 0.f);

	/*	Light.Position =
	Light.Range =
	Light.Falloff =
	Light.Attenuation0 =
	Light.Attenuation1 =
	Light.Attenuation2 =
	Light.Theta
	Light.Phi =
	*/
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &Light, 0), E_FAIL);

	return S_OK;
}

CToolScene * CToolScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolScene *	pInstance = new CToolScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CToolScene::Free(void)
{
	CScene::Free();
}
