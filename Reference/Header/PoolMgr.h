#pragma once

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPoolMgr : public CComponent
{
	DECLARE_SINGLETON(CPoolMgr)

private:
	explicit CPoolMgr();
	virtual ~CPoolMgr();

public:
	void Collect_Obj(CGameObject* pObj);
	CGameObject* Reuse_Obj(const _vec3& vPos, const _vec3& vDir);

private:
	void Release();

private:
	list<CGameObject*>		m_ObjectPool;
	int						m_iCount = 0;

private:
	virtual CComponent* Clone() { return nullptr; }
	virtual	void Free();
};

END