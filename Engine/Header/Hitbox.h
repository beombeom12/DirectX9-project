#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHitBox : public CVIBuffer
{
private:
	explicit CHitBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHitBox(const CHitBox& rhs);
	virtual ~CHitBox();

public:
	void				Get_MinMax(_vec3* _vMin, _vec3* _vMax);
	const _vec3*		Get_VtxPos(void) const { return vPos; }

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

private:
	_vec3	vMin = { 0,0,0 };
	_vec3	vMax = { 0,0,0 };
	_vec3*	vPos;
	_bool	m_bClone;

public:
	static CHitBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END