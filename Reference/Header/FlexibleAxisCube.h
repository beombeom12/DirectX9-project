#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CFlexibleAxisCube : public CVIBuffer
{
private:
	explicit CFlexibleAxisCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFlexibleAxisCube(const CFlexibleAxisCube& rhs);
	virtual ~CFlexibleAxisCube();

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

	const _vec3*		Get_VtxPos(void) const { return m_vPos; }
	//	해당 함수 사용시 변수에 넘겨받아서 사용할 것!
	_vec3*				Get_Flexible_Vtx(void) { return m_vPos; }

public:
	void		Update_Axis(void);

private:
	_vec3*		m_vPos;
	_bool		m_bClone;

public:
	void		Set_X(const _float& fMove) { m_AxisX = fMove; }
	void		Set_Y(const _float& fMove) { m_AxisY = fMove; }
	void		Set_Z(const _float& fMove) { m_AxisZ = fMove; }

	_float		Get_X(void) { return m_AxisX; }
	_float		Get_Y(void) { return m_AxisY; }
	_float		Get_Z(void) { return m_AxisZ; }

private:
	_float		m_AxisX;
	_float		m_AxisY;
	_float		m_AxisZ;

public:
	static CFlexibleAxisCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END