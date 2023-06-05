#pragma once

#include "Base.h"
#include "Scene.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CManagement :	public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CLayer*					Get_Layer(const _tchar* pLayerTag);

	CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

	CComponent*				Get_Component(const _tchar* pLayerTag, 
		const _tchar* pObjTag, 
		const _tchar* pComponentTag,
		COMPONENTID eID);

	HRESULT					Add_GameObject(const _tchar* pLayerTag, CGameObject* pGameObject, const _tchar* pObjTag);
	HRESULT					Delete_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

	CScene*					Get_Scene(void);

public:
	HRESULT		Set_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		LateUpdate_Scene(void);
	void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	CScene*		m_pScene;

public:
	virtual void Free(void);
};
END
