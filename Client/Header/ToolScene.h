#pragma once

#include "Engine_Include.h"
#include "Scene.h"

#include "Terrain.h"
#include "SkyBox.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"

class CToolScene : public Engine::CScene
{
private:
	explicit CToolScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolScene();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Camera(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Wall(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Tool(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Gun(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Shop(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Lava(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_ItemBox(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Throne(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Slime(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Fireman(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Zombie(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Illusioner(const _tchar* pLayerTag);

	HRESULT			Ready_Proto(void);
	HRESULT			Ready_Light(void);

public:
	void Set_Camera(CDynamicCamera* pCamera) { m_pCam = pCamera; }
	CDynamicCamera* Get_Camera() { return m_pCam; }

public:
	static CToolScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CLayer*				m_pCamLayer = nullptr;

private:
	CDynamicCamera*		m_pCam = nullptr;

private:
	virtual void	Free(void);
};

