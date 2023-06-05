#pragma once

#include "GameObject.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*				Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*			Get_GameObject(const _tchar* pObjTag);

	map<const _tchar*, CGameObject*>	Get_GameObjectMap()	{return m_mapObject;}
	map<const _tchar*, CGameObject*>*	Get_GameObjectMapPtr() { return &m_mapObject; }

	list<CGameObject*>			Get_GameList() { return m_ObjectList; }
	list<CGameObject*>*			Get_GameListPtr() { return &m_ObjectList; }

public:
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance);
	HRESULT			Delete_GameObject(const _tchar * pObjTag);
	HRESULT			Add_GameList(CGameObject* pInstance);

	HRESULT			Ready_Layer(void);
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(void);

private:
	map<const _tchar*, CGameObject*>			m_mapObject;
	list<CGameObject*>							m_ObjectList;

private:
	list<pair<const _tchar*, CGameObject*>>		m_ObjectPairList;

public:
	HRESULT										Add_GamePair(const _tchar* pObjTag, CGameObject* pInstance);
	HRESULT										Delete_GamePair(const _tchar* pObjTag);
	CGameObject*								Find_GamePair(const _tchar* pObjTag);	// 임의접근이 없는 List를 위한 Finder
	CComponent*									Get_PairComponent(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	list<pair<const _tchar*, CGameObject*>>		Get_GamePair(void) { return m_ObjectPairList; }
	list<pair<const _tchar*, CGameObject*>>*	Get_GamePairPtr(void) { return &m_ObjectPairList; }

public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};

END