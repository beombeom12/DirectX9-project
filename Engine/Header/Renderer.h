#pragma once


#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup(void);

	void      On_Shop(void) { m_bShopping = true; }
	void      Off_Shop(void) { m_bShopping = false; }

	list<CGameObject*>			Get_GameObjectGroup(RENDERID eId)
	{
		return m_RenderGroup[eId];
	}

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];
	_bool						m_bShopping = false;

private:
	virtual CComponent*	Clone(void) { return nullptr; }
	virtual void Free(void);
};
END