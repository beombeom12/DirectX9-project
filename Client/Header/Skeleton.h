#pragma once
#include "Monster.h"

class CSkeleton : public CMonster
{
private:
	explicit CSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkeleton();

	virtual	HRESULT Ready_Object(const _vec3& vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT				Add_Component(void);

	HRESULT				Create_Item();

	_float				m_fFireAngle = 0.f;
	_float				m_fFrame = 0.f;

public:
	static CSkeleton*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
	virtual void		Free(void);
};

