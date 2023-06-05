#pragma once

#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent :	public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual _int			Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void			LateUpdate_Component(void) {}

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bClone;

public:
	virtual CComponent*	Clone(void)PURE;
	virtual void		Free(void);
};

END