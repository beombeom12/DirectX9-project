#pragma once

#include "Base.h"
#include "Engine_Include.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene :	public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CLayer*					Get_Layer(const _tchar* pLayerTag);
	CComponent*				Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag); 

	SCENE_ID				Get_SceneId() { return m_eID; }
	void					Set_SceneId(SCENE_ID eID) { m_eID = eID; }

	map<const _tchar*, CLayer*> Get_MapLayer() { return m_mapLayer; }

	void Add_Layer(CLayer* pLayer, _tchar* LayerName)
	{
		m_mapLayer.insert({ LayerName,pLayer });
	}

	void New_Layer(_tchar* LayerName)
	{
		Engine::CLayer*		pLayer = Engine::CLayer::Create();
		NULL_CHECK_RETURN(pLayer, );
		m_mapLayer.insert({ LayerName,pLayer });
	}

public:
	virtual		HRESULT		Ready_Scene(void);
	virtual		_int Update_Scene(const _float& fTimeDelta);
	virtual		void LateUpdate_Scene(void);
	virtual		void Render_Scene(void) {}

	

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;

	SCENE_ID						m_eID;

public:
	virtual void	Free(void);
};

END