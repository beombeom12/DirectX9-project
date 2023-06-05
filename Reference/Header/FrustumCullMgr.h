#pragma once

#include "Engine_Include.h"

enum PLANE_TYPE 
{
	PLANE_FRONT,
	PLANE_BACK,
	PLANE_UP,
	PLANE_DOWN,
	PLANE_LEFT,
	PLANE_RIGHT,

	PLANE_END
};

class ENGINE_DLL CFrustumCullMgr
{
	DECLARE_SINGLETON(CFrustumCullMgr)

private:
	explicit CFrustumCullMgr();
	virtual ~CFrustumCullMgr();

public:
	HRESULT		Update(const _matrix& matView, const _matrix& matProj);
	_bool		ContainsSphere(const _vec3& vPos, _float radius);

private:
	D3DXPLANE m_Planes[7];
};

