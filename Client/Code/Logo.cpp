#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_Function.h"
#include "Stage.h"
#include "ToolScene.h"
#include "LogoBilBoard.h"
#include "LogoCamera.h"
#include "Loading.h"
#include "PlayButton.h"
#include "OptionButton.h"
#include "LogoUI.h"
#include "ExitButton.h"
#include "LoadingBar.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Ready_Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Ready_Layer_UI"), E_FAIL);

	m_hVideo = MCIWndCreate(g_hWnd, nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, L"../Bin/Resources/Video/Space.wmv");

	// MoveWindow : 동영상을 재생할 크기를 설정

	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);
	MCIWndPlay(m_hVideo);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo)  && !m_bVideo)
	{
		MCIWndClose(m_hVideo);
		_float fBGMSound = 0.5f;
		PlayBGM(L"LogoBgm.mp3", fBGMSound);
		m_bVideo = true;
		return 0;
	}
	else
	{
		Engine::CScene::Update_Scene(fTimeDelta);

		if (m_pLoadingBar == nullptr)
			m_pLoadingBar = dynamic_cast<CLoadingBar*>(Engine::Get_GameObject(L"Ready_Layer_UI", L"LoadingBar"));

		if (m_pLoadingBar != nullptr)
		{
			if (m_pLoading == nullptr)
			{
				m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
				NULL_CHECK_RETURN(m_pLoading, E_FAIL);
			}
			else if (m_pLoading != nullptr)
			{
				m_Min++;
				m_Max = (_float)m_pLoading->Get_MaxPercentage();
			}
			dynamic_cast<CLoadingBar*>(m_pLoadingBar)->Set_Min(m_Min);
			dynamic_cast<CLoadingBar*>(m_pLoadingBar)->Set_Max(m_Max);
		}

		if (m_pLoading->Get_Finish())
		{
			if (m_PlayButton->Get_MouseCheck())
			{
				CScene*      pScene = CStage::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);
				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
				pScene->Set_SceneId(STAGE_SCENE);
				return 0;
			}

			if (m_OptionButton->Get_MouseCheck())
			{
				CScene*      pScene = CToolScene::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pScene, E_FAIL);
				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
				pScene->Set_SceneId(TOOL_SCENE);
				return 0;
			}
		}
	}
	return 0;
}

void CLogo::LateUpdate_Scene(void)
{
	if (m_pLoadingBar == nullptr)
		m_pLoadingBar = dynamic_cast<CLoadingBar*>(Engine::Get_GameObject(L"Ready_Layer_UI", L"LoadingBar"));
	if (m_Max == m_Min)
	{
		m_pLoadingBar->Kill_Obj();
	}
	Engine::CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{

}

HRESULT CLogo::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*      pGameObject = nullptr;

	pGameObject = CLogoCamera::Create(m_pGraphicDev, &_vec3(45.f, 10.f, 55.f), &_vec3(45.f, 0.f, 45.f), &_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	// Sky box
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CLogo::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*      pGameObject = nullptr;

	//pGameObject = CLogoUI::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LogoUI", pGameObject), E_FAIL);

	//PlayButton
	m_PlayButton = dynamic_cast<CPlayButton*> (pGameObject = CPlayButton::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayButton", pGameObject), E_FAIL);

	//OptionButton
	m_OptionButton = dynamic_cast<COptionButton*>(pGameObject = COptionButton::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OptionButton", pGameObject), E_FAIL);

	//ExitButton
	m_ExitButton = dynamic_cast<CExitButton*>(pGameObject = CExitButton::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ExitButton", pGameObject), E_FAIL);

	//LoadingBar
	m_pLoadingBar = dynamic_cast<CLoadingBar*>(pGameObject = CLoadingBar::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LoadingBar", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}
CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*   pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free(void)
{
	MCIWndClose(m_hVideo);
	Safe_Release(m_pLoading);
	Engine::CScene::Free();
}

HRESULT CLogo::Ready_Light(void)
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


HRESULT CLogo::Ready_Proto(void)
{
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LoadingBar", CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HudLoading", CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(TRANSFORM_COMP, CTransform::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CUBETEX_COMP, CCubeTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(TERRAINTEX_COMP, CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(SPHERETEX_COMP, CSphereTex::Create(m_pGraphicDev, 1.f)), E_FAIL);
	}
	{
		//Texture
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture_Stage_1", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture2D/tex_stone_3.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SkyBox_TEX", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Sky01.jpg", TEX_NORMAL)), E_FAIL);
	}
	{      //MENUUI
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_OptionButton", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/MenuUI/UI%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoUITexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/character_select_UI_1.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ButtonPlayTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/MenuUI/UI%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitButton", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/MenuUI/UI%d.png", TEX_NORMAL, 2)), E_FAIL);
	}
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HudLoadinga", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/LoadingUI/perk_selected.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ReadyLoading", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/LoadingUI/theHUDui_10.png", TEX_NORMAL)), E_FAIL);
	}


	{
		//Shader
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_SHADER, CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl"))), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CUBETEX_SHADER, CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Cube.hlsl"))), E_FAIL);
	}
	return S_OK;
}
